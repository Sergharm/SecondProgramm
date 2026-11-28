#include "ProcessManager.h"
#include "FileDescriptor.h"

#include <array>
#include <cerrno>
#include <csignal>
#include <stdexcept>
#include <system_error>
#include <thread>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

namespace {
void systemError(const char* action) {
    throw std::system_error(errno, std::generic_category(), action);
}
}

ProcessResult ProcessManager::run(const std::vector<std::string>& command,
                                  std::string_view program,
                                  std::chrono::milliseconds timeout) const {
    // Дескриптор 3 зарезервирован для передачи проверенного бинарника Bubblewrap.
    const FileDescriptor reserved(open("/dev/null", O_RDWR | O_CLOEXEC));
    if (reserved.get() < 0) systemError("Открытие /dev/null");
    const FileDescriptor sample(memfd_create("sample", MFD_CLOEXEC));
    const FileDescriptor output(memfd_create("trace", MFD_CLOEXEC));
    if (sample.get() < 0 || output.get() < 0) systemError("Создание временного файла");
    if (fcntl(output.get(), F_SETFL, O_APPEND) < 0) systemError("Настройка трассировки");

    while (!program.empty()) {
        const auto written = write(sample.get(), program.data(), program.size());
        if (written < 0 && errno == EINTR) continue;
        if (written <= 0) systemError("Копирование тестового файла");
        program.remove_prefix(static_cast<std::size_t>(written));
    }
    if (lseek(sample.get(), 0, SEEK_SET) < 0) systemError("Чтение копии тестового файла");

    std::vector<char*> arguments;
    for (const auto& arg : command) arguments.push_back(const_cast<char*>(arg.c_str()));
    arguments.push_back(nullptr);
    char locale[] = "LC_ALL=C";
    char* environment[] = {locale, nullptr};

    const auto pid = fork();
    if (pid < 0) systemError("Создание процесса");
    if (pid == 0) {
        if (dup2(output.get(), STDERR_FILENO) < 0 ||
            dup2(reserved.get(), STDIN_FILENO) < 0 ||
            dup2(reserved.get(), STDOUT_FILENO) < 0 ||
            (sample.get() == 3 ? fcntl(3, F_SETFD, 0) : dup2(sample.get(), 3)) < 0) {
            _exit(126);
        }
        execve(arguments.front(), arguments.data(), environment);
        constexpr char message[] = "Не удалось запустить Bubblewrap.\n";
        (void)write(STDERR_FILENO, message, sizeof(message) - 1);
        _exit(127);
    }

    const auto deadline = std::chrono::steady_clock::now() + timeout;
    int status = 0;
    bool timedOut = false;
    for (;;) {
        const auto result = waitpid(pid, &status, WNOHANG);
        if (result == pid) break;
        if (result < 0 && errno != EINTR) systemError("Ожидание Bubblewrap");
        if (std::chrono::steady_clock::now() >= deadline) {
            // Bubblewrap передаёт завершение процессам песочницы через --die-with-parent.
            timedOut = true;
            kill(pid, SIGKILL);
            while (waitpid(pid, &status, 0) < 0) {
                if (errno != EINTR) systemError("Ожидание после остановки Bubblewrap");
            }
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::string captured;
    std::array<char, 4096> buffer{};
    constexpr std::size_t maxOutput = 128 * 1024;
    while (captured.size() < maxOutput) {
        const auto count = pread(output.get(), buffer.data(), buffer.size(),
                                 static_cast<off_t>(captured.size()));
        if (count < 0 && errno == EINTR) continue;
        if (count < 0) systemError("Чтение трассировки");
        if (count == 0) break;
        captured.append(buffer.data(), static_cast<std::size_t>(count));
    }
    if (captured.size() == maxOutput) captured += "\nТрассировка ограничена 128 КиБ.\n";

    const int code = WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);
    return {code, timedOut, std::move(captured)};
}

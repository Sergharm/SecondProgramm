#include "Sandbox.h"
#include "AllowedTests.h"
#include "FileDescriptor.h"

#include <array>
#include <cerrno>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string_view>

#include <fcntl.h>
#include <sys/stat.h>

namespace fs = std::filesystem;

namespace {
constexpr auto timeLimit = std::chrono::seconds(5);

template <std::size_t size>
std::string_view bytes(const unsigned char (&test)[size]) {
    return {reinterpret_cast<const char*>(test), size};
}

void verifyFile(const fs::path& path, std::string_view expected) {
    const FileDescriptor file(open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW | O_NONBLOCK));
    struct stat info{};
    if (file.get() < 0 || fstat(file.get(), &info) < 0 || !S_ISREG(info.st_mode) ||
        info.st_size < 0 || static_cast<std::uintmax_t>(info.st_size) != expected.size()) {
        throw std::runtime_error("Файл отличается от бинарника, собранного вместе с LinuxSandbox.");
    }

    std::array<char, 4096> buffer{};
    for (;;) {
        const auto count = read(file.get(), buffer.data(), buffer.size());
        if (count < 0 && errno == EINTR) continue;
        if (count == 0 && expected.empty()) return;
        if (count <= 0 || expected.substr(0, count) != std::string_view(buffer.data(), count)) {
            throw std::runtime_error("Не удалось подтвердить содержимое тестового файла.");
        }
        expected.remove_prefix(static_cast<std::size_t>(count));
    }
}

std::string timestamp() {
    const auto now = std::time(nullptr);
    std::tm local{};
    localtime_r(&now, &local);
    std::ostringstream text;
    text << std::put_time(&local, "%Y-%m-%d %H:%M:%S %z");
    return text.str();
}
}

Sandbox::Sandbox(fs::path binaryDirectory) : binaryDirectory_(std::move(binaryDirectory)) {
    log("Сеанс LinuxSandbox v0.1 начат.");
}

void Sandbox::select(const fs::path& path) {
    if (path.empty()) throw std::runtime_error("Укажите путь к файлу.");
    std::error_code error;
    const auto resolved = fs::canonical(path, error);
    if (error == std::errc::no_such_file_or_directory) {
        throw std::runtime_error("Файл по указанному пути не найден.");
    }
    if (error) throw std::runtime_error("Не удалось открыть указанный путь.");
    if (!fs::is_regular_file(resolved)) throw std::runtime_error("Ожидается обычный файл.");
    if (access(resolved.c_str(), R_OK) != 0) throw std::runtime_error("Файл недоступен для чтения.");

    selected_ = resolved;
    lastAnalysis_.reset();
    log("Выбран файл " + selected_.string());
}

const fs::path& Sandbox::selected() const { return selected_; }

FileInfo Sandbox::fileInfo() const {
    if (selected_.empty()) throw std::runtime_error("Сначала выберите файл.");
    const FileDescriptor file(open(selected_.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW | O_NONBLOCK));
    struct stat info{};
    if (file.get() < 0 || fstat(file.get(), &info) < 0 || !S_ISREG(info.st_mode)) {
        throw std::runtime_error("Выбранный файл недоступен для чтения.");
    }
    std::array<unsigned char, 4> magic{};
    ssize_t count;
    do {
        count = read(file.get(), magic.data(), magic.size());
    } while (count < 0 && errno == EINTR);
    if (count < 0) throw std::runtime_error("Не удалось прочитать заголовок файла.");
    const bool elf = count == 4 && magic == std::array<unsigned char, 4>{0x7f, 'E', 'L', 'F'};
    return {selected_, static_cast<std::uintmax_t>(info.st_size),
            access(selected_.c_str(), X_OK) == 0, elf ? "ELF" : "Обычный файл"};
}

const Analysis& Sandbox::analyze() {
    lastAnalysis_.emplace();
    auto& analysis = *lastAnalysis_;
    try {
        const auto info = fileInfo();
        std::string_view approved;
        if (info.path == binaryDirectory_ / "test_tcp") approved = bytes(allowed::tcp);
        if (info.path == binaryDirectory_ / "test_udp") approved = bytes(allowed::udp);
        if (approved.empty()) {
            throw std::runtime_error("Разрешены только test_tcp и test_udp рядом с LinuxSandbox.");
        }
        if (!info.executable) throw std::runtime_error("Тестовый файл не имеет права на исполнение.");
        verifyFile(info.path, approved);
        const auto command = isolation_.command();
        log("Начат анализ " + info.path.filename().string() + " через Bubblewrap и strace.");
        const auto result = process_.run(command, approved, timeLimit);
        auto trace = monitor_.parse(result.output);
        analysis.events = std::move(trace.events);
        analysis.diagnostics = std::move(trace.diagnostics);
        if (result.timedOut) {
            analysis.status = "Превышен лимит в 5 секунд. Анализ остановлен.";
        } else if (result.exitCode != 0) {
            analysis.status = "Анализ прерван. Код завершения " + std::to_string(result.exitCode) + '.';
        } else if (analysis.events.empty()) {
            analysis.status = "Анализ прерван. Сетевые события в трассе отсутствуют.";
        } else {
            analysis.status = "Анализ завершён.";
        }
    } catch (const std::exception& error) {
        analysis.status = std::string("Запуск отклонён. ") + error.what();
    }

    log(analysis.status + " Сетевых событий " + std::to_string(analysis.events.size()) + '.');
    if (!analysis.diagnostics.empty()) log(analysis.diagnostics);
    return analysis;
}

const std::optional<Analysis>& Sandbox::lastAnalysis() const { return lastAnalysis_; }
const std::vector<std::string>& Sandbox::journal() const { return journal_; }

void Sandbox::log(const std::string& message) {
    journal_.push_back('[' + timestamp() + "] " + message);
}

fs::path Sandbox::saveReport() {
    if (!lastAnalysis_ || selected_.empty()) {
        throw std::runtime_error("Выберите файл и запустите анализ перед сохранением отчёта.");
    }
    const auto& analysis = *lastAnalysis_;
    std::ostringstream report;
    report << "LINUX SANDBOX v0.1 — отчёт\n"
           << "Дата: " << timestamp() << '\n'
           << "Файл: " << selected_.string() << '\n'
           << "Статус: " << analysis.status << "\n\n"
           << "Сетевые события (" << analysis.events.size() << ")\n"
           << "PID\tВызов\tПротокол\tIP\tПорт\tСтатус\tРезультат\n";
    const auto field = [](const std::string& value) { return value.empty() ? "-" : value; };
    for (const auto& event : analysis.events) {
        report << (event.pid == 0 ? "-" : std::to_string(event.pid)) << '\t'
               << event.call << '\t' << field(event.protocol) << '\t' << field(event.address) << '\t'
               << field(event.port) << '\t' << (event.success ? "УСПЕХ" : "ОШИБКА")
               << '\t' << event.result << '\n';
    }

    report << "\nЖурнал сеанса\n";
    for (const auto& entry : journal_) report << entry << '\n';
    const auto path = reports_.save(report.str());
    log("Сохранён отчёт " + path.string());
    return path;
}

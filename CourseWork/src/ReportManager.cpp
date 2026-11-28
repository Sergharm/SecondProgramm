#include "ReportManager.h"
#include "FileDescriptor.h"

#include <cerrno>
#include <stdexcept>
#include <system_error>

std::filesystem::path ReportManager::save(const std::string& text) const {
    std::filesystem::create_directories("reports");
    char path[] = "reports/analysis-XXXXXX.txt";
    const FileDescriptor file(mkstemps(path, 4));
    if (file.get() < 0) {
        throw std::system_error(errno, std::generic_category(), "Создание отчёта");
    }

    std::size_t offset = 0;
    while (offset < text.size()) {
        const auto written = write(file.get(), text.data() + offset, text.size() - offset);
        if (written < 0 && errno == EINTR) continue;
        if (written <= 0) {
            const int error = written < 0 ? errno : EIO;
            unlink(path);
            throw std::system_error(error, std::generic_category(), "Запись отчёта");
        }
        offset += static_cast<std::size_t>(written);
    }
    return path;
}

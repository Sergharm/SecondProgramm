#include "ConsoleUI.h"
#include "Sandbox.h"

#include <filesystem>
#include <iostream>

int main() {
    try {
        Sandbox sandbox(std::filesystem::canonical("/proc/self/exe").parent_path());
        ConsoleUI(sandbox).run();
    } catch (const std::exception& error) {
        std::cerr << "Не удалось запустить LinuxSandbox: " << error.what() << '\n';
        return 1;
    }
}

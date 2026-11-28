#include "ConsoleUI.h"
#include "Sandbox.h"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {
const char* menu = R"(
========================================
          LINUX SANDBOX v0.1
========================================
1. Выбрать исследуемый файл
2. Информация о файле
3. Запустить анализ
4. Просмотреть сетевые события
5. Просмотреть журнал
6. Сохранить отчёт
0. Выход
)";

void printEvents(const Analysis& analysis) {
    std::cout << analysis.status << "\nСетевых событий: " << analysis.events.size() << '\n';
    for (const auto& event : analysis.events) {
        std::cout << "  [" << (event.success ? "УСПЕХ" : "ОШИБКА") << "] PID ";
        if (event.pid) std::cout << event.pid;
        else std::cout << '-';
        std::cout << "  " << event.call;
        if (!event.protocol.empty()) std::cout << "  " << event.protocol;
        if (!event.address.empty()) std::cout << "  IP " << event.address;
        if (!event.port.empty()) std::cout << "  порт " << event.port;
        std::cout << "  => " << event.result << '\n';
    }
}
}

ConsoleUI::ConsoleUI(Sandbox& sandbox) : sandbox_(sandbox) {}

void ConsoleUI::run() {
    for (;;) {
        std::cout << menu << "\nВыбранный файл: "
                  << (sandbox_.selected().empty() ? "не указан" : sandbox_.selected().string())
                  << "\nВведите номер команды: " << std::flush;

        std::string input;
        if (!std::getline(std::cin, input)) break;
        const auto first = input.find_first_not_of(" \t\r");
        const auto last = input.find_last_not_of(" \t\r");
        const auto command = first == std::string::npos ? "" : input.substr(first, last - first + 1);
        try {
            if (command == "0") break;
            if (command == "1") {
                std::cout << "Введите путь к файлу: " << std::flush;
                if (!std::getline(std::cin, input)) break;
                sandbox_.select(input);
                std::cout << "Файл выбран.\n";
            } else if (command == "2") {
                const auto info = sandbox_.fileInfo();
                std::cout << "Путь: " << info.path.string() << "\nТип: " << info.type
                          << "\nРазмер: " << info.size << " байт\nДоступ на исполнение: "
                          << (info.executable ? "да" : "нет") << '\n';
            } else if (command == "3") {
                const auto& analysis = sandbox_.analyze();
                std::cout << analysis.status << '\n';
                if (!analysis.diagnostics.empty()) std::cout << analysis.diagnostics;
                std::cout << "Сетевых событий: " << analysis.events.size() << '\n';
            } else if (command == "4") {
                if (!sandbox_.lastAnalysis()) throw std::runtime_error("Сначала запустите анализ.");
                printEvents(*sandbox_.lastAnalysis());
            } else if (command == "5") {
                std::cout << "Журнал сеанса\n";
                for (const auto& entry : sandbox_.journal()) std::cout << entry << '\n';
            } else if (command == "6") {
                const auto report = sandbox_.saveReport();
                std::cout << "Отчёт сохранён: " << report.string() << '\n';
            } else {
                std::cout << "Некорректная команда. Введите число от 0 до 6.\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Ошибка: " << error.what() << '\n';
        }
    }
    std::cout << "Сеанс завершён.\n";
}

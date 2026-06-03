#include "ui.h"
#include <iostream>
#include <clocale>

void UserInterface::show_menu() {
    std::cout << "Меню:\n";
    std::cout << "1. Шифрование текста\n";
    std::cout << "2. Расшифрование текста\n";
    std::cout << "3. Шифрование файла\n";
    std::cout << "4. Расшифрование файла\n";
    std::cout << "5. Генерация ключа\n";
    std::cout << "0. Выход\n";
}

void UserInterface::run() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int choice;
    do {
        show_menu();
        std::cout << "Выбор: ";
        std::cin >> choice;
    } while (choice != 0);
}

void UserInterface::process_text(bool is_encrypt) {}
void UserInterface::process_file(bool is_encrypt) {}
void UserInterface::generate_key() {}

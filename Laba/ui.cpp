#include "ui.h"
#include "library_loader.h"
#include <iostream>
#include <clocale>
#include <limits>
#include <vector>

void UserInterface::show_menu() {
    std::cout << "Меню:\n";
    std::cout << "1. Шифрование текста\n";
    std::cout << "2. Расшифрование текста\n";
    std::cout << "3. Шифрование файла\n";
    std::cout << "4. Расшифрование файла\n";
    std::cout << "5. Генерация ключа\n";
    std::cout << "0. Выход\n";
}

void UserInterface::process_text(bool is_encrypt) {
    std::string algo;
    std::cout << "Алгоритм (affine/playfair): ";
    std::cin >> algo;
    
    try {
        LibraryLoader loader(algo);
        auto info = loader.get_info();
        
        std::cout << "Размер ключа: " << info->key_size << " байт\n";
        std::cout << "Ключ (hex): ";
        std::string key_hex;
        std::cin >> key_hex;
        
        std::vector<uint8_t> key(info->key_size);
        for (size_t i = 0; i < info->key_size; ++i) {
            key[i] = std::stoul(key_hex.substr(i*2, 2), nullptr, 16);
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Текст: ";
        std::string text;
        std::getline(std::cin, text);
        
        std::vector<uint8_t> input(text.begin(), text.end());
        std::vector<uint8_t> output(input.size());
        
        ConstBuffer k{key.data(), key.size()};
        ConstBuffer in{input.data(), input.size()};
        MutBuffer out{output.data(), output.size()};
        
        int status = loader.execute_crypto(is_encrypt, k, in, &out);
        if (status == 0) {
            std::string result(output.begin(), output.end());
            std::cout << "Результат: " << result << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

void UserInterface::run() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int choice;
    do {
        show_menu();
        std::cout << "Выбор: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: process_text(true); break;
            case 2: process_text(false); break;
        }
    } while (choice != 0);
}

void UserInterface::process_file(bool is_encrypt) {}
void UserInterface::generate_key() {}

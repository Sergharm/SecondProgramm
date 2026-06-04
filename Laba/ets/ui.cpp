#include "ui.h"
#include "library_loader.h"
#include "file_processor.h"
#include "key_generator.h"
#include <iostream>
#include <clocale>
#include <limits>
#include <vector>
#include <filesystem>

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
    
    if (algo.empty()) {
        std::cerr << "Ошибка: алгоритм не указан\n";
        return;
    }
    
    try {
        LibraryLoader loader(algo);
        auto info = loader.get_info();
        
        std::cout << "Размер ключа: " << info->key_size << " байт\n";
        std::string key_hex;
        while (true) {
            std::cout << "Ключ (hex): ";
            std::cin >> key_hex;
            if (key_hex.length() == info->key_size * 2) break;
            std::cout << "Ошибка: требуется " << info->key_size * 2 << " символов\n";
        }
        
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
        } else {
            std::cerr << "Ошибка шифрования: код " << status << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

void UserInterface::process_file(bool is_encrypt) {
    std::string algo, in_path, out_path;
    std::cout << "Алгоритм: ";
    std::cin >> algo;
    std::cout << "Входной файл: ";
    std::cin >> in_path;
    
    if (!std::filesystem::exists(in_path)) {
        std::cerr << "Ошибка: файл не найден\n";
        return;
    }
    
    std::cout << "Выходной файл: ";
    std::cin >> out_path;
    
    try {
        LibraryLoader loader(algo);
        auto info = loader.get_info();
        
        std::vector<uint8_t> key(info->key_size, 0x00);
        std::vector<uint8_t> input = FileProcessor::read_file(in_path);
        std::vector<uint8_t> output(input.size());
        
        ConstBuffer k{key.data(), key.size()};
        ConstBuffer in{input.data(), input.size()};
        MutBuffer out{output.data(), output.size()};
        
        int status = loader.execute_crypto(is_encrypt, k, in, &out);
        if (status == 0) {
            FileProcessor::write_file(out_path, output);
            std::cout << "Готово: " << out_path << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

void UserInterface::generate_key() {
    std::string algo, path;
    std::cout << "Алгоритм: ";
    std::cin >> algo;
    std::cout << "Сохранить в: ";
    std::cin >> path;
    
    try {
        LibraryLoader loader(algo);
        auto info = loader.get_info();
        
        auto key = KeyGenerator::generate_key(info->key_size);
        FileProcessor::write_file(path, key);
        std::cout << "Ключ создан: " << path << " (" << key.size() << " байт)\n";
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
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        switch(choice) {
            case 1: process_text(true); break;
            case 2: process_text(false); break;
            case 3: process_file(true); break;
            case 4: process_file(false); break;
            case 5: generate_key(); break;
        }
    } while (choice != 0);
}

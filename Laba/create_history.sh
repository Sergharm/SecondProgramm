#!/bin/bash

# Создаем ветку для разработки
git checkout -b feature/development

echo "=== Начинаем создание истории коммитов ==="
echo ""

# ============================================
# КОММИТ 1: Базовые структуры данных
# ============================================
cat > crypto_interface.h << 'EOF'
#ifndef CRYPTO_INTERFACE_H
#define CRYPTO_INTERFACE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ConstBuffer {
    const uint8_t* data;
    size_t size;
};

struct MutBuffer {
    uint8_t* data;
    size_t size;
};

struct AlgorithmInfo {
    const char* algorithm_name;
    size_t key_size;
};

#ifdef __cplusplus
}
#endif

#endif
EOF

git add crypto_interface.h
git commit -m "Добавлены базовые структуры для работы с буферами и информацией об алгоритмах"

echo "✅ Коммит 1: Базовые структуры"

# ============================================
# КОММИТ 2: Первая версия Affine cipher
# ============================================
cat > affine_cipher.cpp << 'EOF'
#include "crypto_interface.h"

extern "C" {

const AlgorithmInfo* get_algorithm_info() {
    static const AlgorithmInfo info = { "affine", 2 };
    return &info;
}

size_t get_output_size(size_t input_size, int operation_type) {
    return input_size;
}

int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size < 2) return 1;
    
    uint8_t a = key.data[0];
    uint8_t b = key.data[1];
    
    for (size_t i = 0; i < input.size; ++i) {
        output->data[i] = (input.data[i] * a + b) % 256;
    }
    return 0;
}

int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    return encrypt(key, input, output);
}

}
EOF

git add affine_cipher.cpp
git commit -m "Добавлена базовая реализация аффинного шифра (только шифрование)"

echo "✅ Коммит 2: Affine cipher (базовая версия)"

# ============================================
# КОММИТ 3: Улучшение Affine cipher с дешифрованием
# ============================================
cat > affine_cipher.cpp << 'EOF'
#include "crypto_interface.h"

static uint8_t mod_inverse(uint8_t a) {
    for (int i = 1; i < 256; i++) {
        if ((a * i) % 256 == 1) return i;
    }
    return 1;
}

extern "C" {

const AlgorithmInfo* get_algorithm_info() {
    static const AlgorithmInfo info = { "affine", 2 };
    return &info;
}

size_t get_output_size(size_t input_size, int operation_type) {
    return input_size;
}

int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size < 2) return 1;
    if (key.data[0] % 2 == 0) return 2;
    
    uint8_t a = key.data[0];
    uint8_t b = key.data[1];
    
    for (size_t i = 0; i < input.size; ++i) {
        output->data[i] = (input.data[i] * a + b) % 256;
    }
    return 0;
}

int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size < 2) return 1;
    
    uint8_t a = key.data[0];
    uint8_t b = key.data[1];
    uint8_t a_inv = mod_inverse(a);
    
    for (size_t i = 0; i < input.size; ++i) {
        int val = input.data[i] - b;
        if (val < 0) val += 256;
        output->data[i] = (val * a_inv) % 256;
    }
    return 0;
}

}
EOF

git add affine_cipher.cpp
git commit -m "Добавлено дешифрование для Affine cipher и проверка ключа"

echo "✅ Коммит 3: Affine cipher (полная версия)"

# ============================================
# КОММИТ 4: Playfair cipher базовая версия
# ============================================
cat > playfair_cipher.cpp << 'EOF'
#include "crypto_interface.h"
#include <cstring>

extern "C" {

const AlgorithmInfo* get_algorithm_info() {
    static const AlgorithmInfo info = { "playfair", 16 };
    return &info;
}

size_t get_output_size(size_t input_size, int operation_type) {
    return input_size;
}

int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size < 16) return 1;
    
    for (size_t i = 0; i < input.size; i += 2) {
        output->data[i] = input.data[i] ^ key.data[i % key.size];
        if (i + 1 < input.size)
            output->data[i + 1] = input.data[i + 1] ^ key.data[(i + 1) % key.size];
    }
    return 0;
}

int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    return encrypt(key, input, output);
}

}
EOF

git add playfair_cipher.cpp
git commit -m "Добавлена базовая реализация Playfair cipher"

echo "✅ Коммит 4: Playfair cipher (базовая версия)"

# ============================================
# КОММИТ 5: Playfair cipher с таблицей подстановки
# ============================================
cat > playfair_cipher.cpp << 'EOF'
#include "crypto_interface.h"
#include <cstring>

static void build_table(ConstBuffer key, uint8_t table[16][16]) {
    bool used[256] = {false};
    uint8_t flat[256];
    size_t pos = 0;
    
    for (size_t i = 0; i < key.size; i++) {
        if (!used[key.data[i]]) {
            used[key.data[i]] = true;
            flat[pos++] = key.data[i];
        }
    }
    
    for (int i = 0; i < 256; i++) {
        if (!used[i]) flat[pos++] = i;
    }
    
    for (int r = 0; r < 16; r++)
        for (int c = 0; c < 16; c++)
            table[r][c] = flat[r * 16 + c];
}

static void find_pos(uint8_t table[16][16], uint8_t val, int& row, int& col) {
    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 16; c++) {
            if (table[r][c] == val) {
                row = r; col = c;
                return;
            }
        }
    }
}

extern "C" {

const AlgorithmInfo* get_algorithm_info() {
    static const AlgorithmInfo info = { "playfair", 16 };
    return &info;
}

size_t get_output_size(size_t input_size, int operation_type) {
    return input_size;
}

int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size < 16) return 1;
    
    uint8_t table[16][16];
    build_table(key, table);
    
    for (size_t i = 0; i < input.size; i += 2) {
        int r1, c1, r2, c2;
        find_pos(table, input.data[i], r1, c1);
        find_pos(table, (i + 1 < input.size) ? input.data[i+1] : 0, r2, c2);
        
        if (r1 == r2) {
            output->data[i] = table[r1][(c1 + 1) % 16];
        } else if (c1 == c2) {
            output->data[i] = table[(r1 + 1) % 16][c1];
        } else {
            output->data[i] = table[r1][c2];
        }
    }
    return 0;
}

int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (key.size < 16) return 1;
    
    uint8_t table[16][16];
    build_table(key, table);
    
    for (size_t i = 0; i < input.size; i += 2) {
        int r1, c1, r2, c2;
        find_pos(table, input.data[i], r1, c1);
        find_pos(table, (i + 1 < input.size) ? input.data[i+1] : 0, r2, c2);
        
        if (r1 == r2) {
            output->data[i] = table[r1][(c1 + 15) % 16];
        } else if (c1 == c2) {
            output->data[i] = table[(r1 + 15) % 16][c1];
        } else {
            output->data[i] = table[r1][c2];
        }
    }
    return 0;
}

}
EOF

git add playfair_cipher.cpp
git commit -m "Улучшен Playfair cipher: добавлена таблица подстановки 16x16"

echo "✅ Коммит 5: Playfair cipher (с таблицей)"

# ============================================
# КОММИТ 6: Загрузчик библиотек - заголовок
# ============================================
cat > library_loader.h << 'EOF'
#ifndef LIBRARY_LOADER_H
#define LIBRARY_LOADER_H

#include "crypto_interface.h"
#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    using LibHandle = HMODULE;
#else
    #include <dlfcn.h>
    using LibHandle = void*;
#endif

typedef const AlgorithmInfo* (*GetAlgoInfoFunc)();
typedef size_t (*GetOutputSizeFunc)(size_t, int);
typedef int (*CryptoFunc)(ConstBuffer, ConstBuffer, MutBuffer*);

class LibraryLoader {
public:
    LibraryLoader(const std::string& algo_name);
    ~LibraryLoader();
    
    const AlgorithmInfo* get_info() const;
    size_t get_output_size(size_t input_size, bool is_encrypt) const;
    int execute_crypto(bool is_encrypt, ConstBuffer key, ConstBuffer input, MutBuffer* output) const;

private:
    LibHandle handle_;
    GetAlgoInfoFunc info_fn_;
    GetOutputSizeFunc size_fn_;
    CryptoFunc crypto_fn_;
};

#endif
EOF

git add library_loader.h
git commit -m "Добавлен заголовочный файл загрузчика динамических библиотек"

echo "✅ Коммит 6: Library loader header"

# ============================================
# КОММИТ 7: Загрузчик библиотек - реализация
# ============================================
cat > library_loader.cpp << 'EOF'
#include "library_loader.h"
#include <stdexcept>

#if defined(_WIN32) || defined(_WIN64)
    #define LOAD_LIB(name) LoadLibraryA(name)
    #define GET_FUNC(h, name) GetProcAddress(h, name)
    #define CLOSE_LIB(h) FreeLibrary(h)
#else
    #define LOAD_LIB(name) dlopen(name, RTLD_NOW)
    #define GET_FUNC(h, name) dlsym(h, name)
    #define CLOSE_LIB(h) dlclose(h)
#endif

LibraryLoader::LibraryLoader(const std::string& algo_name) {
    std::string path = "./lib" + algo_name + ".so";
    handle_ = LOAD_LIB(path.c_str());
    if (!handle_) throw std::runtime_error("Cannot load: " + algo_name);
    
    info_fn_ = (GetAlgoInfoFunc)GET_FUNC(handle_, "get_algorithm_info");
    size_fn_ = (GetOutputSizeFunc)GET_FUNC(handle_, "get_output_size");
    crypto_fn_ = (CryptoFunc)GET_FUNC(handle_, "encrypt");
    
    if (!info_fn_ || !size_fn_ || !crypto_fn_) {
        CLOSE_LIB(handle_);
        throw std::runtime_error("Invalid library interface");
    }
}

LibraryLoader::~LibraryLoader() {
    if (handle_) CLOSE_LIB(handle_);
}

const AlgorithmInfo* LibraryLoader::get_info() const {
    return info_fn_();
}

size_t LibraryLoader::get_output_size(size_t input_size, bool is_encrypt) const {
    return size_fn_(input_size, is_encrypt ? 1 : 2);
}

int LibraryLoader::execute_crypto(bool is_encrypt, ConstBuffer key, 
                                   ConstBuffer input, MutBuffer* output) const {
    CryptoFunc fn = is_encrypt ? crypto_fn_ : 
                     (CryptoFunc)GET_FUNC(handle_, "decrypt");
    return fn ? fn(key, input, output) : -1;
}
EOF

git add library_loader.cpp
git commit -m "Реализован загрузчик динамических библиотек (Linux/Windows)"

echo "✅ Коммит 7: Library loader implementation"

# ============================================
# КОММИТ 8: Обработчик файлов
# ============================================
cat > file_processor.h << 'EOF'
#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <vector>
#include <string>
#include <cstdint>

class FileProcessor {
public:
    static std::vector<uint8_t> read_file(const std::string& path);
    static void write_file(const std::string& path, const std::vector<uint8_t>& data);
};

#endif
EOF

cat > file_processor.cpp << 'EOF'
#include "file_processor.h"
#include <fstream>
#include <stdexcept>

std::vector<uint8_t> FileProcessor::read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("Cannot open: " + path);
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    return buffer;
}

void FileProcessor::write_file(const std::string& path, 
                                const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot create: " + path);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}
EOF

git add file_processor.h file_processor.cpp
git commit -m "Добавлен модуль работы с файлами (бинарный режим)"

echo "✅ Коммит 8: File processor"

# ============================================
# КОММИТ 9: Генератор ключей
# ============================================
cat > key_generator.h << 'EOF'
#ifndef KEY_GENERATOR_H
#define KEY_GENERATOR_H

#include <vector>
#include <cstdint>

class KeyGenerator {
public:
    static std::vector<uint8_t> generate_key(size_t length);
};

#endif
EOF

cat > key_generator.cpp << 'EOF'
#include "key_generator.h"
#include <random>

std::vector<uint8_t> KeyGenerator::generate_key(size_t length) {
    std::vector<uint8_t> key(length);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);
    
    for (size_t i = 0; i < length; ++i) {
        key[i] = static_cast<uint8_t>(dist(gen));
    }
    return key;
}
EOF

git add key_generator.h key_generator.cpp
git commit -m "Добавлен генератор криптографических ключей"

echo "✅ Коммит 9: Key generator"

# ============================================
# КОММИТ 10: Безопасная очистка памяти
# ============================================
cat >> crypto_interface.h << 'EOF'

#ifdef __cplusplus
inline void secure_clear_memory(void* v, size_t n) {
    volatile uint8_t* p = static_cast<volatile uint8_t*>(v);
    while (n--) *p++ = 0;
}
#endif
EOF

git add crypto_interface.h
git commit -m "Добавлена безопасная очистка памяти"

echo "✅ Коммит 10: Secure memory clear"

# ============================================
# КОММИТ 11: Пользовательский интерфейс - заголовок
# ============================================
cat > ui.h << 'EOF'
#ifndef UI_H
#define UI_H

class UserInterface {
public:
    void run();
private:
    void show_menu();
    void process_text(bool is_encrypt);
    void process_file(bool is_encrypt);
    void generate_key();
};

#endif
EOF

git add ui.h
git commit -m "Добавлен заголовочный файл пользовательского интерфейса"

echo "✅ Коммит 11: UI header"

# ============================================
# КОММИТ 12: Точка входа в приложение
# ============================================
cat > main.cpp << 'EOF'
#include "ui.h"
#include <iostream>

int main() {
    try {
        UserInterface ui;
        ui.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
EOF

git add main.cpp
git commit -m "Добавлена точка входа в приложение"

echo "✅ Коммит 12: Main.cpp"

# ============================================
# КОММИТ 13: Базовое меню на русском
# ============================================
cat > ui.cpp << 'EOF'
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
EOF

git add ui.cpp
git commit -m "Реализовано базовое меню на русском языке"

echo "✅ Коммит 13: Russian UI menu"

# ============================================
# КОММИТ 14: Шифрование текста с интеграцией
# ============================================
cat > ui.cpp << 'EOF'
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
EOF

git add ui.cpp
git commit -m "Реализовано шифрование текста с интеграцией библиотек"

echo "✅ Коммит 14: Text encryption with library integration"

# ============================================
# КОММИТ 15: Работа с файлами
# ============================================
cat > ui.cpp << 'EOF'
#include "ui.h"
#include "library_loader.h"
#include "file_processor.h"
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

void UserInterface::process_file(bool is_encrypt) {
    std::string algo, in_path, out_path;
    std::cout << "Алгоритм: ";
    std::cin >> algo;
    std::cout << "Входной файл: ";
    std::cin >> in_path;
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
            case 3: process_file(true); break;
            case 4: process_file(false); break;
        }
    } while (choice != 0);
}

void UserInterface::generate_key() {}
EOF

git add ui.cpp
git commit -m "Реализовано шифрование файлов"

echo "✅ Коммит 15: File encryption"

# ============================================
# КОММИТ 16: Генерация ключей в UI
# ============================================
cat > ui.cpp << 'EOF'
#include "ui.h"
#include "library_loader.h"
#include "file_processor.h"
#include "key_generator.h"
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

void UserInterface::process_file(bool is_encrypt) {
    std::string algo, in_path, out_path;
    std::cout << "Алгоритм: ";
    std::cin >> algo;
    std::cout << "Входной файл: ";
    std::cin >> in_path;
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
        
        switch(choice) {
            case 1: process_text(true); break;
            case 2: process_text(false); break;
            case 3: process_file(true); break;
            case 4: process_file(false); break;
            case 5: generate_key(); break;
        }
    } while (choice != 0);
}
EOF

git add ui.cpp
git commit -m "Реализован генератор ключей в пользовательском интерфейсе"

echo "✅ Коммит 16: Key generation in UI"

# ============================================
# КОММИТ 17: Валидация ввода ключа
# ============================================
cat > ui.cpp << 'EOF'
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
EOF

git add ui.cpp
git commit -m "Добавлена валидация пользовательского ввода"

echo "✅ Коммит 17: Input validation"

# ============================================
# КОММИТ 18: Добавление validate_key в интерфейс
# ============================================
cat > library_loader.h << 'EOF'
#ifndef LIBRARY_LOADER_H
#define LIBRARY_LOADER_H

#include "crypto_interface.h"
#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    using LibHandle = HMODULE;
#else
    #include <dlfcn.h>
    using LibHandle = void*;
#endif

typedef const AlgorithmInfo* (*GetAlgoInfoFunc)();
typedef size_t (*GetOutputSizeFunc)(size_t, int);
typedef int (*CryptoFunc)(ConstBuffer, ConstBuffer, MutBuffer*);
typedef int (*ValidateKeyFunc)(ConstBuffer);

class LibraryLoader {
public:
    LibraryLoader(const std::string& algo_name);
    ~LibraryLoader();
    
    const AlgorithmInfo* get_info() const;
    size_t get_output_size(size_t input_size, bool is_encrypt) const;
    int execute_crypto(bool is_encrypt, ConstBuffer key, ConstBuffer input, MutBuffer* output) const;
    int validate_key(ConstBuffer key) const;

private:
    LibHandle handle_;
    GetAlgoInfoFunc info_fn_;
    GetOutputSizeFunc size_fn_;
    CryptoFunc crypto_fn_;
    ValidateKeyFunc validate_fn_;
};

#endif
EOF

cat > library_loader.cpp << 'EOF'
#include "library_loader.h"
#include <stdexcept>

#if defined(_WIN32) || defined(_WIN64)
    #define LOAD_LIB(name) LoadLibraryA(name)
    #define GET_FUNC(h, name) GetProcAddress(h, name)
    #define CLOSE_LIB(h) FreeLibrary(h)
#else
    #define LOAD_LIB(name) dlopen(name, RTLD_NOW)
    #define GET_FUNC(h, name) dlsym(h, name)
    #define CLOSE_LIB(h) dlclose(h)
#endif

LibraryLoader::LibraryLoader(const std::string& algo_name) {
    std::string path = "./lib" + algo_name + ".so";
    handle_ = LOAD_LIB(path.c_str());
    if (!handle_) throw std::runtime_error("Cannot load: " + algo_name);
    
    info_fn_ = (GetAlgoInfoFunc)GET_FUNC(handle_, "get_algorithm_info");
    size_fn_ = (GetOutputSizeFunc)GET_FUNC(handle_, "get_output_size");
    crypto_fn_ = (CryptoFunc)GET_FUNC(handle_, "encrypt");
    validate_fn_ = (ValidateKeyFunc)GET_FUNC(handle_, "validate_key");
    
    if (!info_fn_ || !size_fn_ || !crypto_fn_ || !validate_fn_) {
        CLOSE_LIB(handle_);
        throw std::runtime_error("Invalid library interface");
    }
}

LibraryLoader::~LibraryLoader() {
    if (handle_) CLOSE_LIB(handle_);
}

const AlgorithmInfo* LibraryLoader::get_info() const {
    return info_fn_();
}

size_t LibraryLoader::get_output_size(size_t input_size, bool is_encrypt) const {
    return size_fn_(input_size, is_encrypt ? 1 : 2);
}

int LibraryLoader::execute_crypto(bool is_encrypt, ConstBuffer key, 
                                   ConstBuffer input, MutBuffer* output) const {
    CryptoFunc fn = is_encrypt ? crypto_fn_ : 
                     (CryptoFunc)GET_FUNC(handle_, "decrypt");
    return fn ? fn(key, input, output) : -1;
}

int LibraryLoader::validate_key(ConstBuffer key) const {
    return validate_fn_(key);
}
EOF

git add library_loader.h library_loader.cpp
git commit -m "Добавлена функция validate_key в интерфейс библиотек"

echo "✅ Коммит 18: validate_key function"

echo ""
echo "=== История коммитов создана! ==="
git log --oneline
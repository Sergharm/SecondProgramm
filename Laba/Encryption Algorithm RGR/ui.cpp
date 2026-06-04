#include "ui.h"
#include "library_loader.h"
#include "file_processor.h"
#include "key_generator.h"
#include "Menu.h" 
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <limits>
#include <csignal>
#include <cstdlib>

using namespace std;
namespace fs = std::filesystem;

// Обработчик сигнала Ctrl+C
void signal_handler(int signum) {
    cout << "\n\nПрограмма завершена пользователем.\n";
    exit(0);
}

void setup_signal_handler() {
    signal(SIGINT, signal_handler);
}

// Функция авторизации
bool login() {
    string password;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    while (attempts < MAX_ATTEMPTS) {
        cout << "Введите пароль: ";
        cin >> password;

        if (password == "admin") {
            cout << "Авторизация успешна!" << endl;
            return true;
        } else {
            attempts++;
            cout << "Неверный пароль! Осталось попыток: " << (MAX_ATTEMPTS - attempts) << endl;
        }
    }
    
    cout << "Превышено количество попыток. Доступ запрещен." << endl;
    return false;
}

// Вспомогательные функции ввода
string UserInterface::safe_input_string(const string& prompt) {
    string result;
    while (true) {
        cout << prompt;
        if (getline(cin, result) && !result.empty()) {
            return result;
        }
        cout << "Ввод не может быть пустым. Попробуйте снова.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string UserInterface::safe_input_path(const string& prompt) {
    string path;
    while (true) {
        cout << prompt << ": ";
        if (getline(cin, path) && !path.empty()) {
            return path;
        }
        cout << "Путь не может быть пустым. Попробуйте снова.\n";
        cin.clear();
    }
}

bool UserInterface::safe_input_yes_no(const string& prompt, bool default_yes) {
    string answer;
    while (true) {
        cout << prompt << " (Y/n): ";
        if (getline(cin, answer)) {
            if (answer.empty()) return default_yes;
            
            char c = tolower(answer[0]);
            if (c == 'y') return true;
            if (c == 'n') return false;
        }
        cout << "Ответьте Y или N.\n";
    }
}

string UserInterface::get_auto_text_filename(bool is_encrypt) const {
    return is_encrypt ? "text_encrypted.txt" : "text_decrypted.txt";
}

string UserInterface::get_auto_output_path(const string& input_path, bool is_encrypt) const {
    fs::path p(input_path);
    string suffix = is_encrypt ? "_enc" : "_dec";
    
    fs::path parent = p.parent_path();
    if (parent.empty()) {
        parent = fs::current_path();
    }
    
    string new_name = p.stem().string() + suffix + p.extension().string();
    return (parent / new_name).string();
}

// Основная функция запуска
void UserInterface::run() {
    setup_signal_handler();
    
    cout << "=== Encryption Algorithm RGR ===\n\n";
    
    // Авторизация
    if (!login()) {
        return;
    }
    
    int input = -1;
    
    while (input != Menu::EXIT) {
        cout << endl << "\t Меню " << endl;
        cout << "Выберите действие:" << endl;
        cout << "1. Шифрование текста" << endl;
        cout << "2. Расшифрование текста" << endl;
        cout << "3. Шифрование файла" << endl;
        cout << "4. Расшифрование файла" << endl;
        cout << "5. Генерация ключа" << endl;
        cout << "0. Выход" << endl;
        cout << ">> ";
        
        if (!(cin >> input)) {
            cout << "Ошибка! Введите числовое значение" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            input = -1;
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        try {
            switch (input) {
                case Menu::ENCRYPT_TEXT:
                    cout << "\n--- Шифрование текста ---\n";
                    process_text(true);
                    break;
                    
                case Menu::DECRYPT_TEXT:
                    cout << "\n--- Расшифрование текста ---\n";
                    process_text(false);
                    break;
                    
                case Menu::ENCRYPT_FILE:
                    cout << "\n--- Шифрование файла ---\n";
                    process_file(true);
                    break;
                    
                case Menu::DECRYPT_FILE:
                    cout << "\n--- Расшифрование файла ---\n";
                    process_file(false);
                    break;
                    
                case Menu::GENERATE_KEY:
                    cout << "\n--- Генерация ключа ---\n";
                    generate_and_save_key();
                    break;
                    
                case Menu::EXIT:
                    cout << "Завершение работы.\n";
                    break;
                    
                default:
                    cout << "Ошибка! Такого действия нет" << endl;
                    break;
            }
        } catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << "\n";
        }
        
        if (input != Menu::EXIT) {
            cout << "\nНажмите Enter для возврата в меню...";
            cin.get();
            cout << "\n";
        }
    }
}

void UserInterface::show_menu() {
    // Не используется, меню теперь в run()
}

// Ввод ключа (hex или файл)
vector<uint8_t> input_key(size_t key_size) {
    vector<uint8_t> key(key_size);
    string key_input;
    
    cout << "Размер ключа: " << key_size << " байт\n";
    
    while (true) {
        cout << "Ключ (hex) или путь к файлу ключа: ";
        getline(cin, key_input);
        
        if (key_input.empty()) {
            cout << "Ввод не может быть пустым.\n";
            continue;
        }
        
        // Проверяем, является ли ввод путем к файлу
        bool is_path = (key_input.find('/') != string::npos || 
                       key_input.find('\\') != string::npos ||
                       key_input.find('.') != string::npos);
        
        if (is_path && fs::exists(key_input)) {
            try {
                vector<uint8_t> file_key = FileProcessor::read_file(key_input);
                if (file_key.size() < key_size) {
                    cout << "Ошибка: размер ключа в файле (" << file_key.size() 
                         << " байт) меньше требуемого (" << key_size << " байт).\n";
                    continue;
                }
                key = vector<uint8_t>(file_key.begin(), file_key.begin() + key_size);
                return key;
            } catch (const exception& e) {
                cout << "Ошибка чтения файла ключа: " << e.what() << "\n";
                continue;
            }
        } else {
            // Пытаемся интерпретировать как hex-строку
            string clean_key;
            for (char c : key_input) {
                if (c != ' ') clean_key += tolower(c);
            }
            
            if (clean_key.length() != key_size * 2) {
                cout << "Ошибка: требуется " << key_size * 2 
                     << " hex-символов (сейчас " << clean_key.length() << ").\n";
                continue;
            }
            
            bool valid = true;
            for (char c : clean_key) {
                if (!isxdigit(c)) {
                    valid = false;
                    break;
                }
            }
            
            if (!valid) {
                cout << "Ошибка: ключ должен содержать только hex-цифры (0-9, a-f).\n";
                continue;
            }
            
            for (size_t i = 0; i < key_size; ++i) {
                key[i] = static_cast<uint8_t>(stoul(clean_key.substr(i * 2, 2), nullptr, 16));
            }
            return key;
        }
    }
}

void UserInterface::process_text(bool is_encrypt) {
    string algo = safe_input_string("Алгоритм (affine/playfair): ");
    
    LibraryLoader loader(algo);
    const AlgorithmInfo* info = loader.get_info();
    
    vector<uint8_t> key = input_key(info->key_size);
    
    ConstBuffer k_buf{key.data(), key.size()};
    if (loader.validate_key(k_buf) != 0) {
        secure_clear_memory(key.data(), key.size());
        throw runtime_error("Ключ не подходит для данного алгоритма.");
    }
    
    string text = safe_input_string(is_encrypt ? "Текст для шифрования: " : "Шифротекст: ");
    
    vector<uint8_t> input_data(text.begin(), text.end());
    size_t out_size = loader.get_output_size(input_data.size(), is_encrypt);
    vector<uint8_t> output_data(out_size);
    
    ConstBuffer in_buf{input_data.data(), input_data.size()};
    MutBuffer out_buf{output_data.data(), out_size};
    
    int status = loader.execute_crypto(is_encrypt, k_buf, in_buf, &out_buf);
    if (status != 0) {
        secure_clear_memory(key.data(), key.size());
        throw runtime_error("Ошибка операции. Код: " + to_string(status));
    }
    
    output_data.resize(out_buf.size);
    string result(output_data.begin(), output_data.end());
    
    cout << "\nРезультат:\n" << result << "\n";
    
    cout << "\nСохранить результат в файл? ";
    if (safe_input_yes_no("", true)) {
        string save_path = safe_input_path("Путь к файлу");
        
        try {
            fs::path p(save_path);
            if (p.has_parent_path() && !p.parent_path().empty()) {
                fs::path dir = p.parent_path();
                if (!fs::exists(dir)) {
                    cout << "Директория \"" << dir.string() << "\" не существует. Создать? ";
                    if (safe_input_yes_no("", true)) {
                        fs::create_directories(dir);
                        cout << "Директория создана.\n";
                    } else {
                        throw runtime_error("Сохранение отменено.");
                    }
                }
            }
            
            ofstream file(save_path, ios::binary);
            if (!file) {
                throw runtime_error("Не удалось открыть файл для записи.");
            }
            file.write(reinterpret_cast<const char*>(output_data.data()), output_data.size());
            file.close();
            
            cout << "Результат сохранён в: " << save_path << "\n";
            
        } catch (const exception& e) {
            cerr << "Ошибка сохранения: " << e.what() << "\n";
        }
    }
    
    secure_clear_memory(key.data(), key.size());
}

void UserInterface::process_file(bool is_encrypt) {
    string algo = safe_input_string("Алгоритм (affine/playfair): ");
    
    LibraryLoader loader(algo);
    const AlgorithmInfo* info = loader.get_info();
    
    string input_path = safe_input_path("Входной файл");
    
    if (!fs::exists(input_path)) {
        throw runtime_error("Входной файл не существует: " + input_path);
    }
    
    vector<uint8_t> key = input_key(info->key_size);
    
    ConstBuffer k_buf{key.data(), key.size()};
    if (loader.validate_key(k_buf) != 0) {
        secure_clear_memory(key.data(), key.size());
        throw runtime_error("Ключ не подходит для данного алгоритма.");
    }
    
    string default_output = get_auto_output_path(input_path, is_encrypt);
    string output_path = safe_input_path("Выходной файл");
    if (output_path.empty()) {
        output_path = default_output;
    }
    
    vector<uint8_t> input_data = FileProcessor::read_file(input_path);
    
    size_t out_size = loader.get_output_size(input_data.size(), is_encrypt);
    vector<uint8_t> output_data(out_size);
    
    ConstBuffer in_buf{input_data.data(), input_data.size()};
    MutBuffer out_buf{output_data.data(), out_size};
    
    int status = loader.execute_crypto(is_encrypt, k_buf, in_buf, &out_buf);
    if (status != 0) {
        secure_clear_memory(key.data(), key.size());
        secure_clear_memory(input_data.data(), input_data.size());
        throw runtime_error("Ошибка операции. Код: " + to_string(status));
    }
    
    output_data.resize(out_buf.size);
    
    try {
        fs::path p(output_path);
        if (p.has_parent_path() && !p.parent_path().empty()) {
            fs::path dir = p.parent_path();
            if (!fs::exists(dir)) {
                cout << "Директория \"" << dir.string() << "\" не существует. Создать? ";
                if (safe_input_yes_no("", true)) {
                    fs::create_directories(dir);
                    cout << "Директория создана.\n";
                } else {
                    throw runtime_error("Создание директории отменено.");
                }
            }
        }
        
        FileProcessor::write_file(output_path, output_data);
        cout << "Готово. Результат: " << output_path << "\n";
        
    } catch (const exception& e) {
        secure_clear_memory(key.data(), key.size());
        secure_clear_memory(input_data.data(), input_data.size());
        throw;
    }
    
    secure_clear_memory(key.data(), key.size());
    secure_clear_memory(input_data.data(), input_data.size());
}

void UserInterface::generate_and_save_key() {
    string algo = safe_input_string("Алгоритм (affine/playfair): ");
    
    LibraryLoader loader(algo);
    const AlgorithmInfo* info = loader.get_info();
    
    string default_name = "key_" + string(info->algorithm_name) + ".bin";
    string save_path = safe_input_path("Сохранить ключ в");
    if (save_path.empty()) {
        save_path = default_name;
    }
    
    vector<uint8_t> key;
    int attempts = 0;
    int status = -1;
    
    do {
        key = KeyGenerator::generate_secure_key(info->key_size);
        ConstBuffer k_buf{key.data(), key.size()};
        status = loader.validate_key(k_buf);
        attempts++;
    } while (status != 0 && attempts < 1000);
    
    if (status != 0) {
        secure_clear_memory(key.data(), key.size());
        throw runtime_error("Не удалось сгенерировать корректный ключ.");
    }
    
    try {
        fs::path p(save_path);
        if (p.has_parent_path() && !p.parent_path().empty()) {
            fs::path dir = p.parent_path();
            if (!fs::exists(dir)) {
                cout << "Директория \"" << dir.string() << "\" не существует. Создать? ";
                if (safe_input_yes_no("", true)) {
                    fs::create_directories(dir);
                    cout << "Директория создана.\n";
                } else {
                    throw runtime_error("Создание директории отменено.");
                }
            }
        }
        
        FileProcessor::write_file(save_path, key);
        
        cout << "Ключ сгенерирован: " << save_path << "\n";
        cout << "Размер: " << key.size() << " байт\n";
        
    } catch (const exception& e) {
        secure_clear_memory(key.data(), key.size());
        throw;
    }
    
    secure_clear_memory(key.data(), key.size());
}
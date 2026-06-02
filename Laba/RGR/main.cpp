#include "crypto_interface.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <random>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #define LIB_HANDLE HMODULE
    #define LOAD_LIB(name) LoadLibraryA(name)
    #define GET_FUNC(handle, name) GetProcAddress((HMODULE)handle, name)
    #define CLOSE_LIB(handle) FreeLibrary((HMODULE)handle)
    #define LIB_PREFIX ""
    #define LIB_EXT ".dll"
#else
    #include <dlfcn.h>
    #define LIB_HANDLE void*
    #define LOAD_LIB(name) dlopen(name, RTLD_NOW)
    #define GET_FUNC(handle, name) dlsym(handle, name)
    #define CLOSE_LIB(handle) dlclose(handle)
    #define LIB_PREFIX "lib"
    #define LIB_EXT ".so"
#endif

typedef const AlgorithmInfo* (*GetAlgoInfoFunc)();
typedef size_t (*GetOutputSizeFunc)(size_t, int);
typedef int (*CryptoFunc)(ConstBuffer, ConstBuffer, MutBuffer*);

bool load_algorithm(const char* algo_name, const char* mode_name, 
                    LIB_HANDLE* out_handle, GetAlgoInfoFunc* out_info_fn,
                    GetOutputSizeFunc* out_size_fn, CryptoFunc* out_crypto_fn) {
    size_t needed = std::snprintf(nullptr, 0, "./%s%s%s", LIB_PREFIX, algo_name, LIB_EXT) + 1;
    char* lib_path = (char*)std::malloc(needed);
    if (!lib_path) return false;
    
    std::snprintf(lib_path, needed, "./%s%s%s", LIB_PREFIX, algo_name, LIB_EXT);

    *out_handle = LOAD_LIB(lib_path);
    std::free(lib_path);

    if (!*out_handle) {
        std::cerr << "Error: Cannot load library.\n";
        return false;
    }

    *out_info_fn = (GetAlgoInfoFunc)GET_FUNC(*out_handle, "get_algorithm_info");
    *out_size_fn = (GetOutputSizeFunc)GET_FUNC(*out_handle, "get_output_size");
    
    const char* func_name = (std::strcmp(mode_name, "encrypt") == 0) ? "encrypt" : "decrypt";
    *out_crypto_fn = (CryptoFunc)GET_FUNC(*out_handle, func_name);

    if (!*out_info_fn) { 
        std::cerr << "Error: Missing 'get_algorithm_info'\n"; 
        CLOSE_LIB(*out_handle);
        return false; 
    }
    if (!*out_size_fn) { 
        std::cerr << "Error: Missing 'get_output_size'\n"; 
        CLOSE_LIB(*out_handle);
        return false; 
    }
    if (!*out_crypto_fn) { 
        std::cerr << "Error: Missing '" << func_name << "'\n"; 
        CLOSE_LIB(*out_handle);
        return false; 
    }

    return true;
}

// Генерация криптографически стойкого ключа
std::vector<uint8_t> generate_secure_key(size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist(0, 255);
    
    std::vector<uint8_t> key(length);
    for (size_t i = 0; i < length; ++i) {
        key[i] = static_cast<uint8_t>(dist(gen));
    }
    return key;
}

void print_help() {
    std::cout << "Usage: cryptum [OPTIONS]\n"
              << "Options:\n"
              << "  -a, --algorithm <name>  Algorithm (affine, playfair)\n"
              << "  -m, --mode <mode>       Mode (encrypt, decrypt, generate-key)\n"
              << "  -k, --key <file>        Key file path\n"
              << "  -i, --input <file>      Input file path\n"
              << "  -o, --output <file>     Output file path\n"
              << "      --save-key <file>   Save generated key to file\n"
              << "  -h, --help              Show help\n";
}

int main(int argc, char* argv[]) {
    const char* algo = nullptr;
    const char* mode = nullptr;
    const char* key_path = nullptr;
    const char* in_path = nullptr;
    const char* out_path = nullptr;
    const char* save_key_path = nullptr;

    for (int i = 1; i < argc; ++i) {
        if ((std::strcmp(argv[i], "-a") == 0 || std::strcmp(argv[i], "--algorithm") == 0) && i + 1 < argc) algo = argv[++i];
        else if ((std::strcmp(argv[i], "-m") == 0 || std::strcmp(argv[i], "--mode") == 0) && i + 1 < argc) mode = argv[++i];
        else if ((std::strcmp(argv[i], "-k") == 0 || std::strcmp(argv[i], "--key") == 0) && i + 1 < argc) key_path = argv[++i];
        else if ((std::strcmp(argv[i], "-i") == 0 || std::strcmp(argv[i], "--input") == 0) && i + 1 < argc) in_path = argv[++i];
        else if ((std::strcmp(argv[i], "-o") == 0 || std::strcmp(argv[i], "--output") == 0) && i + 1 < argc) out_path = argv[++i];
        else if (std::strcmp(argv[i], "--save-key") == 0 && i + 1 < argc) save_key_path = argv[++i];
        else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) { print_help(); return 0; }
    }

    if (!algo || !mode) { print_help(); return 1; }

    // Режим генерации ключа
    if (std::strcmp(mode, "generate-key") == 0) {
        LIB_HANDLE lib_handle = nullptr;
        GetAlgoInfoFunc get_info = nullptr;
        GetOutputSizeFunc get_size = nullptr;
        CryptoFunc crypto_op = nullptr;

        if (!load_algorithm(algo, "encrypt", &lib_handle, &get_info, &get_size, &crypto_op)) {
            return 1;
        }

        const AlgorithmInfo* info = get_info();
        std::vector<uint8_t> key = generate_secure_key(info->key_size);

        // Сохранение ключа
        if (save_key_path) {
            std::ofstream key_file(save_key_path, std::ios::binary);
            if (!key_file) {
                std::cerr << "Error: Cannot create key file.\n";
                secure_clear_memory(key.data(), key.size());
                CLOSE_LIB(lib_handle);
                return 1;
            }
            key_file.write(reinterpret_cast<const char*>(key.data()), key.size());
            key_file.close();
            std::cout << "Key generated and saved to " << save_key_path << "\n";
            std::cout << "Key size: " << key.size() << " bytes.\n";
        } else if (out_path) {
            std::ofstream key_file(out_path, std::ios::binary);
            if (!key_file) {
                std::cerr << "Error: Cannot create output file.\n";
                secure_clear_memory(key.data(), key.size());
                CLOSE_LIB(lib_handle);
                return 1;
            }
            key_file.write(reinterpret_cast<const char*>(key.data()), key.size());
            key_file.close();
        } else {
            // Вывод в stdout в hex
            std::cout << "Generated key (" << key.size() << " bytes):\n";
            for (size_t i = 0; i < key.size(); ++i) {
                printf("%02X ", key[i]);
            }
            std::cout << "\n";
        }

        secure_clear_memory(key.data(), key.size());
        CLOSE_LIB(lib_handle);
        return 0;
    }

    // Режим шифрования/расшифрования
    if (std::strcmp(mode, "encrypt") != 0 && std::strcmp(mode, "decrypt") != 0) {
        std::cerr << "Error: Invalid mode. Use 'encrypt', 'decrypt', or 'generate-key'.\n";
        return 1;
    }

    if (!key_path || !in_path || !out_path) {
        std::cerr << "Error: Missing required arguments for crypto mode.\n";
        std::cerr << "Required: -k/--key, -i/--input, -o/--output\n";
        return 1;
    }

    LIB_HANDLE lib_handle = nullptr;
    GetAlgoInfoFunc get_info = nullptr;
    GetOutputSizeFunc get_size = nullptr;
    CryptoFunc crypto_op = nullptr;

    if (!load_algorithm(algo, mode, &lib_handle, &get_info, &get_size, &crypto_op)) {
        return 1;
    }

    // Чтение ключа
    std::ifstream key_file(key_path, std::ios::binary);
    if (!key_file) {
        std::cerr << "Error: Cannot open key file.\n";
        CLOSE_LIB(lib_handle);
        return 1;
    }
    key_file.seekg(0, std::ios::end);
    size_t key_size = key_file.tellg();
    key_file.seekg(0, std::ios::beg);
    std::vector<uint8_t> key(key_size);
    key_file.read(reinterpret_cast<char*>(key.data()), key_size);
    key_file.close();

    // Чтение входных данных
    std::ifstream input_file(in_path, std::ios::binary);
    if (!input_file) {
        std::cerr << "Error: Cannot open input file.\n";
        secure_clear_memory(key.data(), key.size());
        CLOSE_LIB(lib_handle);
        return 1;
    }
    input_file.seekg(0, std::ios::end);
    size_t input_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);
    std::vector<uint8_t> input_data(input_size);
    input_file.read(reinterpret_cast<char*>(input_data.data()), input_size);
    input_file.close();

    // Выделение буфера для вывода
    int op_type = (std::strcmp(mode, "encrypt") == 0) ? 1 : 2;
    size_t output_size = get_size(input_size, op_type);
    std::vector<uint8_t> output_data(output_size);

    ConstBuffer key_buf{ key.data(), key.size() };
    ConstBuffer in_buf{ input_data.data(), input_size };
    MutBuffer out_buf{ output_data.data(), output_size };

    int status = crypto_op(key_buf, in_buf, &out_buf);
    
    if (status == 0) {
        // Запись результата
        std::ofstream output_file(out_path, std::ios::binary);
        if (!output_file) {
            std::cerr << "Error: Cannot create output file.\n";
            secure_clear_memory(key.data(), key.size());
            secure_clear_memory(input_data.data(), input_size);
            secure_clear_memory(output_data.data(), output_size);
            CLOSE_LIB(lib_handle);
            return 1;
        }
        output_file.write(reinterpret_cast<const char*>(output_data.data()), out_buf.size);
        output_file.close();
        
        std::cout << "Success. Output size: " << out_buf.size << " bytes.\n";
    } else {
        std::cerr << "Crypto error code: " << status << "\n";
    }

    secure_clear_memory(key.data(), key.size());
    secure_clear_memory(input_data.data(), input_size);
    secure_clear_memory(output_data.data(), output_size);
    CLOSE_LIB(lib_handle);
    return status;
}
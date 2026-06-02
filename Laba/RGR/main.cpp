#include "crypto_interface.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>

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
                    LIB_HANDLE* out_handle, GetOutputSizeFunc* out_size_fn, CryptoFunc* out_crypto_fn) {
    size_t needed = std::snprintf(nullptr, 0, "%s%s%s", LIB_PREFIX, algo_name, LIB_EXT) + 1;
    char* lib_path = (char*)std::malloc(needed);
    if (!lib_path) return false;
    
    std::snprintf(lib_path, needed, "%s%s%s", LIB_PREFIX, algo_name, LIB_EXT);

    *out_handle = LOAD_LIB(lib_path);
    std::free(lib_path);

    if (!*out_handle) {
        std::cerr << "Error: Cannot load library.\n";
        return false;
    }

    auto get_info = (GetAlgoInfoFunc)GET_FUNC(*out_handle, "get_algorithm_info");
    *out_size_fn = (GetOutputSizeFunc)GET_FUNC(*out_handle, "get_output_size");
    
    const char* func_name = (std::strcmp(mode_name, "encrypt") == 0) ? "encrypt" : "decrypt";
    *out_crypto_fn = (CryptoFunc)GET_FUNC(*out_handle, func_name);

    if (!get_info || !*out_size_fn || !*out_crypto_fn) {
        std::cerr << "Error: Invalid library interface.\n";
        CLOSE_LIB(*out_handle);
        return false;
    }

    return true;
}

void print_help() {
    std::cout << "Usage: cryptum [OPTIONS]\n"
              << "Options:\n"
              << "  -a, --algorithm <name>  Algorithm (affine, playfair)\n"
              << "  -m, --mode <mode>       Mode (encrypt, decrypt)\n"
              << "  -k, --key <file>        Key file path\n"
              << "  -i, --input <file>      Input file path\n"
              << "  -o, --output <file>     Output file path\n"
              << "  -h, --help              Show help\n";
}

int main(int argc, char* argv[]) {
    const char* algo = nullptr;
    const char* mode = nullptr;

    for (int i = 1; i < argc; ++i) {
        if ((std::strcmp(argv[i], "-a") == 0 || std::strcmp(argv[i], "--algorithm") == 0) && i + 1 < argc) algo = argv[++i];
        else if ((std::strcmp(argv[i], "-m") == 0 || std::strcmp(argv[i], "--mode") == 0) && i + 1 < argc) mode = argv[++i];
        else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) { print_help(); return 0; }
    }

    if (!algo || !mode) { print_help(); return 1; }

    LIB_HANDLE lib_handle = nullptr;
    GetOutputSizeFunc get_size = nullptr;
    CryptoFunc crypto_op = nullptr;

    if (!load_algorithm(algo, mode, &lib_handle, &get_size, &crypto_op)) {
        return 1;
    }

    // Демонстрация работы
    uint8_t dummy_key[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    uint8_t input_data[6] = {'H', 'e', 'l', 'l', 'o', '!'};
    size_t in_size = 6;
    
    int op_type = (std::strcmp(mode, "encrypt") == 0) ? 1 : 2;
    size_t out_size = get_size(in_size, op_type);
    
    uint8_t* output_data = (uint8_t*)std::malloc(out_size);
    if (!output_data) {
        CLOSE_LIB(lib_handle);
        return 1;
    }

    ConstBuffer key_buf{ dummy_key, 16 };
    ConstBuffer in_buf{ input_data, in_size };
    MutBuffer out_buf{ output_data, out_size };

    int status = crypto_op(key_buf, in_buf, &out_buf);
    
    if (status == 0) {
        std::cout << "Success. Output size: " << out_buf.size << " bytes.\n";
        for (size_t i = 0; i < out_buf.size; ++i) {
            printf("%02X ", output_data[i]);
        }
        std::cout << "\n";
    } else {
        std::cerr << "Crypto error code: " << status << "\n";
    }

    secure_clear_memory(dummy_key, sizeof(dummy_key));
    secure_clear_memory(input_data, in_size);
    secure_clear_memory(output_data, out_size);
    std::free(output_data);

    CLOSE_LIB(lib_handle);
    return status;
}
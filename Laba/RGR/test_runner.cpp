#include "crypto_interface.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

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

bool run_test(const char* lib_name, const std::vector<uint8_t>& plain, const std::vector<uint8_t>& key) {
    std::string filename = "./" + std::string(LIB_PREFIX) + lib_name + LIB_EXT;
    LIB_HANDLE handle = LOAD_LIB(filename.c_str());
    if (!handle) {
        std::cerr << "[FAIL] Could not load " << filename << "\n";
        return false;
    }

    auto get_size = (GetOutputSizeFunc)GET_FUNC(handle, "get_output_size");
    auto enc = (CryptoFunc)GET_FUNC(handle, "encrypt");
    auto dec = (CryptoFunc)GET_FUNC(handle, "decrypt");

    if (!get_size || !enc || !dec) {
        std::cerr << "[FAIL] Invalid interface in " << lib_name << "\n";
        CLOSE_LIB(handle);
        return false;
    }

    // 1. Шифрование
    size_t enc_size = get_size(plain.size(), 1);
    std::vector<uint8_t> enc_buf(enc_size);
    ConstBuffer k{key.data(), key.size()};
    ConstBuffer p{plain.data(), plain.size()};
    MutBuffer e{enc_buf.data(), enc_size};

    if (enc(k, p, &e) != 0) {
        std::cerr << "[FAIL] Encryption failed in " << lib_name << "\n";
        CLOSE_LIB(handle);
        return false;
    }

    // 2. Расшифрование
    size_t dec_size = get_size(e.size, 2);
    std::vector<uint8_t> dec_buf(dec_size);
    MutBuffer d{dec_buf.data(), dec_size};
    ConstBuffer c{enc_buf.data(), e.size};

    if (dec(k, c, &d) != 0) {
        std::cerr << "[FAIL] Decryption failed in " << lib_name << "\n";
        CLOSE_LIB(handle);
        return false;
    }

    // 3. Сравнение
    bool success = (d.size == plain.size() && std::memcmp(plain.data(), dec_buf.data(), plain.size()) == 0);
    
    if (success) {
        std::cout << "[PASS] " << lib_name << " test passed.\n";
    } else {
        std::cerr << "[FAIL] Data mismatch in " << lib_name << "\n";
    }

    secure_clear_memory(enc_buf.data(), enc_buf.size());
    secure_clear_memory(dec_buf.data(), dec_buf.size());
    CLOSE_LIB(handle);
    return success;
}

int main() {
    std::cout << "Starting cryptographic tests...\n";
    
    // Тест 1: Affine
    std::vector<uint8_t> key_affine = {0x05, 0x10}; // a=5 (нечетное), b=16
    std::vector<uint8_t> data_affine = {'T', 'E', 'S', 'T', 0x00, 0xFF};
    bool t1 = run_test("affine", data_affine, key_affine);

    // Тест 2: Playfair (четное количество байт)
    std::vector<uint8_t> key_playfair(16, 0xAA);
    std::vector<uint8_t> data_playfair = {'H', 'e', 'l', 'l', 'o', '!'};
    bool t2 = run_test("playfair", data_playfair, key_playfair);

    // Тест 3: Playfair (нечетное количество байт - проверка PKCS#7)
    std::vector<uint8_t> data_playfair_odd = {'A', 'B', 'C'};
    bool t3 = run_test("playfair", data_playfair_odd, key_playfair);

    if (t1 && t2 && t3) {
        std::cout << "All tests PASSED.\n";
        return 0;
    }
    return 1;
}
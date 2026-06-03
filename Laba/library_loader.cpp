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

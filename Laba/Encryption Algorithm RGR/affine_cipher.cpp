#include "crypto_interface.h"

using namespace std;

static uint8_t mod_inverse_256(uint8_t a) {
    for (int i = 1; i < 256; i += 2) {
        if ((static_cast<uint16_t>(a) * i) % 256 == 1) {
            return static_cast<uint8_t>(i);
        }
    }
    return 1; 
}

extern "C" {

const AlgorithmInfo* get_algorithm_info() {
    static const AlgorithmInfo info = { "affine", 2 };
    return &info;
}

size_t get_output_size(size_t input_size, int operation_type) {
    (void)operation_type;
    return input_size;
}

int validate_key(ConstBuffer key) {
    if (key.size < 2) return 1;
    if (key.data[0] % 2 == 0) return 2;
    return 0;
}

int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (!output || !output->data || output->size < input.size) return 1;
    if (validate_key(key) != 0) return 3;

    uint8_t a = key.data[0];
    uint8_t b = key.data[1];

    for (size_t i = 0; i < input.size; ++i) {
        output->data[i] = static_cast<uint8_t>((static_cast<uint16_t>(input.data[i]) * a + b) % 256);
    }
    return 0;
}

int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (!output || !output->data || output->size < input.size) return 1;
    if (validate_key(key) != 0) return 3;

    uint8_t a = key.data[0];
    uint8_t b = key.data[1];
    uint8_t a_inv = mod_inverse_256(a);
    
    for (size_t i = 0; i < input.size; ++i) {
        int16_t val = static_cast<int16_t>(input.data[i]) - b;
        if (val < 0) val += 256;
        output->data[i] = static_cast<uint8_t>((static_cast<uint16_t>(val) * a_inv) % 256);
    }
    return 0;
}

}
#include "playfair_cipher.h"
#include <vector>
#include <cstring>

using namespace std;

// ==========================================
// Реализация методов класса PlayfairCipher
// ==========================================

void PlayfairCipher::build_table(ConstBuffer key, uint8_t table[16][16]) {
    bool used[256] = {false};
    uint8_t flat[256];
    size_t pos = 0;

    // Сначала добавляем уникальные байты из ключа
    for (size_t i = 0; i < key.size; ++i) {
        if (!used[key.data[i]]) {
            used[key.data[i]] = true;
            flat[pos++] = key.data[i];
        }
    }
    
    // Затем добавляем остальные байты по возрастанию
    for (int i = 0; i < 256; ++i) {
        if (!used[i]) {
            flat[pos++] = static_cast<uint8_t>(i);
        }
    }

    // Заполняем таблицу 16x16
    for (int r = 0; r < 16; ++r) {
        for (int c = 0; c < 16; ++c) {
            table[r][c] = flat[r * 16 + c];
        }
    }
}

void PlayfairCipher::find_pos(const uint8_t table[16][16], uint8_t val, int& row, int& col) {
    for (int r = 0; r < 16; ++r) {
        for (int c = 0; c < 16; ++c) {
            if (table[r][c] == val) {
                row = r;
                col = c;
                return;
            }
        }
    }
}

const AlgorithmInfo* PlayfairCipher::get_info() {
    static const AlgorithmInfo info = { "playfair", 16 };
    return &info;
}

size_t PlayfairCipher::get_output_size(size_t input_size, bool is_encrypt) {
    if (is_encrypt) {
        // PKCS#7 padding для блока размером 2 байта
        size_t remainder = input_size % 2;
        if (remainder == 0) return input_size + 2; // Если кратно, добавляем полный блок padding
        return input_size + 1;                     // Иначе добавляем 1 байт
    }
    return input_size;
}

int PlayfairCipher::validate_key(ConstBuffer key) {
    if (key.size < 16) return 1; // Требуется минимум 16 байт для инициализации таблицы
    return 0;
}

int PlayfairCipher::encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (!output || !output->data) return 1;
    if (validate_key(key) != 0) return 3;

    size_t pad_len = (input.size % 2 == 0) ? 2 : 1;
    size_t padded_size = input.size + pad_len;

    if (output->size < padded_size) return 1;

    uint8_t table[16][16];
    build_table(key, table);

    // Создаем буфер с padding
    vector<uint8_t> padded_input(padded_size, static_cast<uint8_t>(pad_len));
    memcpy(padded_input.data(), input.data, input.size);

    // Шифрование парами
    for (size_t i = 0; i < padded_size; i += 2) {
        int r1, c1, r2, c2;
        find_pos(table, padded_input[i], r1, c1);
        find_pos(table, padded_input[i + 1], r2, c2);

        if (r1 == r2) {
            output->data[i]     = table[r1][(c1 + 1) % 16];
            output->data[i + 1] = table[r2][(c2 + 1) % 16];
        } else if (c1 == c2) {
            output->data[i]     = table[(r1 + 1) % 16][c1];
            output->data[i + 1] = table[(r2 + 1) % 16][c2];
        } else {
            output->data[i]     = table[r1][c2];
            output->data[i + 1] = table[r2][c1];
        }
    }

    output->size = padded_size;
    return 0;
}

int PlayfairCipher::decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    if (!output || !output->data || input.size % 2 != 0 || output->size < input.size) return 1;
    if (validate_key(key) != 0) return 3;

    // Обработка пустого ввода
    if (input.size == 0) {
        output->size = 0;
        return 0;
    }

    uint8_t table[16][16];
    build_table(key, table);

    // Расшифрование парами
    for (size_t i = 0; i < input.size; i += 2) {
        int r1, c1, r2, c2;
        find_pos(table, input.data[i], r1, c1);
        find_pos(table, input.data[i + 1], r2, c2);


        if (r1 == r2) {
            output->data[i]     = table[r1][(c1 + 15) % 16];
            output->data[i + 1] = table[r2][(c2 + 15) % 16];
        } else if (c1 == c2) {
            output->data[i]     = table[(r1 + 15) % 16][c1];
            output->data[i + 1] = table[(r2 + 15) % 16][c2];
        } else {
            output->data[i]     = table[r1][c2];
            output->data[i + 1] = table[r2][c1];
        }
    }

    // Снятие PKCS#7 padding
    uint8_t pad_val = output->data[input.size - 1];
    if (pad_val > 2 || pad_val == 0) {
        output->size = input.size; // Неверный padding, возвращаем как есть
    } else {
        bool valid_pad = true;
        for (uint8_t i = 1; i <= pad_val; ++i) {
            if (output->data[input.size - i] != pad_val) {
                valid_pad = false;
                break;
            }
        }
        output->size = valid_pad ? (input.size - pad_val) : input.size;
    }

    return 0;
}

// ==========================================
// Реализация C-интерфейса для загрузчика
// ==========================================

extern "C" {

const AlgorithmInfo* get_algorithm_info() {
    return PlayfairCipher::get_info();
}

size_t get_output_size(size_t input_size, int operation_type) {
    return PlayfairCipher::get_output_size(input_size, operation_type == 1);
}

int validate_key(ConstBuffer key) {
    return PlayfairCipher::validate_key(key);
}

int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    return PlayfairCipher::encrypt(key, input, output);
}

int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    return PlayfairCipher::decrypt(key, input, output);
}

}

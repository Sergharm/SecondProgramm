#ifndef PLAYFAIR_CIPHER_H
#define PLAYFAIR_CIPHER_H

#include "crypto_interface.h"
#include <cstdint>

// C++ класс, инкапсулирующий логику шифра Плейфера
class PlayfairCipher {
public:
    // Получение информации об алгоритме
    static const AlgorithmInfo* get_info();
    
    // Расчет размера выходных данных
    static size_t get_output_size(size_t input_size, bool is_encrypt);
    
    // Валидация ключа
    static int validate_key(ConstBuffer key);
    
    // Шифрование
    static int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output);
    
    // Расшифрование
    static int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output);

private:
    // Построение таблицы подстановки 16x16
    static void build_table(ConstBuffer key, uint8_t table[16][16]);
    
    // Поиск позиции байта в таблице
    static void find_pos(const uint8_t table[16][16], uint8_t val, int& row, int& col);
};

// Унифицированный C-интерфейс для динамического загрузчика
extern "C" {
    const AlgorithmInfo* get_algorithm_info();
    size_t get_output_size(size_t input_size, int operation_type);
    int validate_key(ConstBuffer key);
    int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output);
    int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output);
}

#endif // PLAYFAIR_CIPHER_H

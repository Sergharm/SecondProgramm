#ifndef AES_H
#define AES_H

#include <vector>
#include <string>
#include <cstdint>

using namespace std;

// Константы для AES-128
const unsigned int AES_BLOCK_SIZE = 16;  // 128 бит = 16 байт
const unsigned int AES_KEY_SIZE = 16;    // 128 бит = 16 байт
const unsigned int AES_NUM_ROUNDS = 10;  // Количество раундов для AES-128

// Таблицы подстановок
extern const uint8_t S_BOX[256];
extern const uint8_t INV_S_BOX[256];
extern const uint8_t RCON[10];

// Генерация случайных байтов
vector<uint8_t> generateRandomBytes(unsigned int size);

// Сохранение и загрузка ключа
void saveKeyToFile(const vector<uint8_t>& key, const string& filename);
vector<uint8_t> loadKeyFromFile(const string& filename);

// PKCS7 Padding
vector<uint8_t> applyPKCS7Padding(const vector<uint8_t>& data, unsigned int blockSize);
vector<uint8_t> removePKCS7Padding(const vector<uint8_t>& data);

// Преобразование типов
vector<uint8_t> stringToBytes(const string& str);
string bytesToString(const vector<uint8_t>& bytes);

// Расширение ключа
vector<uint8_t> expandKey(const vector<uint8_t>& key);

// Основные операции AES
void subBytes(vector<vector<uint8_t>>& state);
void invSubBytes(vector<vector<uint8_t>>& state);
void shiftRows(vector<vector<uint8_t>>& state);
void invShiftRows(vector<vector<uint8_t>>& state);
void mixColumns(vector<vector<uint8_t>>& state);
void invMixColumns(vector<vector<uint8_t>>& state);
void addRoundKey(vector<vector<uint8_t>>& state, const vector<uint8_t>& roundKey);

// Вспомогательные функции
vector<vector<uint8_t>> bytesToState(const vector<uint8_t>& block);
vector<uint8_t> stateToBytes(const vector<vector<uint8_t>>& state);
vector<uint8_t> getRoundKey(const vector<uint8_t>& expandedKey, unsigned int round);

// Шифрование и расшифрование блока
vector<uint8_t> encryptBlock(const vector<uint8_t>& block, const vector<uint8_t>& expandedKey);
vector<uint8_t> decryptBlock(const vector<uint8_t>& block, const vector<uint8_t>& expandedKey);

// Режим CBC (Cipher Block Chaining)
vector<uint8_t> encryptCBC(const vector<uint8_t>& plaintext, const vector<uint8_t>& key, const vector<uint8_t>& iv);
vector<uint8_t> decryptCBC(const vector<uint8_t>& ciphertext, const vector<uint8_t>& key, const vector<uint8_t>& iv);

// Вывод
void printState(const vector<vector<uint8_t>>& state, const string& title);
void printRoundKey(const vector<uint8_t>& roundKey, unsigned int roundNum);
void printBytes(const vector<uint8_t>& bytes, const string& title);

#endif
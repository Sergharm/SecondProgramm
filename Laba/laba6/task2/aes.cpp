#include "aes.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <algorithm>

using namespace std;

// Таблица S-Box
const uint8_t S_BOX[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

// Обратная S-Box
const uint8_t INV_S_BOX[256] = {
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

// Константы для расширения ключа
const uint8_t RCON[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};

// Генерация случайных байтов
vector<uint8_t> generateRandomBytes(unsigned int size) {
    vector<uint8_t> bytes(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<unsigned int> dis(0, 255);
    for (auto& byte : bytes)
        byte = static_cast<uint8_t>(dis(gen));
    return bytes;
}

// Сохранение ключа в файл
void saveKeyToFile(const vector<uint8_t>& key, const string& filename) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        for (uint8_t byte : key)
            file << static_cast<char>(byte);
        file.close();
    }
}

// PKCS7 Padding
vector<uint8_t> applyPKCS7Padding(const vector<uint8_t>& data, unsigned int blockSize) {
    vector<uint8_t> padded = data;
    unsigned int padLen = blockSize - (data.size() % blockSize);
    for (unsigned int i = 0; i < padLen; ++i)
        padded.push_back(static_cast<uint8_t>(padLen));
    return padded;
}

// Удаление PKCS7 Padding
vector<uint8_t> removePKCS7Padding(const vector<uint8_t>& data) {
    if (data.empty()) return data;
    uint8_t padLen = data.back();
    if (padLen == 0 || padLen > AES_BLOCK_SIZE) return data;
    return vector<uint8_t>(data.begin(), data.end() - padLen);
}

// Преобразование строки в байты
vector<uint8_t> stringToBytes(const string& str) {
    return vector<uint8_t>(str.begin(), str.end());
}

// Преобразование байтов в строку
string bytesToString(const vector<uint8_t>& bytes) {
    return string(bytes.begin(), bytes.end());
}

// Расширение ключа
vector<uint8_t> expandKey(const vector<uint8_t>& key) {
    const unsigned int nk = 4;
    vector<uint8_t> exp(nk * (AES_NUM_ROUNDS + 1) * 4);
    for (unsigned int i = 0; i < nk * 4; ++i)
        exp[i] = key[i];
    for (unsigned int i = nk; i < nk * (AES_NUM_ROUNDS + 1); ++i) {
        uint8_t t[4];
        for (unsigned int j = 0; j < 4; ++j)
            t[j] = exp[(i - 1) * 4 + j];
        if (i % nk == 0) {
            uint8_t tmp = t[0]; t[0] = t[1]; t[1] = t[2]; t[2] = t[3]; t[3] = tmp;
            for (unsigned int j = 0; j < 4; ++j)
                t[j] = S_BOX[t[j]];
            t[0] ^= RCON[i / nk - 1];
        }
        for (unsigned int j = 0; j < 4; ++j)
            exp[i * 4 + j] = exp[(i - nk) * 4 + j] ^ t[j];
    }
    return exp;
}

// Преобразование блока в матрицу состояния
vector<vector<uint8_t>> bytesToState(const vector<uint8_t>& block) {
    vector<vector<uint8_t>> st(4, vector<uint8_t>(4));
    for (unsigned int i = 0; i < 4; ++i)
        for (unsigned int j = 0; j < 4; ++j)
            st[i][j] = block[j * 4 + i];
    return st;
}

// Преобразование матрицы состояния в блок
vector<uint8_t> stateToBytes(const vector<vector<uint8_t>>& st) {
    vector<uint8_t> block(16);
    for (unsigned int i = 0; i < 4; ++i)
        for (unsigned int j = 0; j < 4; ++j)
            block[j * 4 + i] = st[i][j];
    return block;
}

// Получение раундового ключа
vector<uint8_t> getRoundKey(const vector<uint8_t>& expKey, unsigned int round) {
    return vector<uint8_t>(expKey.begin() + round * 16, expKey.begin() + (round + 1) * 16);
}

// SubBytes
void subBytes(vector<vector<uint8_t>>& st) {
    for (unsigned int i = 0; i < 4; ++i)
        for (unsigned int j = 0; j < 4; ++j)
            st[i][j] = S_BOX[st[i][j]];
}

// InvSubBytes
void invSubBytes(vector<vector<uint8_t>>& st) {
    for (unsigned int i = 0; i < 4; ++i)
        for (unsigned int j = 0; j < 4; ++j)
            st[i][j] = INV_S_BOX[st[i][j]];
}

// ShiftRows
void shiftRows(vector<vector<uint8_t>>& st) {
    uint8_t t = st[1][0]; st[1][0] = st[1][1]; st[1][1] = st[1][2]; st[1][2] = st[1][3]; st[1][3] = t;
    t = st[2][0]; st[2][0] = st[2][2]; st[2][2] = t; t = st[2][1]; st[2][1] = st[2][3]; st[2][3] = t;
    t = st[3][3]; st[3][3] = st[3][2]; st[3][2] = st[3][1]; st[3][1] = st[3][0]; st[3][0] = t;
}

// InvShiftRows
void invShiftRows(vector<vector<uint8_t>>& st) {
    uint8_t t = st[1][3]; st[1][3] = st[1][2]; st[1][2] = st[1][1]; st[1][1] = st[1][0]; st[1][0] = t;
    t = st[2][0]; st[2][0] = st[2][2]; st[2][2] = t; t = st[2][1]; st[2][1] = st[2][3]; st[2][3] = t;
    t = st[3][0]; st[3][0] = st[3][1]; st[3][1] = st[3][2]; st[3][2] = st[3][3]; st[3][3] = t;
}

// Умножение в поле Галуа
uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (unsigned int i = 0; i < 8; ++i) {
        if (b & 1) p ^= a;
        bool hi = (a & 0x80) != 0;
        a <<= 1;
        if (hi) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

// MixColumns
void mixColumns(vector<vector<uint8_t>>& st) {
    for (unsigned int j = 0; j < 4; ++j) {
        uint8_t a0 = st[0][j], a1 = st[1][j], a2 = st[2][j], a3 = st[3][j];
        st[0][j] = gmul(0x02, a0) ^ gmul(0x03, a1) ^ a2 ^ a3;
        st[1][j] = a0 ^ gmul(0x02, a1) ^ gmul(0x03, a2) ^ a3;
        st[2][j] = a0 ^ a1 ^ gmul(0x02, a2) ^ gmul(0x03, a3);
        st[3][j] = gmul(0x03, a0) ^ a1 ^ a2 ^ gmul(0x02, a3);
    }
}

// InvMixColumns
void invMixColumns(vector<vector<uint8_t>>& st) {
    for (unsigned int j = 0; j < 4; ++j) {
        uint8_t a0 = st[0][j], a1 = st[1][j], a2 = st[2][j], a3 = st[3][j];
        st[0][j] = gmul(0x0e, a0) ^ gmul(0x0b, a1) ^ gmul(0x0d, a2) ^ gmul(0x09, a3);
        st[1][j] = gmul(0x09, a0) ^ gmul(0x0e, a1) ^ gmul(0x0b, a2) ^ gmul(0x0d, a3);
        st[2][j] = gmul(0x0d, a0) ^ gmul(0x09, a1) ^ gmul(0x0e, a2) ^ gmul(0x0b, a3);
        st[3][j] = gmul(0x0b, a0) ^ gmul(0x0d, a1) ^ gmul(0x09, a2) ^ gmul(0x0e, a3);
    }
}

// AddRoundKey
void addRoundKey(vector<vector<uint8_t>>& st, const vector<uint8_t>& rk) {
    for (unsigned int i = 0; i < 4; ++i)
        for (unsigned int j = 0; j < 4; ++j)
            st[i][j] ^= rk[j * 4 + i];
}

// Шифрование блока
vector<uint8_t> encryptBlock(const vector<uint8_t>& block, const vector<uint8_t>& expKey) {
    vector<vector<uint8_t>> st = bytesToState(block);
    addRoundKey(st, getRoundKey(expKey, 0));
    for (unsigned int r = 1; r < AES_NUM_ROUNDS; ++r) {
        subBytes(st);
        shiftRows(st);
        mixColumns(st);
        addRoundKey(st, getRoundKey(expKey, r));
    }
    subBytes(st);
    shiftRows(st);
    addRoundKey(st, getRoundKey(expKey, AES_NUM_ROUNDS));
    return stateToBytes(st);
}

// Расшифрование блока
vector<uint8_t> decryptBlock(const vector<uint8_t>& block, const vector<uint8_t>& expKey) {
    vector<vector<uint8_t>> st = bytesToState(block);
    addRoundKey(st, getRoundKey(expKey, AES_NUM_ROUNDS));
    for (int r = AES_NUM_ROUNDS - 1; r > 0; --r) {
        invShiftRows(st);
        invSubBytes(st);
        addRoundKey(st, getRoundKey(expKey, r));
        invMixColumns(st);
    }
    invShiftRows(st);
    invSubBytes(st);
    addRoundKey(st, getRoundKey(expKey, 0));
    return stateToBytes(st);
}

// Шифрование в режиме CBC
vector<uint8_t> encryptCBC(const vector<uint8_t>& plaintext, const vector<uint8_t>& key, const vector<uint8_t>& iv) {
    vector<uint8_t> expKey = expandKey(key);
    vector<uint8_t> ciphertext;
    vector<uint8_t> prevBlock = iv;
    for (size_t i = 0; i < plaintext.size(); i += AES_BLOCK_SIZE) {
        vector<uint8_t> xoredBlock(AES_BLOCK_SIZE);
        for (size_t j = 0; j < AES_BLOCK_SIZE; ++j)
            xoredBlock[j] = plaintext[i + j] ^ prevBlock[j];
        vector<uint8_t> encBlock = encryptBlock(xoredBlock, expKey);
        ciphertext.insert(ciphertext.end(), encBlock.begin(), encBlock.end());
        prevBlock = encBlock;
    }
    return ciphertext;
}

// Расшифрование в режиме CBC
vector<uint8_t> decryptCBC(const vector<uint8_t>& ciphertext, const vector<uint8_t>& key, const vector<uint8_t>& iv) {
    vector<uint8_t> expKey = expandKey(key);
    vector<uint8_t> plaintext;
    vector<uint8_t> prevBlock = iv;
    for (size_t i = 0; i < ciphertext.size(); i += AES_BLOCK_SIZE) {
        vector<uint8_t> curBlock(ciphertext.begin() + i, ciphertext.begin() + i + AES_BLOCK_SIZE);
        vector<uint8_t> decBlock = decryptBlock(curBlock, expKey);
        vector<uint8_t> plainBlock(AES_BLOCK_SIZE);
        for (size_t j = 0; j < AES_BLOCK_SIZE; ++j)
            plainBlock[j] = decBlock[j] ^ prevBlock[j];
        plaintext.insert(plaintext.end(), plainBlock.begin(), plainBlock.end());
        prevBlock = curBlock;
    }
    return plaintext;
}

// Вывод матрицы состояния
void printState(const vector<vector<uint8_t>>& st, const string& title) {
    cout << "\n" << title << ":\n";
    for (unsigned int i = 0; i < 4; ++i) {
        for (unsigned int j = 0; j < 4; ++j)
            cout << hex << setw(2) << setfill('0') << static_cast<int>(st[i][j]) << " ";
        cout << "\n";
    }
    cout << dec;
}

// Вывод раундового ключа
void printRoundKey(const vector<uint8_t>& rk, unsigned int num) {
    cout << "\nRound Key " << num << ":\n";
    for (unsigned int i = 0; i < 4; ++i) {
        for (unsigned int j = 0; j < 4; ++j)
            cout << hex << setw(2) << setfill('0') << static_cast<int>(rk[j * 4 + i]) << " ";
        cout << "\n";
    }
    cout << dec;
}

// Вывод байтов
void printBytes(const vector<uint8_t>& bytes, const string& title) {
    cout << title << ": ";
    for (uint8_t b : bytes)
        cout << hex << setw(2) << setfill('0') << static_cast<int>(b);
    cout << dec << "\n";
}
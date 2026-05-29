#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <random>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Константы AES-128
const int BLOCK_SIZE = 16;
const int KEY_SIZE   = 16;
const int NUM_ROUNDS = 10;

// Таблица замен S-Box
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

// Обратная таблица Inv S-Box
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

//  ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ

// Вывод вектора байт в hex виде
void printBytes(const vector<uint8_t>& bytes, string title) {
    cout << title << ": ";
    for (uint8_t b : bytes)
        cout << hex << setw(2) << setfill('0') << (int)b;
    cout << dec << endl;
}

// Вывод матрицы состояния 4x4
void printState(const vector<vector<uint8_t>>& state, string title) {
    cout << title << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            cout << hex << setw(2) << setfill('0') << (int)state[i][j] << " ";
        cout << dec << endl;
    }
    cout << endl;
}

// Вывод раундового ключа
void printRoundKey(const vector<uint8_t>& rk, int num) {
    cout << "Round Key " << num << ":" << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            cout << hex << setw(2) << setfill('0') << (int)rk[j * 4 + i] << " ";
        cout << dec << endl;
    }
    cout << endl;
}

// Генерация случайных байт
vector<uint8_t> generateRandom(int size) {
    vector<uint8_t> bytes(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 255);
    for (int i = 0; i < size; i++)
        bytes[i] = dis(gen);
    return bytes;
}

// Сохранение ключа в файл
void saveKeyToFile(const vector<uint8_t>& key, string filename) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        for (uint8_t b : key) file << (char)b;
        file.close();
        cout << "Ключ сохранен в " << filename << endl;
    }
}

// PKCS7 Padding
vector<uint8_t> applyPadding(const vector<uint8_t>& data) {
    vector<uint8_t> padded = data;
    int padLen = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
    if (padLen == 0) padLen = BLOCK_SIZE;
    for (int i = 0; i < padLen; i++)
        padded.push_back(padLen);
    return padded;
}

// Удаление PKCS7 Padding
vector<uint8_t> removePadding(const vector<uint8_t>& data) {
    if (data.empty()) return data;
    int padLen = data.back();
    if (padLen <= 0 || padLen > BLOCK_SIZE) return data;
    return vector<uint8_t>(data.begin(), data.end() - padLen);
}

// Строка -> байты
vector<uint8_t> stringToBytes(string s) {
    return vector<uint8_t>(s.begin(), s.end());
}

// Байты -> строка
string bytesToString(vector<uint8_t> b) {
    return string(b.begin(), b.end());
}

// ЯДРО AES 

// Расширение ключа
vector<uint8_t> expandKey(vector<uint8_t> key) {
    vector<uint8_t> expKey(176); // 11 раундов * 16 байт
    for (int i = 0; i < 16; i++) expKey[i] = key[i];

    for (int i = 16; i < 176; i += 4) {
        uint8_t t[4] = {expKey[i-4], expKey[i-3], expKey[i-2], expKey[i-1]};
        if (i % 16 == 0) {
            uint8_t tmp = t[0]; t[0]=t[1]; t[1]=t[2]; t[2]=t[3]; t[3]=tmp;
            t[0] = S_BOX[t[0]]; t[1] = S_BOX[t[1]]; t[2] = S_BOX[t[2]]; t[3] = S_BOX[t[3]];
            t[0] ^= RCON[i/16 - 1];
        }
        expKey[i]   = expKey[i-16] ^ t[0];
        expKey[i+1] = expKey[i-15] ^ t[1];
        expKey[i+2] = expKey[i-14] ^ t[2];
        expKey[i+3] = expKey[i-13] ^ t[3];
    }
    return expKey;
}

// Блок -> матрица состояния
vector<vector<uint8_t>> bytesToState(vector<uint8_t> block) {
    vector<vector<uint8_t>> state(4, vector<uint8_t>(4));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = block[j * 4 + i];
    return state;
}

// Матрица состояния -> блок
vector<uint8_t> stateToBytes(vector<vector<uint8_t>> state) {
    vector<uint8_t> block(16);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            block[j * 4 + i] = state[i][j];
    return block;
}

// SubBytes
void subBytes(vector<vector<uint8_t>>& state) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = S_BOX[state[i][j]];
}

// InvSubBytes
void invSubBytes(vector<vector<uint8_t>>& state) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = INV_S_BOX[state[i][j]];
}

// ShiftRows
void shiftRows(vector<vector<uint8_t>>& state) {
    uint8_t tmp;
    // строка 1: сдвиг на 1
    tmp = state[1][0]; state[1][0]=state[1][1]; state[1][1]=state[1][2]; state[1][2]=state[1][3]; state[1][3]=tmp;
    // строка 2: сдвиг на 2
    tmp = state[2][0]; state[2][0]=state[2][2]; state[2][2]=tmp;
    tmp = state[2][1]; state[2][1]=state[2][3]; state[2][3]=tmp;
    // строка 3: сдвиг на 3
    tmp = state[3][3]; state[3][3]=state[3][2]; state[3][2]=state[3][1]; state[3][1]=state[3][0]; state[3][0]=tmp;
}

// InvShiftRows
void invShiftRows(vector<vector<uint8_t>>& state) {
    uint8_t tmp;
    tmp = state[1][3]; state[1][3]=state[1][2]; state[1][2]=state[1][1]; state[1][1]=state[1][0]; state[1][0]=tmp;
    tmp = state[2][0]; state[2][0]=state[2][2]; state[2][2]=tmp;
    tmp = state[2][1]; state[2][1]=state[2][3]; state[2][3]=tmp;
    tmp = state[3][0]; state[3][0]=state[3][1]; state[3][1]=state[3][2]; state[3][2]=state[3][3]; state[3][3]=tmp;
}

// Умножение в поле Галуа GF(2^8)
uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        bool hi = (a & 0x80) != 0;
        a <<= 1;
        if (hi) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

// MixColumns
void mixColumns(vector<vector<uint8_t>>& state) {
    for (int j = 0; j < 4; j++) {
        uint8_t a0=state[0][j], a1=state[1][j], a2=state[2][j], a3=state[3][j];
        state[0][j] = gmul(0x02,a0) ^ gmul(0x03,a1) ^ a2 ^ a3;
        state[1][j] = a0 ^ gmul(0x02,a1) ^ gmul(0x03,a2) ^ a3;
        state[2][j] = a0 ^ a1 ^ gmul(0x02,a2) ^ gmul(0x03,a3);
        state[3][j] = gmul(0x03,a0) ^ a1 ^ a2 ^ gmul(0x02,a3);
    }
}

// InvMixColumns
void invMixColumns(vector<vector<uint8_t>>& state) {
    for (int j = 0; j < 4; j++) {
        uint8_t a0=state[0][j], a1=state[1][j], a2=state[2][j], a3=state[3][j];
        state[0][j] = gmul(0x0e,a0) ^ gmul(0x0b,a1) ^ gmul(0x0d,a2) ^ gmul(0x09,a3);
        state[1][j] = gmul(0x09,a0) ^ gmul(0x0e,a1) ^ gmul(0x0b,a2) ^ gmul(0x0d,a3);
        state[2][j] = gmul(0x0d,a0) ^ gmul(0x09,a1) ^ gmul(0x0e,a2) ^ gmul(0x0b,a3);
        state[3][j] = gmul(0x0b,a0) ^ gmul(0x0d,a1) ^ gmul(0x09,a2) ^ gmul(0x0e,a3);
    }
}

// AddRoundKey
void addRoundKey(vector<vector<uint8_t>>& state, vector<uint8_t> rk) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] ^= rk[j * 4 + i];
}

// Шифрование одного блока
vector<uint8_t> encryptBlock(vector<uint8_t> block, vector<uint8_t> expKey) {
    vector<vector<uint8_t>> state = bytesToState(block);
    
    addRoundKey(state, vector<uint8_t>(expKey.begin(), expKey.begin()+16));
    printState(state, "State после AddRoundKey (0)");

    for (int r = 1; r < NUM_ROUNDS; r++) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, vector<uint8_t>(expKey.begin()+r*16, expKey.begin()+(r+1)*16));
        if (r == 1) printState(state, "State после раунда 1"); // выводим для наглядности
    }
    
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, vector<uint8_t>(expKey.begin()+NUM_ROUNDS*16, expKey.end()));
    return stateToBytes(state);
}

// Расшифрование одного блока
vector<uint8_t> decryptBlock(vector<uint8_t> block, vector<uint8_t> expKey) {
    vector<vector<uint8_t>> state = bytesToState(block);
    addRoundKey(state, vector<uint8_t>(expKey.end()-16, expKey.end()));
    for (int r = NUM_ROUNDS - 1; r > 0; r--) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, vector<uint8_t>(expKey.begin()+r*16, expKey.begin()+(r+1)*16));
        invMixColumns(state);
    }
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, vector<uint8_t>(expKey.begin(), expKey.begin()+16));
    return stateToBytes(state);
}

// РЕЖИМ CBC 

// Шифрование CBC
vector<uint8_t> encryptCBC(vector<uint8_t> pt, vector<uint8_t> key, vector<uint8_t> iv) {
    vector<uint8_t> expKey = expandKey(key);
    vector<uint8_t> ct;
    vector<uint8_t> prev = iv;

    for (size_t i = 0; i < pt.size(); i += BLOCK_SIZE) {
        vector<uint8_t> block(BLOCK_SIZE);
        for (int j = 0; j < BLOCK_SIZE; j++)
            block[j] = pt[i+j] ^ prev[j];
        
        vector<uint8_t> enc = encryptBlock(block, expKey);
        ct.insert(ct.end(), enc.begin(), enc.end());
        prev = enc;
    }
    return ct;
}

// Расшифрование CBC
vector<uint8_t> decryptCBC(vector<uint8_t> ct, vector<uint8_t> key, vector<uint8_t> iv) {
    vector<uint8_t> expKey = expandKey(key);
    vector<uint8_t> pt;
    vector<uint8_t> prev = iv;

    for (size_t i = 0; i < ct.size(); i += BLOCK_SIZE) {
        vector<uint8_t> block(ct.begin()+i, ct.begin()+i+BLOCK_SIZE);
        vector<uint8_t> dec = decryptBlock(block, expKey);
        
        vector<uint8_t> plain(BLOCK_SIZE);
        for (int j = 0; j < BLOCK_SIZE; j++)
            plain[j] = dec[j] ^ prev[j];
        
        pt.insert(pt.end(), plain.begin(), plain.end());
        prev = block;
    }
    return pt;
}

//  MAIN 
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    // 1. Генерация ключа и IV
    vector<uint8_t> key = generateRandom(KEY_SIZE);
    vector<uint8_t> iv  = generateRandom(BLOCK_SIZE);
    saveKeyToFile(key, "aes_key.bin");

    cout << endl << "ПАРАМЕТРЫ" << endl;
    printBytes(key, "Master Key");
    printBytes(iv,  "IV");

    // 2. Вывод всех раундовых ключей
    vector<uint8_t> expKey = expandKey(key);
    cout << "РАУНДОВЫЕ КЛЮЧИ" << endl;
    for (int i = 0; i <= NUM_ROUNDS; i++)
        printRoundKey(vector<uint8_t>(expKey.begin()+i*16, expKey.begin()+(i+1)*16), i);

    // 3. Ввод текста
    cout << endl << "Введите текст (русский/английский):" << endl;
    string input;
    getline(cin, input);
    if (input.empty()) {
        cout << "Ошибка: пустой ввод!" << endl;
        return 1;
    }

    // 4. Подготовка данных
    vector<uint8_t> data = stringToBytes(input);
    cout << endl << "ДО ШИФРОВАНИЯ" << endl;
    cout << "Текст: " << input << endl;
    printBytes(data, "Байты текста");

    vector<uint8_t> padded = applyPadding(data);
    printBytes(padded, "С паддингом");

    // 5. Шифрование CBC
    cout << endl << "ПРОЦЕСС ШИФРОВАНИЯ" << endl;
    vector<uint8_t> ct = encryptCBC(padded, key, iv);
    printBytes(ct, "Шифротекст");

    // 6. Расшифрование CBC
    cout << endl << "РАСШИФРОВАНИЕ" << endl;
    vector<uint8_t> decPadded = decryptCBC(ct, key, iv);
    vector<uint8_t> dec = removePadding(decPadded);
    printBytes(dec, "Расшифрованные байты");

    string result = bytesToString(dec);
    cout << "Расшифрованный текст: " << result << endl;

    // 7. Проверка
    if (input == result)
        cout << endl << "Успех: текст совпадает!" << endl;
    else
        cout << endl << "Ошибка: текст не совпадает!" << endl;

    return 0;
}
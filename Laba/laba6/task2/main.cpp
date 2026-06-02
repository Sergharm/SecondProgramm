#include "aes.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif


    // Генерация ключа и IV
    vector<uint8_t> key = generateRandomBytes(AES_KEY_SIZE);
    vector<uint8_t> iv = generateRandomBytes(AES_BLOCK_SIZE);
    saveKeyToFile(key, "aes_cbc_key.bin");

    cout << "\nСГЕНЕРИРОВАННЫЕ ПАРАМЕТРЫ" << endl;
    printBytes(key, "Key");
    printBytes(iv, "IV");

    // Расширение ключа
    vector<uint8_t> expKey = expandKey(key);

    cout << "\nРАУНДОВЫЕ КЛЮЧИ" << endl;
    for (unsigned int i = 0; i <= AES_NUM_ROUNDS; ++i) {
        printRoundKey(getRoundKey(expKey, i), i);
    }

    // Ввод текста
    cout << "\nВведите текст для шифрования (русский/английский):" << endl;
    string plaintext;
    getline(cin, plaintext);

    if (plaintext.empty()) {
        cout << "Ошибка: введен пустой текст" << endl;
        return 1;
    }

    // Подготовка данных
    vector<uint8_t> ptBytes = stringToBytes(plaintext);
    cout << "\nИСХОДНЫЕ ДАННЫЕ" << endl;
    cout << "Исходный текст: " << plaintext << endl;
    printBytes(ptBytes, "Plaintext bytes");

    // PKCS7 Padding
    vector<uint8_t> padded = applyPKCS7Padding(ptBytes, AES_BLOCK_SIZE);
    cout << "\nПОСЛЕ PKCS7 PADDING" << endl;
    printBytes(padded, "Padded bytes");

    // Шифрование
    cout << "\nПРОЦЕСС ШИФРОВАНИЯ" << endl;
    vector<uint8_t> ciphertext = encryptCBC(padded, key, iv);
    printBytes(ciphertext, "Ciphertext");

    // Расшифрование
    cout << "\nРАСШИФРОВАНИЕ" << endl;
    vector<uint8_t> decryptedPadded = decryptCBC(ciphertext, key, iv);
    vector<uint8_t> decrypted = removePKCS7Padding(decryptedPadded);
    printBytes(decrypted, "Decrypted bytes");

    string decryptedStr(decrypted.begin(), decrypted.end());
    cout << "Расшифрованный текст: " << decryptedStr << endl;

    // Проверка
    if (plaintext == decryptedStr) {
        cout << "Успех: исходный и расшифрованный текст совпадают" << endl;
    } else {
        cout << "Ошибка: текст не совпадает!" << endl;
    }

    return 0;
}
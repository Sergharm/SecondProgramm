#pragma once
#include <string>

typedef unsigned long long uint;

// Структура для открытого ключа (то, что можно всем показывать)
struct ElGamalPublicKey {
    uint prostoeP;      // простое число (модуль)
    uint generatorG;    // первообразный корень
    uint otkritiyY;     // открытый ключ = g^x mod p
};

// Главное меню Эль-Гамаля
void runElGamalMenu();

// Функции шифрования и дешифрования файлов
void encryptFileElGamal(const std::string& fajlIn, const std::string& fajlOut, ElGamalPublicKey pub);
void decryptFileElGamal(const std::string& fajlIn, const std::string& fajlOut, uint zakritiyX, uint modulP);
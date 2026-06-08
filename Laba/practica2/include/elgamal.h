#pragma once
#include <string>

// Структура для открытого ключа
struct ElGamalPublicKey {
    long long p; 
    long long g; 
    long long y; 
};

// Главное меню Эль-Гамаля
void runElGamalMenu();

// Функции шифрования и дешифрования файлов
void encryptFileElGamal(const std::string& inputFile, const std::string& outputFile, ElGamalPublicKey pub);
void decryptFileElGamal(const std::string& inputFile, const std::string& outputFile, long long x, long long p);

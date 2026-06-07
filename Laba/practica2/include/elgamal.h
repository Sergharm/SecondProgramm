#ifndef ELGAMAL_H
#define ELGAMAL_H

#include <string>
using namespace std;

// ключи эль-гамаля
struct Keys {
    long long p;  // простое число (модуль)
    long long g;  // образующий элемент
    long long y;  // открытый ключ
    long long x;  // закрытый ключ (секретный!)
};

// генерация ключей
Keys gen_keys(long long p, long long g);

// шифрование одного байта
void encrypt_byte(unsigned char byte, const Keys& pub, long long& c1, long long& c2);

// дешифрование одного байта
unsigned char decrypt_byte(long long c1, long long c2, long long x, long long p);

// шифрование файла
void encrypt_file(string in_path, string out_path, const Keys& pub);

// дешифрование файла
void decrypt_file(string in_path, string out_path, long long x, long long p);

#endif
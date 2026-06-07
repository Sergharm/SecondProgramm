#include "elgamal.h"
#include "modulo.h"
#include "euclid.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// генерация случайного числа в диапазоне [min, max] для ключей. 
long long random_num(long long min_val, long long max_val) {
    return min_val + rand() % (max_val - min_val + 1);
}

// генерация ключей эль-гамаля
Keys gen_keys(long long p, long long g) {
    cout << "\n Генерация ключей Эль-Гамаля" << endl;
    
    if (!is_prime(p)) {
        cout << "ОШИБКА: p должно быть простым!" << endl;
        Keys bad; bad.p = 0; bad.g = 0; bad.y = 0; bad.x = 0;
        return bad;
    }
    
    // закрытый ключ x - случайное число от 1 до p-2
    long long xaxa = random_num(1, p - 2);
    cout << "Закрытый ключ x = " << xaxa << " (секретное число от 1 до " << p-2 << ")" << endl;
    
    // открытый ключ y = g^x mod p
    long long ya = binary_pow(g, xaxa, p);
    cout << "Открытый ключ y = g^x mod p = " << g << "^" << xaxa << " mod " << p << " = " << ya << endl;
    
    Keys k;
    k.p = p;
    k.g = g;
    k.y = ya;
    k.x = xaxa;
    return k;
}

// шифрование одного байта
// на выходе пара (c1, c2)
void encrypt_byte(unsigned char bukovka, const Keys& pub, long long& c1, long long& c2) {
    long long k = random_num(1, pub.p - 2);
    
    // c1 = g^k mod p
    c1 = binary_pow(pub.g, k, pub.p);
    
    // c2 = (m * y^k) mod p
    long long yk = binary_pow(pub.y, k, pub.p);
    c2 = safe_mul((long long)bukovka, yk, pub.p);
    
    cout << "  Шифруем байт " << (int)bukovka;
    if (bukovka >= 32 && bukovka < 127) cout << " ('" << (char)bukovka << "')";
    cout << " -> c1=" << c1 << ", c2=" << c2 << endl;
}

// дешифрование одного байта
// m = c2 * (c1^x)^(-1) mod p
unsigned char decrypt_byte(long long c1, long long c2, long long x, long long p) {
    // s = c1^x mod p
    long long s = binary_pow(c1, x, p);
    
    // обратный элемент к s
    long long s_inv = mod_inverse(s, p);
    if (s_inv == -1) {
        cout << "ОШИБКА при дешифровании!" << endl;
        return 0;
    }
    
    // m = c2 * s^(-1) mod p
    long long m = safe_mul(c2, s_inv, p);
    
    cout << "  Расшифровываем c1=" << c1 << ", c2=" << c2 << " -> байт " << (int)m;
    if (m >= 32 && m < 127) cout << " ('" << (char)m << "')";
    cout << endl;
    
    return (unsigned char)m;
}

// шифрование файла
void encrypt_file(string in_path, string out_path, const Keys& pub) {
    cout << "\nШифрование файла" << endl;
    cout << "Вход: " << in_path << endl;
    cout << "Выход: " << out_path << endl;
    
    ifstream fin(in_path, ios::binary);
    if (!fin) {
        cout << "ОШИБКА: не могу открыть файл " << in_path << endl;
        return;
    }
    
    ofstream fout(out_path, ios::binary);
    if (!fout) {
        cout << "ОШИБКА: не могу создать файл " << out_path << endl;
        return;
    }
    
    unsigned char bukovka;
    int count = 0;
    while (fin.read((char*)&bukovka, 1)) {
        long long c1, c2;
        encrypt_byte(bukovka, pub, c1, c2);
        
        // записываем пару чисел в бинарный файл
        // (тут я не до конца понял как это работает, но работает - не трогай)
        fout.write((char*)&c1, sizeof(c1));
        fout.write((char*)&c2, sizeof(c2));
        count++;
    }
    
    fin.close();
    fout.close();
    cout << "Зашифровано байт: " << count << endl;
    cout << "Размер шифротекста: " << count * 2 * sizeof(long long) << " байт" << endl;
}

// дешифрование файла
void decrypt_file(string in_path, string out_path, long long x, long long p) {
    cout << "\n=== Дешифрование файла ===" << endl;
    cout << "Вход: " << in_path << endl;
    cout << "Выход: " << out_path << endl;
    
    ifstream fin(in_path, ios::binary);
    if (!fin) {
        cout << "ОШИБКА: не могу открыть файл " << in_path << endl;
        return;
    }
    
    ofstream fout(out_path, ios::binary);
    if (!fout) {
        cout << "ОШИБКА: не могу создать файл " << out_path << endl;
        return;
    }
    
    long long c1, c2;
    int count = 0;
    // читаем по две long long за раз
    while (fin.read((char*)&c1, sizeof(c1)) && fin.read((char*)&c2, sizeof(c2))) {
        unsigned char bukovka = decrypt_byte(c1, c2, x, p);
        fout.write((char*)&bukovka, 1);
        count++;
    }
    
    fin.close();
    fout.close();
    cout << "Дешифровано байт: " << count << endl;
}
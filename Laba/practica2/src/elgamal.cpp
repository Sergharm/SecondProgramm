#include "../include/elgamal.h"
#include "../include/modulo.h"
#include "../include/euclid.h"
#include "../include/utils.h"
#include <iostream>
#include <fstream>

using namespace std;

// Побайтовое шифрование файла методом Эль-Гамаля
void encryptFileElGamal(const string& fajlIn, const string& fajlOut, ElGamalPublicKey pub) {
    ifstream in(fajlIn, ios::binary);
    ofstream out(fajlOut, ios::binary);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка при открытии файлов для шифрования!\n";
        return;
    }

    cout << "Шифрование файла: " << fajlIn << " -> " << fajlOut << "\n";
    
    // Выбираем случайную (сессионную) сессию k, взаимно простую с p-1
    // Для учебной реализации можно запросить ввод или использовать фиксированное k
    uint64_t k = get_ll("Введите сессионный ключ k (взаимно простой с p-1): ", 2, pub.prostoeP - 2);
    
    // Общая часть для всех байтов сообщения
    uint64_t a = powerMod(pub.generatorG, k, pub.prostoeP, false);
    
    char byte;
    while (in.get(byte)) {
        uint64_t m = static_cast<unsigned char>(byte);
        // b = (y^k * m) mod p
        uint64_t yk = powerMod(pub.otkritiyY, k, pub.prostoeP, false);
        uint64_t b = (static_cast<__int128>(yk) * m) % pub.prostoeP;
        
        // Записываем пару чисел (a, b) как 64-битные блоки
        out.write(reinterpret_cast<const char*>(&a), sizeof(a));
        out.write(reinterpret_cast<const char*>(&b), sizeof(b));
    }
    
    cout << "Шифрование завершено.\n";
}

// Побайтовое дешифрование файла методом Эль-Гамаля
void decryptFileElGamal(const string& fajlIn, const string& fajlOut, uint64_t zakritiyX, uint64_t modulP) {
    ifstream in(fajlIn, ios::binary);
    ofstream out(fajlOut, ios::binary);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка при открытии файлов для дешифрования!\n";
        return;
    }

    cout << "Дешифрование файла: " << fajlIn << " -> " << fajlOut << "\n";
    
    uint64_t a, b;
    while (in.read(reinterpret_cast<char*>(&a), sizeof(a)) && 
           in.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        
        // m = b * (a^x)^(-1) mod p
        uint64_t ax = powerMod(a, zakritiyX, modulP, false);
        uint64_t inv_ax = modInverse(ax, modulP);
        
        uint64_t m = (static_cast<__int128>(b) * inv_ax) % modulP;
        char byte = static_cast<char>(m);
        out.put(byte);
    }
    
    cout << "Дешифрование завершено.\n";
}

// Интерактивное меню Эль-Гамаля
void runElGamalMenu() {
    cout << "\n[Задание 4: Криптосистема Эль-Гамаля]\n";
    cout << "1. Зашифровать файл\n";
    cout << "2. Расшифровать файл\n";
    int choice = static_cast<int>(get_ll("Выберите действие: ", 1, 2));

    if (choice == 1) {
        ElGamalPublicKey pub;
        pub.prostoeP = get_ll("Введите простое число p: ", 3);
        pub.generatorG = get_ll("Введите первообразный корень g: ", 2);
        uint64_t x = get_ll("Введите секретный ключ x (1 < x < p-1): ", 2, pub.prostoeP - 2);
        
        // Вычисляем открытый ключ y = g^x mod p
        pub.otkritiyY = powerMod(pub.generatorG, x, pub.prostoeP, false);
        cout << "Сгенерирован открытый ключ y = " << pub.otkritiyY << "\n";

        string fin = get_existing_file("Введите имя исходного файла для шифрования: ");
        string fout = get_new_file("Введите имя результирующего (зашифрованного) файла: ");
        
        encryptFileElGamal(fin, fout, pub);
    } else {
        uint64_t p = get_ll("Введите модуль p: ", 3);
        uint64_t x = get_ll("Введите ваш секретный ключ x: ", 2, p - 2);
        
        string fin = get_existing_file("Введите имя зашифрованного файла: ");
        string fout = get_new_file("Введите имя файла для расшифрованного текста: ");
        
        decryptFileElGamal(fin, fout, x, p);
    }
}

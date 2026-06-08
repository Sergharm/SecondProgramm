#include "elgamal.h"
#include "modulo.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdint>

using namespace std;

void runElGamalMenu() {
    static uint64 prostoeP = 65537; 
    static uint64 generatorG = 3;     
    static uint64 zakritiyX = 12345; 
    static uint64 otkritiyY = powerMod(generatorG, zakritiyX, prostoeP, false); 

    int vybor = -1;
    while (vybor != 0) {
        cout << "\n--- Подменю: Криптосистема Эль-Гамаля ---\n";
        cout << "1. Сгенерировать / Показать ключи\n";
        cout << "2. Зашифровать файл\n";
        cout << "3. Расшифровать файл\n";
        cout << "0. Назад в главное меню\n";
        
        vybor = static_cast<int>(get_ll("Выберите действие: ", 0, 3));

        if (vybor == 1) {
            cout << "\n[Генерация ключей Эль-Гамаля]\n";
            prostoeP = get_ll("Введите простое число p (например, 257 или 65537): ", 2);
            if (!isPrime(prostoeP)) {
                cout << "Ошибка: " << prostoeP << " не является простым числом!\n";
                continue;
            }
            generatorG = get_ll("Введите первообразный корень g: ", 2, prostoeP - 1);
            zakritiyX = get_ll("Введите секретный ключ x (1 <= x < p): ", 1, prostoeP - 2);
            
            cout << "\nВычисляем открытый ключ y = g^x mod p:\n";
            otkritiyY = powerMod(generatorG, zakritiyX, prostoeP, true);

            cout << "\n[Ключи]:\n";
            cout << "  Открытый ключ:  (p = " << prostoeP << ", g = " << generatorG << ", y = " << otkritiyY << ")\n";
            cout << "  Закрытый ключ:  x = " << zakritiyX << "\n";
        }
        else if (vybor == 2) {
            cout << "\n[Шифрование файла]\n";
            string fajlIn = get_existing_file("Исходный файл: ");
            string fajlOut = get_new_file("Зашифрованный файл: ");
            
            ElGamalPublicKey pub = {prostoeP, generatorG, otkritiyY};
            encryptFileElGamal(fajlIn, fajlOut, pub);
            cout << "Запомните закрытый ключ x = " << zakritiyX << " для дешифрования!\n";
        }
        else if (vybor == 3) {
            cout << "\n[Дешифрование файла]\n";
            string fajlIn = get_existing_file("Зашифрованный файл: ");
            string fajlOut = get_new_file("Расшифрованный файл: ");
            
            uint64 inputX = get_ll("Секретный ключ x: ", 1);
            uint64 inputP = get_ll("Модуль p: ", 2);

            decryptFileElGamal(fajlIn, fajlOut, inputX, inputP);
        }
    }
}

void encryptFileElGamal(const string& fajlIn, const string& fajlOut, ElGamalPublicKey pub) {
    if (pub.prostoeP <= 15) {
        cout << "Ошибка: модуль p должен быть > 15 для шифрования байтов!\n";
        return;
    }

    ifstream in(fajlIn, ios::binary);
    ofstream out(fajlOut, ios::binary);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка открытия файлов.\n";
        return;
    }

    cout << "  Шифрование Эль-Гамалем (побайтово, каждый байт делится на 2 полубайта)...\n";
    cout << "  Параметры: p=" << pub.prostoeP << ", g=" << pub.generatorG << ", y=" << pub.otkritiyY << "\n";
    
    char surovoyBait;
    int schetchik = 0;

    while (in.get(surovoyBait)) {
        unsigned char bait = static_cast<unsigned char>(surovoyBait);
        
        uint64 polubait1 = bait >> 4;
        uint64 polubait2 = bait & 0x0F;

        uint64 k1 = 2 + rand() % (pub.prostoeP - 3);
        uint64 a1 = powerMod(pub.generatorG, k1, pub.prostoeP, false);
        uint64 y_k1 = powerMod(pub.otkritiyY, k1, pub.prostoeP, false);
        uint64 b1 = static_cast<uint64>((static_cast<__int128>(polubait1) * y_k1) % pub.prostoeP);

        uint64 k2 = 2 + rand() % (pub.prostoeP - 3);
        uint64 a2 = powerMod(pub.generatorG, k2, pub.prostoeP, false);
        uint64 y_k2 = powerMod(pub.otkritiyY, k2, pub.prostoeP, false);
        uint64 b2 = static_cast<uint64>((static_cast<__int128>(polubait2) * y_k2) % pub.prostoeP);

        out.write(reinterpret_cast<char*>(&a1), sizeof(a1));
        out.write(reinterpret_cast<char*>(&b1), sizeof(b1));
        out.write(reinterpret_cast<char*>(&a2), sizeof(a2));
        out.write(reinterpret_cast<char*>(&b2), sizeof(b2));
        
        schetchik++;
    }
    
    in.close();
    out.close();
    cout << "Зашифровано байт: " << schetchik << "\n";
    cout << "Размер шифротекста: " << schetchik * 4 * sizeof(uint64) << " байт\n";
    cout << "Файл успешно зашифрован.\n";
}

void decryptFileElGamal(const string& fajlIn, const string& fajlOut, uint64 zakritiyX, uint64 modulP) {
    ifstream in(fajlIn, ios::binary);
    ofstream out(fajlOut, ios::binary);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка открытия файлов.\n";
        return;
    }

    uint64 a1, b1, a2, b2;
    cout << "  Расшифровка (ключ x=" << zakritiyX << ", модуль p=" << modulP << "):\n";
    cout << "  Расшифрованный текст: ";

    int schetchik = 0;
    while (in.read(reinterpret_cast<char*>(&a1), sizeof(a1)) && 
           in.read(reinterpret_cast<char*>(&b1), sizeof(b1)) &&
           in.read(reinterpret_cast<char*>(&a2), sizeof(a2)) && 
           in.read(reinterpret_cast<char*>(&b2), sizeof(b2))) {
        
        uint64 s1 = powerMod(a1, zakritiyX, modulP, false);
        uint64 s_inv1 = powerMod(s1, modulP - 2, modulP, false);
        uint64 m1 = static_cast<uint64>((static_cast<__int128>(b1) * s_inv1) % modulP);

        uint64 s2 = powerMod(a2, zakritiyX, modulP, false);
        uint64 s_inv2 = powerMod(s2, modulP - 2, modulP, false);
        uint64 m2 = static_cast<uint64>((static_cast<__int128>(b2) * s_inv2) % modulP);

        unsigned char bait = static_cast<unsigned char>(((m1 & 0x0F) << 4) | (m2 & 0x0F));
        
        cout << bait;      
        out.put(static_cast<char>(bait)); 
        schetchik++;
    }
    cout << "\n";
    
    in.close();
    out.close();
    cout << "Расшифровано байт: " << schetchik << "\n";
    cout << "Файл успешно обработан.\n";
}
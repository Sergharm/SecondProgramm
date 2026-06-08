#include "elgamal.h"
#include "modulo.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

void runElGamalMenu() {
    // Статические переменные для памяти ключей (чтобы не вводить каждый раз)
    static uint prostoeP = 65537; 
    static uint generatorG = 3;     
    static uint zakritiyX = 12345; 
    static uint otkritiyY = powerMod(generatorG, zakritiyX, prostoeP, false); 

    int vybor = -1;
    while (vybor != 0) {
        cout << "\n--- Подменю: Криптосистема Эль-Гамаля ---\n";
        cout << "1. Сгенерировать / Показать ключи\n";
        cout << "2. Зашифровать файл\n";
        cout << "3. Расшифровать файл\n";
        cout << "0. Назад в главное меню\n";
        
        vybor = static_cast<int>(get_ll("Выберите действие: ", 0, 3));

        if (vybor == 1) {
            cout << "\n[Генерация ключей]\n";
            prostoeP = get_ll("Введите простое число p (рекомендуется > 15, например 257 или 65537): ", 2);
            generatorG = get_ll("Введите первообразный корень g: ", 2);
            zakritiyX = get_ll("Введите ваш секретный ключ x (x < p): ", 1, prostoeP - 1);
            
            otkritiyY = powerMod(generatorG, zakritiyX, prostoeP, false);

            cout << "\n[Ключи успешно обновлены]:\n";
            cout << "  Открытый ключ: (p = " << prostoeP << ", g = " << generatorG << ", y = " << otkritiyY << ")\n";
            cout << "  Закрытый ключ: (x = " << zakritiyX << ")\n";
        }
        else if (vybor == 2) {
            cout << "\n[Шифрование файла]\n";
            string fajlIn = get_existing_file("Введите имя исходного файла (например, plain.txt): ");
            string fajlOut = get_new_file("Введите имя для зашифрованного файла (например, cipher.bin): ");
            
            ElGamalPublicKey pub = {prostoeP, generatorG, otkritiyY};
            encryptFileElGamal(fajlIn, fajlOut, pub);
        }
        else if (vybor == 3) {
            cout << "\n[Дешифрование файла]\n";
            string fajlIn = get_existing_file("Введите имя зашифрованного файла (например, cipher.bin): ");
            string fajlOut = get_new_file("Введите имя для расшифрованного файла (например, decrypted.txt): ");
            
            uint inputX = get_ll("Введите секретный ключ x для расшифровки: ", 1);
            uint inputP = get_ll("Введите модуль p для расшифровки: ", 2);

            decryptFileElGamal(fajlIn, fajlOut, inputX, inputP);
        }
    }
}

void encryptFileElGamal(const string& fajlIn, const string& fajlOut, ElGamalPublicKey pub) {
    if (pub.prostoeP <= 15) {
        cout << "Ошибка: Для шифрования любого языка модуль p должен быть строго больше 15!\n";
        return;
    }

    ifstream in(fajlIn, ios::binary);
    ofstream out(fajlOut, ios::binary);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка открытия файлов.\n";
        return;
    }

    cout << "  Классическое шифрование Эль-Гамаля...\n";
    char surovoyBait;

    while (in.get(surovoyBait)) {
        unsigned char bait = static_cast<unsigned char>(surovoyBait);
        
        // Разделяем 8-битный байт на две половины по 4 бита (значения от 0 до 15)
        uint polubait1 = bait >> 4;     // Старшие 4 бита
        uint polubait2 = bait & 0x0F;   // Младшие 4 бита

        // Шифруем первую половину байта (polubait1)
        uint ephemeralK1 = 2 + rand() % (pub.prostoeP - 3);
        uint shifrA1 = powerMod(pub.generatorG, ephemeralK1, pub.prostoeP, false);
        uint y_stepen1 = powerMod(pub.otkritiyY, ephemeralK1, pub.prostoeP, false);
        uint shifrB1 = static_cast<uint>((static_cast<__int128>(polubait1) * y_stepen1) % pub.prostoeP);

        // Шифруем вторую половину байта (polubait2)
        uint ephemeralK2 = 2 + rand() % (pub.prostoeP - 3);
        uint shifrA2 = powerMod(pub.generatorG, ephemeralK2, pub.prostoeP, false);
        uint y_stepen2 = powerMod(pub.otkritiyY, ephemeralK2, pub.prostoeP, false);
        uint shifrB2 = static_cast<uint>((static_cast<__int128>(polubait2) * y_stepen2) % pub.prostoeP);

        // Пишем 4 числа типа uint в файл на каждый 1 байт текста
        out.write(reinterpret_cast<char*>(&shifrA1), sizeof(shifrA1));
        out.write(reinterpret_cast<char*>(&shifrB1), sizeof(shifrB1));
        out.write(reinterpret_cast<char*>(&shifrA2), sizeof(shifrA2));
        out.write(reinterpret_cast<char*>(&shifrB2), sizeof(shifrB2));
    }
    
    in.close();
    out.close();
    cout << "Файл успешно зашифрован.\n";
}

void decryptFileElGamal(const string& fajlIn, const string& fajlOut, uint zakritiyX, uint modulP) {
    ifstream in(fajlIn, ios::binary);
    ofstream out(fajlOut, ios::binary);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка открытия файлов.\n";
        return;
    }

    uint shifrA1, shifrB1, shifrA2, shifrB2;
    cout << "  Расшифрованный текст: ";

    // Читаем блоки по 4 числа uint
    while (in.read(reinterpret_cast<char*>(&shifrA1), sizeof(shifrA1)) && in.read(reinterpret_cast<char*>(&shifrB1), sizeof(shifrB1)) &&
           in.read(reinterpret_cast<char*>(&shifrA2), sizeof(shifrA2)) && in.read(reinterpret_cast<char*>(&shifrB2), sizeof(shifrB2))) {
        
        // Восстанавливаем старшие 4 бита
        uint s1 = powerMod(shifrA1, zakritiyX, modulP, false);
        uint s_inv1 = powerMod(s1, modulP - 2, modulP, false); 
        uint message1 = static_cast<uint>((static_cast<__int128>(shifrB1) * s_inv1) % modulP);

        // Восстанавливаем младшие 4 бита
        uint s2 = powerMod(shifrA2, zakritiyX, modulP, false);
        uint s_inv2 = powerMod(s2, modulP - 2, modulP, false); 
        uint message2 = static_cast<uint>((static_cast<__int128>(shifrB2) * s_inv2) % modulP);

        // Собираем байт обратно из двух половин
        // Если ключ неверный, значения выйдут за рамки 0-15 и превратятся в случайный мусор
        unsigned char bait = static_cast<unsigned char>(((message1 & 0x0F) << 4) | (message2 & 0x0F));
        
        cout << bait;      
        out.put(static_cast<char>(bait)); 
    }
    cout << "\n";
    
    in.close();
    out.close();
    cout << "Файл успешно обработан.\n";
}
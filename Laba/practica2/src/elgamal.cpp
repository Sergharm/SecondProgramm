#include "elgamal.h"
#include "modulo.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

void runElGamalMenu() {
    // Статические переменные для памяти ключей
    static long long p = 65537; 
    static long long g = 3;     
    static long long x = 12345; 
    static long long y = powerMod(g, x, p, false); 

    int choice = -1;
    while (choice != 0) {
        cout << "\n--- Подменю: Криптосистема Эль-Гамаля ---\n";
        cout << "1. Сгенерировать / Показать ключи\n";
        cout << "2. Зашифровать файл\n";
        cout << "3. Расшифровать файл\n";
        cout << "0. Назад в главное меню\n";
        
        choice = static_cast<int>(get_ll("Выберите действие: ", 0, 3));

        if (choice == 1) {
            cout << "\n[Генерация ключей]\n";
            p = get_ll("Введите простое число p (рекомендуется > 15, например 257 или 65537): ", 2);
            g = get_ll("Введите первообразный корень g: ", 2);
            x = get_ll("Введите ваш секретный ключ x (x < p): ", 1, p - 1);
            
            y = powerMod(g, x, p, false);

            cout << "\n[Ключи успешно обновлены]:\n";
            cout << "  Открытый ключ: (p = " << p << ", g = " << g << ", y = " << y << ")\n";
            cout << "  Закрытый ключ: (x = " << x << ")\n";
        }
        else if (choice == 2) {
            cout << "\n[Шифрование файла]\n";
            string inFile = get_existing_file("Введите имя исходного файла (например, plain.txt): ");
            string outFile = get_new_file("Введите имя для зашифрованного файла (например, cipher.bin): ");
            
            ElGamalPublicKey pub = {p, g, y};
            encryptFileElGamal(inFile, outFile, pub);
        }
        else if (choice == 3) {
            cout << "\n[Дешифрование файла]\n";
            string inFile = get_existing_file("Введите имя зашифрованного файла (например, cipher.bin): ");
            string outFile = get_new_file("Введите имя для расшифрованного файла (например, decrypted.txt): ");
            
            long long input_x = get_ll("Введите секретный ключ x для расшифровки: ", 1);
            long long input_p = get_ll("Введите модуль p для расшифровки: ", 2);

            decryptFileElGamal(inFile, outFile, input_x, input_p);
        }
    }
}

void encryptFileElGamal(const string& inputFile, const string& outputFile, ElGamalPublicKey pub) {
    if (pub.p <= 15) {
        cout << "Ошибка: Для шифрования любого языка модуль p должен быть строго больше 15!\n";
        return;
    }

    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка открытия файлов.\n";
        return;
    }

    cout << "  Классическое шифрование Эль-Гамаля...\n";
    char raw_byte;

    while (in.get(raw_byte)) {
        unsigned char byte = static_cast<unsigned char>(raw_byte);
        
        // Разделяем 8-битный байт на две половины по 4 бита (значения от 0 до 15)
        long long m1 = byte >> 4;     // Старшие 4 бита
        long long m2 = byte & 0x0F;   // Младшие 4 бита

        // Шифруем первую половину байта (m1)
        long long k1 = 2 + rand() % (pub.p - 3);
        long long a1 = powerMod(pub.g, k1, pub.p, false);
        long long y_k1 = powerMod(pub.y, k1, pub.p, false);
        long long b1 = static_cast<long long>((static_cast<__int128>(m1) * y_k1) % pub.p);

        // Шифруем вторую половину байта (m2)
        long long k2 = 2 + rand() % (pub.p - 3);
        long long a2 = powerMod(pub.g, k2, pub.p, false);
        long long y_k2 = powerMod(pub.y, k2, pub.p, false);
        long long b2 = static_cast<long long>((static_cast<__int128>(m2) * y_k2) % pub.p);

        // Пишем 4 числа типа long long в файл на каждый 1 байт текста
        out.write(reinterpret_cast<char*>(&a1), sizeof(a1));
        out.write(reinterpret_cast<char*>(&b1), sizeof(b1));
        out.write(reinterpret_cast<char*>(&a2), sizeof(a2));
        out.write(reinterpret_cast<char*>(&b2), sizeof(b2));
    }
    
    in.close();
    out.close();
    cout << "Файл успешно зашифрован.\n";
}

void decryptFileElGamal(const string& inputFile, const string& outputFile, long long x, long long p) {
    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);
    
    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка открытия файлов.\n";
        return;
    }

    long long a1, b1, a2, b2;
    cout << "  Расшифрованный текст: ";

    // Читаем блоки по 4 числа long long
    while (in.read(reinterpret_cast<char*>(&a1), sizeof(a1)) && in.read(reinterpret_cast<char*>(&b1), sizeof(b1)) &&
           in.read(reinterpret_cast<char*>(&a2), sizeof(a2)) && in.read(reinterpret_cast<char*>(&b2), sizeof(b2))) {
        
        // Восстанавливаем старшие 4 бита
        long long s1 = powerMod(a1, x, p, false);
        long long s_inv1 = powerMod(s1, p - 2, p, false); 
        long long m1 = static_cast<long long>((static_cast<__int128>(b1) * s_inv1) % p);

        // Восстанавливаем младшие 4 бита
        long long s2 = powerMod(a2, x, p, false);
        long long s_inv2 = powerMod(s2, p - 2, p, false); 
        long long m2 = static_cast<long long>((static_cast<__int128>(b2) * s_inv2) % p);

        // Собираем байт обратно из двух половин
        // Если ключ неверный, значения выйдут за рамки 0-15 и превратятся в случайный мусор
        unsigned char byte = static_cast<unsigned char>(((m1 & 0x0F) << 4) | (m2 & 0x0F));
        
        cout << byte;      
        out.put(static_cast<char>(byte)); 
    }
    cout << "\n";
    
    in.close();
    out.close();
    cout << "Файл успешно обработан.\n";
}

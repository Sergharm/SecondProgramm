#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <limits>
#include <string>
#include <fstream>
using namespace std;

// Чтение long long с проверкой диапазона и типа
long long get_ll(const string& prompt, long long min_val = 0, long long max_val = 1000000000000LL);

// Чтение строки (имя файла, путь)
string get_str(const string& prompt);

// Чтение имени существующего файла
string get_existing_file(const string& prompt);

// Чтение имени нового файла (проверка на пустоту)
string get_new_file(const string& prompt);

// Очистка буфера ввода
void clear_input();

#endif
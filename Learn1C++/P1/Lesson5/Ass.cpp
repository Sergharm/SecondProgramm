#include <iostream>
#include <clocale>
#include <windows.h>


int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");
    int a = 10;
    int b = 20;
    int result = 0;

    // Синтаксис: "инструкции" : [выходы] : [входы] : [измененные регистры]
    __asm__ (
        "addl %%ebx, %%eax;" 
        : "=a" (result) 
        : "a" (a), "b" (b)
    );

    std::cout << "Результат: " << result << std::endl;
    return 0;
}

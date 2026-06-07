#include "menu.h"
#include <iostream>
#include <clocale>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");  // чтобы русский выводился нормально. Мож
    show_menu();
    return 0;
}
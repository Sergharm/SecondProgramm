#include "warehouse.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <clocale>

using namespace std;

void initWarehouse(map<string, Cell>& wh) {
    const vector<char> zones = {'A', 'B', 'C'};
    
    for (const char z : zones) {
        for (int r = 1; r <= RACKS_COUNT; ++r) {
            for (int s = 1; s <= SECTIONS_COUNT; ++s) {
                for (int h = 1; h <= SHELVES_COUNT; ++h) {
                    string addr = string(1, z) + to_string(r) + to_string(s) + to_string(h);
                    wh[addr] = Cell();
                }
            }
        }
    }
}

bool isValidAddress(const string& addr) {
    if (addr.size() < 4) return false;
    
    const vector<char> validZones = {'A', 'B', 'C'};
    bool zoneValid = false;
    
    for (const char z : validZones) {
        if (addr[0] == z) {
            zoneValid = true;
            break;
        }
    }
    
    if (!zoneValid) return false;
    
    for (const char c : addr) {
        if (c == addr[0]) continue;
        if (!isdigit(c)) return false;
    }
    
    return true;
}

void addProduct(map<string, Cell>& wh, const string& prod, int qty, const string& addr) {
    if (!isValidAddress(addr)) {
        cout << "Ошибка: неверный адрес " << addr << endl;
        return;
    }
    
    Cell& cell = wh[addr];
    
    if (!cell.isEmpty() && cell.product != prod) {
        cout << "Ошибка: Ячейка " << addr << " занята товаром " << cell.product << endl;
        return;
    }
    
    if (cell.quantity + qty > MAX_PER_CELL) {
        cout << "Ошибка: Превышена вместимость ячейки (максимум " << MAX_PER_CELL << ")" << endl;
        return;
    }
    
    cell.product = prod;
    cell.quantity += qty;
    cout << "Добавлено " << qty << " " << prod << " в " << addr << endl;
}

void removeProduct(map<string, Cell>& wh, const string& prod, int qty, const string& addr) {
    if (!isValidAddress(addr)) {
        cout << "Ошибка: неверный адрес " << addr << endl;
        return;
    }
    
    auto it = wh.find(addr);
    if (it == wh.end()) return;
    
    Cell& cell = it->second;
    
    if (cell.isEmpty()) {
        cout << "Ошибка: Ячейка " << addr << " пуста" << endl;
        return;
    }
    
    if (cell.product != prod) {
        cout << "Ошибка: Товар " << prod << " не найден в ячейке " << addr << endl;
        return;
    }
    
    if (cell.quantity < qty) {
        cout << "Ошибка: Недостаточно товаров для удаления" << endl;
        return;
    }
    
    cell.quantity -= qty;
    cout << "Удалено " << qty << " " << prod << " (остаток: " << cell.quantity << ")" << endl;
    
    if (cell.isEmpty()) {
        cell.product = "";
    }
}

void showInfo(const map<string, Cell>& wh) {
    int totalStored = 0;
    const int totalCap = ZONES_COUNT * RACKS_COUNT * SECTIONS_COUNT * SHELVES_COUNT * MAX_PER_CELL;
    
    map<char, int> zoneStored;
    map<char, int> zoneCap;
    const vector<char> zones = {'A', 'B', 'C'};
    
    for (const char z : zones) {
        zoneCap[z] = RACKS_COUNT * SECTIONS_COUNT * SHELVES_COUNT * MAX_PER_CELL;
        zoneStored[z] = 0;
    }

    vector<string> filled;
    vector<string> empty;
    
    for (const auto& pair : wh) {
        const string& addr = pair.first;
        const Cell& cell = pair.second;
        const char z = addr[0];
        
        if (!cell.isEmpty()) {
            totalStored += cell.quantity;
            zoneStored[z] += cell.quantity;
            filled.push_back(addr + ": " + cell.product + "(" + to_string(cell.quantity) + ")");
        } else {
            empty.push_back(addr);
        }
    }

    cout << fixed << setprecision(2);
    cout << "Загруженность склада: " << (double)totalStored / totalCap * 100 << "%" << endl;
    cout << endl;
    
    for (const char z : zones) {
        cout << "Загруженность зоны " << z << ": " 
             << (double)zoneStored[z] / zoneCap[z] * 100 << "%" << endl;
    }

    cout << endl;
    cout << "Заполненные ячейки:" << endl;
    if (filled.empty()) {
        cout << "  (нет)" << endl;
    } else {
        for (const string& info : filled) {
            cout << "  " << info << endl;
        }
    }

    cout << endl;
    cout << "Пустые ячейки:" << endl;
    if (empty.empty()) {
        cout << "  (нет)" << endl;
    } else {
        int count = 0;
        for (const string& addr : empty) {
            if (count >= 30) {
                cout << endl << "  ... и ещё " << (empty.size() - 30) << endl;
                break;
            }
            cout << "  " << addr;
            if ((count + 1) % 5 == 0) {
                cout << endl;
            } else {
                cout << ", ";
            }
            ++count;
        }
    }
}
#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <map>
#include <vector>
#include <clocale>

using namespace std;

const int MAX_PER_CELL = 10;
const int ZONES_COUNT = 3;
const int RACKS_COUNT = 17;
const int SECTIONS_COUNT = 4;
const int SHELVES_COUNT = 3;

struct Cell {
    string product;
    int quantity = 0;
    bool isEmpty() const { return quantity == 0; }
};

void initWarehouse(map<string, Cell>& wh);
bool isValidAddress(const string& addr);
void addProduct(map<string, Cell>& wh, const string& prod, int qty, const string& addr);
void removeProduct(map<string, Cell>& wh, const string& prod, int qty, const string& addr);
void showInfo(const map<string, Cell>& wh);

#endif
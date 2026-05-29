// Задание 1.3: Клеточный автомат Джона Конвея (Game of Life)
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <utility>
#include <windows.h>

using namespace std;

// Кроссплатформенная очистка консоли
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Вывод сетки (1 - живая, 0 - мёртвая)
void printGrid(const vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int cell : row) cout << (cell ? "1 " : "0 ");
        cout << "\n";
    }
}

// Подсчёт соседей (ограниченная плоскость)
int countNeighbors(const vector<vector<int>>& g, int r, int c) {
    int cnt = 0;
    for (int dr = -1; dr <= 1; ++dr)
        for (int dc = -1; dc <= 1; ++dc)
            if (dr || dc) {
                int nr = r + dr, nc = c + dc;
                if (nr >= 0 && nr < g.size() && nc >= 0 && nc < g[0].size())
                    cnt += g[nr][nc];
            }
    return cnt;
}

// Расчёт следующего поколения по классическим правилам
vector<vector<int>> nextGeneration(const vector<vector<int>>& g) {
    int R = g.size(), C = g[0].size();
    vector<vector<int>> ng(R, vector<int>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            int n = countNeighbors(g, r, c);
            if (g[r][c]) ng[r][c] = (n == 2 || n == 3) ? 1 : 0; // Выживание
            else        ng[r][c] = (n == 3) ? 1 : 0;             // Рождение
        }
    return ng;
}

void placeFigure(vector<vector<int>>& g, const vector<pair<int,int>>& cells, int r0, int c0) {
    for (const auto& p : cells) {
        int nr = r0 + p.first, nc = c0 + p.second;
        if (nr >= 0 && nr < g.size() && nc >= 0 && nc < g[0].size())
            g[nr][nc] = 1;
    }
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");
    srand(static_cast<unsigned>(time(nullptr)));

    const int R = 12, C = 24;
    vector<vector<int>> grid(R, vector<int>(C, 0));

    vector<pair<int,int>> block   = {{0,0},{0,1},{1,0},{1,1}};
    vector<pair<int,int>> beehive = {{0,1},{0,2},{1,0},{1,3},{2,1},{2,2}};
    vector<pair<int,int>> loaf    = {{0,1},{0,2},{1,0},{1,3},{2,1},{2,3},{3,2}};
    vector<pair<int,int>> boat    = {{0,0},{0,1},{1,0},{1,2},{2,1}};


    placeFigure(grid, block,   1, 2);
    placeFigure(grid, beehive, 1, 14);
    placeFigure(grid, loaf,    6, 2);
    placeFigure(grid, boat,    6, 14);

    cout << "Устойчивые фигуры (Block, Beehive, Loaf, Boat). Поколение 0:\n";
    printGrid(grid);
    cout << "\nПроверка 3 шагов (состояние не изменится):\n";
    for (int step = 0; step < 3; ++step) {
        grid = nextGeneration(grid);
        printGrid(grid);
        cout << "---\n";
    }

    cout << "\nСлучайная колония (20 поколений, анимация в консоли):\n";
    grid.assign(R, vector<int>(C, 0));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            if (rand() % 3 == 0) grid[r][c] = 1; 

    for (int step = 0; step < 20; ++step) {
        clearScreen();
        cout << "Поколение " << step + 1 << " | Живых: ";
        int alive = 0;
        for (const auto& row : grid) for (int v : row) alive += v;
        cout << alive << "\n\n";
        
        printGrid(grid);
        Sleep(250); // 
        grid = nextGeneration(grid);
    }

    return 0;
}
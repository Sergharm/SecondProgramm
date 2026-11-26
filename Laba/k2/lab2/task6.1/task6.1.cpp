//   task6.1.exe linear < команды.txt     линейное апробирование
//   task6.1.exe double < команды.txt     двойное хеширование
//   task6.1.exe bench 100000              эмпирический анализ обоих методов
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include "hashtable.h"
#include <locale>

using namespace std;
using Clock = chrono::steady_clock;

static void runCommands(OpenTable& t) {
    string line;
    while (getline(cin, line)) {
        istringstream is(line);
        string cmd, key, value;
        is >> cmd >> key >> value;
        if (cmd == "PUT" && !value.empty()) {
            t.Put(key, value);
            cout << "-> " << value << endl;
        } else if (cmd == "GET" && !key.empty()) {
            string v;
            if (t.Get(key, v)) cout << "-> " << v << endl;
            else cout << "-> -1" << endl;
        } else if (cmd == "DEL" && !key.empty()) {
            if (t.Delete(key)) cout << "-> " << key << endl;
            else cout << "Ошибка: ключа нет" << endl;
        } else if (!cmd.empty()) {
            cout << "Ошибка: неверная команда: " << line << endl;
        }
    }
}

static double ms(Clock::duration d) {
    return chrono::duration<double, milli>(d).count();
}

static void bench(int n) {
    mt19937 rng(1);
    uniform_int_distribution<int> dist(0, 999999999);
    vector<string> keys;
    unordered_set<string> seen;
    while ((int)keys.size() < n) {
        string k = to_string(dist(rng));
        if (seen.insert(k).second) keys.push_back(k);
    }

    cout << "Ключей: " << n << endl;
    cout << "Метод        вставка, мс   поиск, мс   проб (вставка)   перестроений" << endl;
    for (bool dbl : {false, true}) {
        OpenTable t(8, dbl);
        auto t0 = Clock::now();
        for (const auto& k : keys) t.Put(k, "v");
        auto ins = Clock::now() - t0;
        long long probes = t.Probes;
        t0 = Clock::now();
        string v;
        for (const auto& k : keys) t.Get(k, v);
        auto look = Clock::now() - t0;
        cout << (dbl ? "двойное     " : "линейное    ")
             << ms(ins) << "        " << ms(look) << "        " << probes
             << "            " << t.Rehashes << endl;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if (argc < 2) {
        cout << "Запуск: task6.1 linear|double  или  task6.1 bench N" << endl;
        return 1;
    }
    string mode = argv[1];
    if (mode == "bench") {
        bench(argc >= 3 ? atoi(argv[2]) : 100000);
        return 0;
    }
    if (mode == "linear") { OpenTable t(8, false); runCommands(t); return 0; }
    if (mode == "double") { OpenTable t(8, true);  runCommands(t); return 0; }
    cout << "Неизвестный режим: " << mode << endl;
    return 1;
}

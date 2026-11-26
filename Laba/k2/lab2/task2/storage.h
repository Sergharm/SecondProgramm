#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

// Файл данных в формате ЛР1: одна строка — одна структура: ТИП имя элемент1 элемент2 ...
inline std::vector<std::string> loadLines(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.find_first_not_of(" \t") != std::string::npos) lines.push_back(line);
    }
    return lines;
}

inline bool saveLines(const std::string& path, const std::vector<std::string>& lines) {
    std::ofstream out(path);
    if (!out) return false;
    for (const auto& l : lines) out << l << "\n";
    return true;
}

// Ищет структуру по имени: возвращает номер строки (-1 — нет), тип и элементы
inline int findName(const std::vector<std::string>& lines, const std::string& name,
                    std::string& type, std::vector<std::string>& elems) {
    for (size_t i = 0; i < lines.size(); ++i) {
        std::istringstream is(lines[i]);
        std::string t, n, w;
        is >> t >> n;
        if (n == name) {
            type = t;
            elems.clear();
            while (is >> w) elems.push_back(w);
            return (int)i;
        }
    }
    return -1;
}

// Формирует строку структуры и заменяет (idx >= 0) или добавляет (idx == -1)
inline void putLine(std::vector<std::string>& lines, int idx, const std::string& type,
                    const std::string& name, const std::vector<std::string>& elems) {
    std::string line = type + " " + name;
    for (const auto& e : elems) line += " " + e;
    if (idx == -1) lines.push_back(line);
    else lines[idx] = line;
}

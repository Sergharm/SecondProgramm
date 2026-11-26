// Пакет storage: чтение и запись файла данных в формате ЛР1.
// Одна строка — одна структура: ТИП имя элемент1 элемент2 ...
package main

import (
	"os"
	"strings"
)

// Load читает строки файла. Если файла нет — пустой список.
func Load(path string) ([]string, error) {
	data, err := os.ReadFile(path)
	if err != nil {
		if os.IsNotExist(err) {
			return []string{}, nil
		}
		return nil, err
	}
	lines := []string{}
	for _, l := range strings.Split(string(data), "\n") {
		l = strings.TrimRight(l, "\r")
		if strings.TrimSpace(l) != "" {
			lines = append(lines, l)
		}
	}
	return lines, nil
}

// Save записывает строки в файл
func Save(path string, lines []string) error {
	return os.WriteFile(path, []byte(strings.Join(lines, "\n")+"\n"), 0644)
}

// Find ищет структуру по имени: номер строки (-1 — нет), тип и элементы
func Find(lines []string, name string) (int, string, []string) {
	for i, l := range lines {
		f := strings.Fields(l)
		if len(f) >= 2 && f[1] == name {
			return i, f[0], f[2:]
		}
	}
	return -1, "", nil
}

// Put заменяет строку структуры (idx >= 0) или добавляет новую (idx == -1)
func Put(lines []string, idx int, typ, name string, elems []string) []string {
	line := strings.TrimSpace(typ + " " + name + " " + strings.Join(elems, " "))
	if idx == -1 {
		return append(lines, line)
	}
	lines[idx] = line
	return lines
}

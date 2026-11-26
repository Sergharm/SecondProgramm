// Задание 2: АТД «Множество» на основе хеш-таблицы.
// Запуск: ./set --file set.data --query "SETADD имя элемент"
// Команды: SETADD имя x, SETDEL имя x, SET_AT имя x (TRUE/FALSE), PRINT имя
// Тип структуры в файле — N (набор).
package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	fileName, query := "", ""
	for i := 1; i < len(os.Args); i++ {
		switch {
		case os.Args[i] == "--file" && i+1 < len(os.Args):
			fileName = os.Args[i+1]
			i++
		case os.Args[i] == "--query" && i+1 < len(os.Args):
			query = os.Args[i+1]
			i++
		}
	}
	words := strings.Fields(query)
	if fileName == "" || len(words) < 2 {
		fmt.Println("Запуск: ./set --file set.data --query 'SETADD имя элемент'")
		os.Exit(1)
	}
	cmd, name := words[0], words[1]
	arg := ""
	if len(words) >= 3 {
		arg = words[2]
	}

	lines, err := Load(fileName)
	if err != nil {
		fmt.Println("Ошибка чтения файла:", err)
		os.Exit(1)
	}
	idx, _, elems := Find(lines, name)
	if idx == -1 && cmd != "SETADD" {
		fmt.Println("Ошибка: множество", name, "не найдено")
		os.Exit(1)
	}

	set := New()
	for _, e := range elems {
		set.Add(e)
	}

	changed := false
	switch cmd {
	case "SETADD":
		if set.Add(arg) {
			fmt.Println("-> " + arg)
			changed = true
		} else {
			fmt.Println("Ошибка: элемент уже есть во множестве")
		}
	case "SETDEL":
		if set.Del(arg) {
			fmt.Println("-> " + arg)
			changed = true
		} else {
			fmt.Println("Ошибка: элемента нет во множестве")
		}
	case "SET_AT":
		if set.Contains(arg) {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case "PRINT":
		if set.Size() == 0 {
			fmt.Println("ПУСТОЕ")
		} else {
			fmt.Println(strings.Join(set.Items(), " "))
		}
	default:
		fmt.Println("Ошибка: неизвестная команда", cmd)
		os.Exit(1)
	}

	if changed {
		lines = Put(lines, idx, "N", name, set.Items())
		if err := Save(fileName, lines); err != nil {
			fmt.Println("Ошибка записи файла:", err)
			os.Exit(1)
		}
	}
}

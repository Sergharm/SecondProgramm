// Задание 3, вариант 3: дополнительные операции над множествами.
// Поддерживаются все команды множества из задания 2 и новые операции:
//
//	SETADD имя x      добавить элемент
//	SETDEL имя x      удалить элемент
//	SET_AT имя x      проверить наличие элемента (TRUE/FALSE)
//	PRINT имя         вывести множество
//	UNION A B C       C = A ∪ B
//	INTERSECT A B C   C = A ∩ B
//	DIFF A B C        C = A \ B
//
// Запуск: ./task3 --file set.data --query "UNION A B C"
package main

import (
	"fmt"
	"os"
	"strings"
)

// load читает множество по имени из строк файла
func load(lines []string, name string) (*Set, bool) {
	idx, _, elems := Find(lines, name)
	if idx == -1 {
		return nil, false
	}
	s := New()
	for _, e := range elems {
		s.Add(e)
	}
	return s, true
}

// store сохраняет множество под именем (заменяет строку или добавляет новую)
func store(lines []string, fileName, name string, s *Set) bool {
	idx, _, _ := Find(lines, name)
	lines = Put(lines, idx, "N", name, s.Items())
	return Save(fileName, lines) == nil
}

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
		fmt.Println("Запуск: ./task3 --file set.data --query 'SETADD A x' | 'SETDEL A x' | 'SET_AT A x' | 'PRINT A' | 'UNION A B C' | 'INTERSECT A B C' | 'DIFF A B C'")
		os.Exit(1)
	}

	lines, err := Load(fileName)
	if err != nil {
		fmt.Println("Ошибка чтения файла:", err)
		os.Exit(1)
	}
	cmd := words[0]

	// ---- команды одного множества: SETADD, SETDEL, SET_AT, PRINT ----
	switch cmd {
	case "SETADD", "SETDEL", "SET_AT", "PRINT":
		name := words[1]
		arg := ""
		if len(words) >= 3 {
			arg = words[2]
		}
		s, ok := load(lines, name)
		if cmd == "SETADD" {
			if arg == "" {
				fmt.Println("Ошибка: укажите элемент")
				os.Exit(1)
			}
			if !ok {
				s = New()
			}
			if !s.Add(arg) {
				fmt.Println("Ошибка: элемент уже есть во множестве")
				os.Exit(1)
			}
			fmt.Println("-> " + arg)
			if !store(lines, fileName, name, s) {
				fmt.Println("Ошибка записи файла")
				os.Exit(1)
			}
			return
		}
		if !ok {
			fmt.Println("Ошибка: множество", name, "не найдено")
			os.Exit(1)
		}
		switch cmd {
		case "SETDEL":
			if !s.Del(arg) {
				fmt.Println("Ошибка: элемента нет во множестве")
				os.Exit(1)
			}
			fmt.Println("-> " + arg)
			if !store(lines, fileName, name, s) {
				fmt.Println("Ошибка записи файла")
				os.Exit(1)
			}
		case "SET_AT":
			if s.Contains(arg) {
				fmt.Println("-> TRUE")
			} else {
				fmt.Println("-> FALSE")
			}
		case "PRINT":
			if s.Size() == 0 {
				fmt.Println("ПУСТОЕ")
			} else {
				fmt.Println(strings.Join(s.Items(), " "))
			}
		}
		return
	}

	// ---- операции над двумя множествами: UNION, INTERSECT, DIFF ----
	if len(words) != 4 || (cmd != "UNION" && cmd != "INTERSECT" && cmd != "DIFF") {
		fmt.Println("Ошибка: неизвестная команда или неверное число аргументов")
		os.Exit(1)
	}
	a, okA := load(lines, words[1])
	b, okB := load(lines, words[2])
	if !okA || !okB {
		fmt.Println("Ошибка: одно из множеств не найдено")
		os.Exit(1)
	}
	res := New()
	switch cmd {
	case "UNION":
		for _, x := range a.Items() {
			res.Add(x)
		}
		for _, x := range b.Items() {
			res.Add(x)
		}
	case "INTERSECT":
		for _, x := range a.Items() {
			if b.Contains(x) {
				res.Add(x)
			}
		}
	case "DIFF":
		for _, x := range a.Items() {
			if !b.Contains(x) {
				res.Add(x)
			}
		}
	}
	if !store(lines, fileName, words[3], res) {
		fmt.Println("Ошибка записи файла")
		os.Exit(1)
	}
	fmt.Println(strings.Join(res.Items(), " "))
}

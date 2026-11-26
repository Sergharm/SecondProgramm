// Лабораторная работа №1. Интерфейс работы со структурами данных (мини-СУБД).
//
// Сборка (в папке с go.mod):
//   go build -o dbms .
//
// Запуск:
//   ./dbms --file file.data --query 'КОМАНДА имя [аргументы]'
//
// Файл: одна строка — одна структура:   ТИП имя элемент1 элемент2 ...
//   ТИП — первая буква команд этой структуры (M, F, L, S, Q).
//   Стек записывается от дна к вершине, очередь — от начала к концу.
//
// Индексы считаются с 0. Результат команды выводится как "-> значение".
//
// PRINT имя                          вывести любую структуру
//
// Массив (M)
//   MCREATE имя                      создать пустой массив
//   MPUSH имя значение               добавить в конец
//   MINSERT имя индекс значение      добавить по индексу
//   MGET имя индекс                  получить по индексу
//   MDEL имя индекс                  удалить по индексу
//   MSET имя индекс значение         заменить по индексу
//   MLEN имя                         длина массива
//
// Односвязный список (F). У двусвязного (L) команды те же, только первая буква L
//   FPUSHHEAD имя значение           добавить в голову
//   FPUSHTAIL имя значение           добавить в хвост (FPUSH — то же самое)
//   FPUSHBEFORE имя индекс значение  добавить перед элементом с этим индексом
//   FPUSHAFTER имя индекс значение   добавить после элемента с этим индексом
//   FDELHEAD имя                     удалить голову
//   FDELTAIL имя                     удалить хвост
//   FDELBEFORE имя индекс            удалить элемент перед индексом
//   FDELAFTER имя индекс             удалить элемент после индекса
//   FDELVAL имя значение             удалить по значению (FDEL — то же самое)
//   FGETHEAD имя                     прочитать голову
//   FGETTAIL имя                     прочитать хвост
//   FGET имя индекс                  прочитать по индексу
//   FFIND имя значение               поиск по значению (выводит индекс)
//
// Стек (S)
//   SPUSH имя значение               положить на вершину
//   SPOP имя                         снять с вершины (выводит снятый элемент)
//   SPEEK имя                        посмотреть вершину, не снимая
//
// Очередь (Q)
//   QPUSH имя значение               добавить в конец
//   QPOP имя                         забрать из начала (выводит элемент)
//   QPEEK имя                        посмотреть первый, не забирая

package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

// проверка, что строка — целое число (индекс)
func isNumber(s string) bool {
	length := len(s)
	start := 0
	if length > 0 && s[0] == '-' {
		start = 1
	}
	// хотя бы одна цифра и не больше 9, чтобы число точно влезло в int
	if length-start < 1 || length-start > 9 {
		return false
	}
	for i := start; i < length; i++ {
		if s[i] < '0' || s[i] > '9' {
			return false
		}
	}
	return true
}

// строка -> индекс; -1, если это не число
func toIndex(s string) int {
	if !isNumber(s) {
		return -1
	}
	n, _ := strconv.Atoi(s)
	return n
}

// сколько слов должно быть в запросе (команда + имя + аргументы)
// -1 — такой команды нет
func wordsNeeded(cmd string) int {
	switch cmd {
	// команда и имя
	case "PRINT", "MCREATE", "MLEN",
		"FDELHEAD", "FDELTAIL", "FGETHEAD", "FGETTAIL",
		"LDELHEAD", "LDELTAIL", "LGETHEAD", "LGETTAIL",
		"SPOP", "SPEEK", "QPOP", "QPEEK":
		return 2
	// команда, имя и одно значение (или индекс)
	case "MPUSH", "MGET", "MDEL",
		"FPUSH", "FPUSHHEAD", "FPUSHTAIL",
		"FDEL", "FDELVAL", "FDELBEFORE", "FDELAFTER",
		"FGET", "FFIND",
		"LPUSH", "LPUSHHEAD", "LPUSHTAIL",
		"LDEL", "LDELVAL", "LDELBEFORE", "LDELAFTER",
		"LGET", "LFIND",
		"SPUSH", "QPUSH":
		return 3
	// команда, имя, индекс и значение
	case "MINSERT", "MSET",
		"FPUSHBEFORE", "FPUSHAFTER",
		"LPUSHBEFORE", "LPUSHAFTER":
		return 4
	}
	return -1
}

// команды добавления: если структуры с таким именем ещё нет, она создаётся
func isAddCommand(cmd string) bool {
	switch cmd {
	case "MCREATE", "MPUSH", "MINSERT",
		"FPUSH", "FPUSHHEAD", "FPUSHTAIL",
		"LPUSH", "LPUSHHEAD", "LPUSHTAIL",
		"SPUSH", "QPUSH":
		return true
	}
	return false
}

// Каждая функция do... ниже:
//   1) собирает структуру из строки data (элементы, прочитанные из файла),
//   2) выполняет команду,
//   3) возвращает новую строку data и true, если структура изменилась.

// ---------- Массив (M) ----------
func doArray(cmd, arg1, arg2, data string) (string, bool) {
	arr := NewArray()
	for _, w := range strings.Fields(data) {
		arr.PushBack(w)
	}
	changed := false

	switch cmd {
	case "MCREATE":
		fmt.Println("-> создан пустой массив")
		changed = true
	case "MPUSH":
		arr.PushBack(arg1)
		fmt.Println("->", arg1)
		changed = true
	case "MINSERT":
		indx := toIndex(arg1)
		if indx < 0 || indx > arr.Size() { // индекс == размер — вставка в конец
			fmt.Println("Ошибка: неверный индекс")
		} else {
			arr.Insert(indx, arg2)
			fmt.Println("->", arg2)
			changed = true
		}
	case "MGET":
		indx := toIndex(arg1)
		if indx < 0 || indx >= arr.Size() {
			fmt.Println("Ошибка: неверный индекс")
		} else {
			fmt.Print("-> ")
			arr.Get(indx)
		}
	case "MDEL":
		indx := toIndex(arg1)
		if indx < 0 || indx >= arr.Size() {
			fmt.Println("Ошибка: неверный индекс")
		} else {
			fmt.Println("->", arr.At(indx))
			arr.DelIndx(indx)
			changed = true
		}
	case "MSET":
		indx := toIndex(arg1)
		if indx < 0 || indx >= arr.Size() {
			fmt.Println("Ошибка: неверный индекс")
		} else {
			arr.Set(indx, arg2)
			fmt.Println("->", arg2)
			changed = true
		}
	case "MLEN":
		fmt.Println("->", arr.Size())
	case "PRINT":
		if arr.Size() == 0 {
			fmt.Println("ПУСТОЙ")
		} else {
			arr.Print()
		}
	}

	if changed {
		data = arr.ToString()
	}
	return data, changed
}

// ---------- Односвязный список (F) ----------
func doList(cmd, arg1, arg2, data string) (string, bool) {
	flist := &LinkedList{}
	for _, w := range strings.Fields(data) {
		flist.PushBack(w)
	}
	size := flist.Size()
	changed := false

	switch cmd {
	case "FPUSHHEAD":
		flist.PushFront(arg1)
		fmt.Println("->", arg1)
		changed = true
	case "FPUSHTAIL", "FPUSH":
		flist.PushBack(arg1)
		fmt.Println("->", arg1)
		changed = true
	case "FPUSHBEFORE":
		indx := toIndex(arg1)
		if indx < 0 || indx >= size {
			fmt.Println("Ошибка: неверный индекс")
		} else {
			flist.PushBefore(indx, arg2)
			fmt.Println("->", arg2)
			changed = true
		}
	case "FPUSHAFTER":
		indx := toIndex(arg1)
		if indx < 0 || indx >= size {
			fmt.Println("Ошибка: неверный индекс")
		} else {
			flist.PushAfter(indx, arg2)
			fmt.Println("->", arg2)
			changed = true
		}
	case "FDELHEAD":
		if size == 0 {
			fmt.Println("Ошибка: список пуст")
		} else {
			fmt.Println("->", flist.At(0))
			flist.DelFront()
			changed = true
		}
	case "FDELTAIL":
		if size == 0 {
			fmt.Println("Ошибка: список пуст")
		} else {
			fmt.Println("->", flist.At(size-1))
			flist.DelBack()
			changed = true
		}
	case "FDELBEFORE":
		indx := toIndex(arg1)
		if indx < 1 || indx >= size { // перед элементом indx должен быть ещё один
			fmt.Println("Ошибка: неверный индекс")
		} else {
			fmt.Println("->", flist.At(indx-1))
			flist.DelBefore(indx)
			changed = true
		}
	case "FDELAFTER":
		indx := toIndex(arg1)
		if indx < 0 || indx >= size-1 { // после элемента indx должен быть ещё один
			fmt.Println("Ошибка: неверный индекс")
		} else {
			fmt.Println("->", flist.At(indx+1))
			flist.DelAfter(indx)
			changed = true
		}
	case "FDELVAL", "FDEL":
		if flist.Find(arg1) == -1 {
			fmt.Println("Ошибка: значение не найдено")
		} else {
			flist.DelVal(arg1)
			fmt.Println("->", arg1)
			changed = true
		}
	case "FGETHEAD":
		if size == 0 {
			fmt.Println("Ошибка: список пуст")
		} else {
			fmt.Print("-> ")
			flist.GetHead()
		}
	case "FGETTAIL":
		if size == 0 {
			fmt.Println("Ошибка: список пуст")
		} else {
			fmt.Print("-> ")
			flist.GetTail()
		}
	case "FGET":
		indx := toIndex(arg1)
		if indx < 0 || indx >= size {
			fmt.Println("Ошибка: неверный индекс")
		} else {
			fmt.Print("-> ")
			flist.GetIndx(indx)
		}
	case "FFIND":
		indx := flist.Find(arg1)
		if indx == -1 {
			fmt.Println("-> не найдено")
		} else {
			fmt.Println("->", indx)
		}
	case "PRINT":
		if size == 0 {
			fmt.Println("ПУСТОЙ")
		} else {
			flist.Print()
		}
	}

	if changed {
		data = flist.ToString()
	}
	return data, changed
}

// ---------- Двусвязный список (L) ----------
func doDoublyList(cmd, arg1, arg2, data string) (string, bool) {
	dlist := &DoublyList{}
	for _, w := range strings.Fields(data) {
		dlist.PushBack(w)
	}
	size := dlist.Size()
	changed := false

	switch cmd {
	case "LPUSHHEAD":
		dlist.PushFront(arg1)
		fmt.Println("->", arg1)
		changed = true
	case "LPUSHTAIL", "LPUSH":
		dlist.PushBack(arg1)
		fmt.Println("->", arg1)
		changed = true
	case "LPUSHBEFORE":
		indx := toIndex(arg1)
		if indx < 0 || indx >= size {
			fmt.Println("Ошибка: неверный индекс")
		} else {
			dlist.PushBefore(indx, arg2)
			fmt.Println("->", arg2)
			changed = true
		}
	case "LPUSHAFTER":
		indx := toIndex(arg1)
		if indx < 0 || indx >= size {
			fmt.Println("Ошибка: неверный индекс")
		} else {
			dlist.PushAfter(indx, arg2)
			fmt.Println("->", arg2)
			changed = true
		}
	case "LDELHEAD":
		if size == 0 {
			fmt.Println("Ошибка: список пуст")
		} else {
			fmt.Println("->", dlist.At(0))
			dlist.DelFront()
			changed = true
		}
	case "LDELTAIL":
		if size == 0 {
			fmt.Println("Ошибка: список пуст")
		} else {
			fmt.Println("->", dlist.At(size-1))
			dlist.DelBack()
			changed = true
		}
	case "LDELBEFORE":
		indx := toIndex(arg1)
		if indx < 1 || indx >= size { // перед элементом indx должен быть ещё один
			fmt.Println("Ошибка: неверный индекс")
		} else {
			fmt.Println("->", dlist.At(indx-1))
			dlist.DelBefore(indx)
			changed = true
		}
	case "LDELAFTER":
		indx := toIndex(arg1)
		if indx < 0 || indx >= size-1 { // после элемента indx должен быть ещё один
			fmt.Println("Ошибка: неверный индекс")
		} else {
			fmt.Println("->", dlist.At(indx+1))
			dlist.DelAfter(indx)
			changed = true
		}
	case "LDELVAL", "LDEL":
		if dlist.Find(arg1) == -1 {
			fmt.Println("Ошибка: значение не найдено")
		} else {
			dlist.DelVal(arg1)
			fmt.Println("->", arg1)
			changed = true
		}
	case "LGETHEAD":
		if size == 0 {
			fmt.Println("Ошибка: список пуст")
		} else {
			fmt.Print("-> ")
			dlist.GetHead()
		}
	case "LGETTAIL":
		if size == 0 {
			fmt.Println("Ошибка: список пуст")
		} else {
			fmt.Print("-> ")
			dlist.GetTail()
		}
	case "LGET":
		indx := toIndex(arg1)
		if indx < 0 || indx >= size {
			fmt.Println("Ошибка: неверный индекс")
		} else {
			fmt.Print("-> ")
			dlist.GetIndx(indx)
		}
	case "LFIND":
		indx := dlist.Find(arg1)
		if indx == -1 {
			fmt.Println("-> не найдено")
		} else {
			fmt.Println("->", indx)
		}
	case "PRINT":
		dlist.Print() // пустой список сам выводит "ПУСТОЙ"
	}

	if changed {
		data = dlist.ToString()
	}
	return data, changed
}

// ---------- Стек (S) ----------
func doStack(cmd, arg1, data string) (string, bool) {
	st := &Stack{}
	for _, w := range strings.Fields(data) {
		st.Push(w) // в файле элементы от дна к вершине
	}
	changed := false

	switch cmd {
	case "SPUSH":
		st.Push(arg1)
		fmt.Println("->", arg1)
		changed = true
	case "SPOP":
		if st.Size() == 0 {
			fmt.Println("Ошибка: стек пуст")
		} else {
			fmt.Print("-> ")
			st.Peek() // выводим вершину
			st.Pop()  // и удаляем её
			changed = true
		}
	case "SPEEK":
		if st.Size() == 0 {
			fmt.Println("Ошибка: стек пуст")
		} else {
			fmt.Print("-> ")
			st.Peek()
		}
	case "PRINT":
		st.Print()
	}

	if changed {
		data = st.ToString()
	}
	return data, changed
}

// ---------- Очередь (Q) ----------
func doQueue(cmd, arg1, data string) (string, bool) {
	q := &Queue{}
	for _, w := range strings.Fields(data) {
		q.Push(w) // в файле элементы от начала к концу
	}
	changed := false

	switch cmd {
	case "QPUSH":
		q.Push(arg1)
		fmt.Println("->", arg1)
		changed = true
	case "QPOP":
		if q.Size() == 0 {
			fmt.Println("Ошибка: очередь пуста")
		} else {
			fmt.Print("-> ")
			q.Peek() // выводим первый элемент
			q.Pop()  // и удаляем его
			changed = true
		}
	case "QPEEK":
		if q.Size() == 0 {
			fmt.Println("Ошибка: очередь пуста")
		} else {
			fmt.Print("-> ")
			q.Peek()
		}
	case "PRINT":
		q.Print()
	}

	if changed {
		data = q.ToString()
	}
	return data, changed
}

func main() {
	os.Exit(run())
}

// run выполняет один запрос и возвращает код завершения программы
func run() int {
	// 1. разбираем аргументы: --file имя_файла --query 'запрос'
	fileName := ""
	query := ""
	args := os.Args
	for i := 1; i < len(args); i++ {
		if args[i] == "--file" && i+1 < len(args) {
			fileName = args[i+1]
			i++
		} else if args[i] == "--query" && i+1 < len(args) {
			query = args[i+1]
			i++
		}
	}
	if fileName == "" || query == "" {
		fmt.Println("Запуск: ./dbms --file file.data --query 'КОМАНДА имя [аргументы]'")
		return 1
	}

	// 2. разбиваем запрос на слова (храним в своём массиве)
	words := NewArray()
	for _, w := range strings.Fields(query) {
		words.PushBack(w)
	}
	cmd := words.At(0)
	name := words.At(1)
	arg1 := words.At(2)
	arg2 := words.At(3)

	need := wordsNeeded(cmd)
	if need == -1 {
		fmt.Println("Ошибка: неизвестная команда", cmd)
		return 1
	}
	if words.Size() != need {
		fmt.Println("Ошибка: неверное количество аргументов для", cmd)
		return 1
	}

	// 3. читаем файл: каждая непустая строка — одна структура
	// (если файла ещё нет — считаем, что он пустой)
	lines := NewArray()
	content, err := os.ReadFile(fileName)
	if err == nil {
		for _, line := range strings.Split(string(content), "\n") {
			if line != "" {
				lines.PushBack(line)
			}
		}
	}

	// 4. ищем структуру с нужным именем
	found := -1    // номер строки в файле; -1 — такой структуры нет
	fileType := "" // тип найденной структуры
	data := ""     // её элементы через пробел
	for i := 0; i < lines.Size(); i++ {
		parts := strings.Fields(lines.At(i))
		if len(parts) >= 2 && parts[1] == name {
			found = i
			fileType = parts[0]
			data = strings.Join(parts[2:], " ")
			break
		}
	}

	// 5. тип структуры — первая буква команды (для PRINT берём тип из файла)
	if found == -1 && !isAddCommand(cmd) {
		fmt.Println("Ошибка: структура", name, "не найдена")
		return 1
	}
	structType := ""
	if cmd == "PRINT" {
		structType = fileType
	} else {
		structType = cmd[:1]
	}
	if found != -1 && fileType != structType {
		fmt.Println("Ошибка: имя", name, "уже занято структурой другого типа")
		return 1
	}
	if found != -1 && cmd == "MCREATE" {
		fmt.Println("Ошибка: массив", name, "уже существует")
		return 1
	}

	// 6. выполняем команду
	changed := false
	switch structType {
	case "M":
		data, changed = doArray(cmd, arg1, arg2, data)
	case "F":
		data, changed = doList(cmd, arg1, arg2, data)
	case "L":
		data, changed = doDoublyList(cmd, arg1, arg2, data)
	case "S":
		data, changed = doStack(cmd, arg1, data)
	case "Q":
		data, changed = doQueue(cmd, arg1, data)
	default:
		fmt.Println("Ошибка: в файле неизвестный тип структуры", structType)
		return 1
	}

	// 7. если структура изменилась — перезаписываем файл
	if changed {
		newLine := structType + " " + name
		if data != "" {
			newLine += " " + data
		}
		if found == -1 {
			lines.PushBack(newLine)
		} else {
			lines.Set(found, newLine)
		}
		out := ""
		for i := 0; i < lines.Size(); i++ {
			out += lines.At(i) + "\n"
		}
		if err := os.WriteFile(fileName, []byte(out), 0644); err != nil {
			fmt.Println("Ошибка: не удалось записать файл", fileName)
			return 1
		}
	}
	return 0
}

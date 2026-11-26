// Задание 4, вариант 3: сопоставление с паттерном.
// «?» заменяет ровно один символ, «*» — любую последовательность символов, включая пустую.
// Строка и шаблон хранятся посимвольно в Array (из ЛР1).
//
// Запуск: ./task4 --pattern "*@stud.nstu.ru" --text "meow@stud.nstu.ru"
package main

import (
	"fmt"
	"os"
)

// toArray раскладывает строку по символам в массив из ЛР1
func toArray(s string) *Array {
	a := NewArray()
	for _, r := range s {
		a.PushBack(string(r))
	}
	return a
}

// match проверяет соответствие шаблона строке.
// Жадный обход с возвратом к последней «*»: O(длина строки * длина шаблона) в худшем случае.
func match(p, s *Array) bool {
	i, j := 0, 0          // позиции в шаблоне и в строке
	starP, starS := -1, 0 // позиция последней «*» в шаблоне и соответствующая позиция в строке
	for j < s.Size() {
		switch {
		case i < p.Size() && p.At(i) == "*":
			starP = i
			starS = j
			i++ // пока пропускаем «*» (сначала она пустая)
		case i < p.Size() && (p.At(i) == "?" || p.At(i) == s.At(j)):
			i++
			j++
		case starP != -1:
			// несовпадение: «*» захватывает ещё один символ
			i = starP + 1
			starS++
			j = starS
		default:
			return false
		}
	}
	for i < p.Size() && p.At(i) == "*" {
		i++ // хвостовые «*» могут быть пустыми
	}
	return i == p.Size()
}

func main() {
	pattern, text := "", ""
	havePattern, haveText := false, false
	for i := 1; i < len(os.Args); i++ {
		switch {
		case os.Args[i] == "--pattern" && i+1 < len(os.Args):
			pattern = os.Args[i+1]
			havePattern = true
			i++
		case os.Args[i] == "--text" && i+1 < len(os.Args):
			text = os.Args[i+1]
			haveText = true
			i++
		}
	}
	if !havePattern || !haveText {
		fmt.Println("Запуск: ./task4 --pattern \"*@stud.nstu.ru\" --text \"meow@stud.nstu.ru\"")
		os.Exit(1)
	}
	if match(toArray(pattern), toArray(text)) {
		fmt.Println("Соответствует")
	} else {
		fmt.Println("Не соответствует")
	}
}

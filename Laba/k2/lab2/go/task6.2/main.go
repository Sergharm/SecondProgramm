// Задание 6.2, вариант 3: преобразование целого числа в римскую цифру.
// Решение использует хеш-таблицу из задания 6.1 (открытая адресация, двойное хеширование)
// как кэш: повторно запрошенные числа берутся из таблицы.
//
// Ввод (stdin): числа, по одному в строке. Пример: 3749 -> MMMDCCXLIX
package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

var romanTable = []struct {
	value  int
	symbol string
}{
	{1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
	{100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
	{10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"},
}

// toRoman переводит число от 1 до 3999
func toRoman(n int) string {
	var b strings.Builder
	for _, r := range romanTable {
		for n >= r.value {
			b.WriteString(r.symbol)
			n -= r.value
		}
	}
	return b.String()
}

func main() {
	cache := NewOpen(16, true)
	hits := 0

	sc := bufio.NewScanner(os.Stdin)
	for sc.Scan() {
		s := strings.TrimSpace(sc.Text())
		if s == "" {
			continue
		}
		n, err := strconv.Atoi(s)
		if err != nil || n < 1 || n > 3999 {
			fmt.Println("Ошибка: число должно быть от 1 до 3999:", s)
			continue
		}
		if r, ok := cache.Get(s); ok {
			hits++
			fmt.Println(r)
			continue
		}
		r := toRoman(n)
		cache.Put(s, r)
		fmt.Println(r)
	}
	fmt.Printf("В кэше: %d, повторных запросов из кэша: %d\n", cache.Count(), hits)
}

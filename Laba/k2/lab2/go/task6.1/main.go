// Задание 6.1, вариант 1: хеш-таблица с линейным апробированием и двойным хешированием.
//
// Режимы:
//
//	./task6 --method linear < команды.txt    (линейное апробирование)
//	./task6 --method double < команды.txt    (двойное хеширование)
//	./task6 --bench 100000                    (эмпирический анализ обоих методов)
//
// Команды (stdin, по одной в строке): PUT ключ значение, GET ключ, DEL ключ
package main

import (
	"bufio"
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"strings"
	"time"
)

func runCommands(t *OpenTable) {
	sc := bufio.NewScanner(os.Stdin)
	for sc.Scan() {
		f := strings.Fields(sc.Text())
		if len(f) == 0 {
			continue
		}
		switch {
		case f[0] == "PUT" && len(f) == 3:
			t.Put(f[1], f[2])
			fmt.Println("-> " + f[2])
		case f[0] == "GET" && len(f) == 2:
			if v, ok := t.Get(f[1]); ok {
				fmt.Println("-> " + v)
			} else {
				fmt.Println("-> -1")
			}
		case f[0] == "DEL" && len(f) == 2:
			if t.Delete(f[1]) {
				fmt.Println("-> " + f[1])
			} else {
				fmt.Println("Ошибка: ключа нет")
			}
		default:
			fmt.Println("Ошибка: неверная команда:", sc.Text())
		}
	}
}

func bench(n int) {
	rnd := rand.New(rand.NewSource(1))
	keys := make([]string, 0, n)
	seen := map[string]bool{}
	for len(keys) < n {
		k := strconv.Itoa(rnd.Intn(1000000000))
		if !seen[k] {
			seen[k] = true
			keys = append(keys, k)
		}
	}

	fmt.Printf("Ключей: %d\n", n)
	fmt.Printf("%-20s %12s %12s %12s %12s\n", "Метод", "вставка, мс", "поиск, мс", "проб", "перестр.")
	for _, dbl := range []bool{false, true} {
		name := "линейное"
		if dbl {
			name = "двойное"
		}
		t := NewOpen(8, dbl)
		t0 := time.Now()
		for _, k := range keys {
			t.Put(k, "v")
		}
		ins := time.Since(t0)
		probesAfterInsert := t.Probes
		t0 = time.Now()
		for _, k := range keys {
			t.Get(k)
		}
		look := time.Since(t0)
		fmt.Printf("%-20s %12.2f %12.2f %12d %12d\n", name, ms(ins), ms(look), probesAfterInsert, t.Rehashes)
	}
}

func ms(d time.Duration) float64 {
	return float64(d.Microseconds()) / 1000
}

func main() {
	method := ""
	benchN := 0
	for i := 1; i < len(os.Args); i++ {
		switch {
		case os.Args[i] == "--method" && i+1 < len(os.Args):
			method = os.Args[i+1]
			i++
		case os.Args[i] == "--bench" && i+1 < len(os.Args):
			benchN, _ = strconv.Atoi(os.Args[i+1])
			i++
		}
	}

	if benchN > 0 {
		bench(benchN)
		return
	}
	switch method {
	case "linear":
		runCommands(NewOpen(8, false))
	case "double":
		runCommands(NewOpen(8, true))
	default:
		fmt.Println("Запуск: ./task6 --method linear|double  или  ./task6 --bench N")
		os.Exit(1)
	}
}

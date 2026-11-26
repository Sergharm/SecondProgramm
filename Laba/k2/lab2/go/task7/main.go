// Задание 7, вариант 1: кэш LRU (LeastRecentlyUsed).
// Значения хранятся в хеш-таблице методом цепочек (задание 6),
// порядок обращений — в двусвязном списке из ЛР1: голова — самый давний, хвост — самый свежий.
//
// Ввод (stdin): сначала ёмкость cap и количество запросов Q, затем Q запросов:
//
//	SET x y  — записать значение
//	GET x    — вернуть значение или -1
//
// Вывод: значения всех GET через пробел.
package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type LRU struct {
	cap   int
	table *OpenTable
	order *DoublyList // ключи от самого давнего к самому свежему
}

// NewLRU создаёт кэш заданной ёмкости
func NewLRU(capacity int) *LRU {
	return &LRU{
		cap:   capacity,
		table: NewOpen(16, true),
		order: &DoublyList{},
	}
}

// touch переносит ключ в хвост списка (самый свежий).
// DelVal ищет ключ линейно, поэтому операция O(cap).
func (c *LRU) touch(key string) {
	c.order.DelVal(key)
	c.order.PushBack(key)
}

// Set записывает значение; при переполнении удаляет самый давний ключ
func (c *LRU) Set(key, value string) {
	if c.cap <= 0 {
		return
	}
	if _, ok := c.table.Get(key); ok {
		c.table.Put(key, value)
		c.touch(key)
		return
	}
	if c.table.Count() >= c.cap {
		oldest := c.order.At(0)
		c.order.DelFront()
		c.table.Delete(oldest)
	}
	c.table.Put(key, value)
	c.order.PushBack(key)
}

// Get возвращает значение или -1; обращение делает ключ самым свежим
func (c *LRU) Get(key string) string {
	v, ok := c.table.Get(key)
	if !ok {
		return "-1"
	}
	c.touch(key)
	return v
}

func main() {
	sc := bufio.NewScanner(os.Stdin)
	sc.Split(bufio.ScanWords)

	next := func() string {
		if sc.Scan() {
			return sc.Text()
		}
		return ""
	}

	capacity, err1 := strconv.Atoi(next())
	q, err2 := strconv.Atoi(next())
	if err1 != nil || err2 != nil {
		fmt.Println("Ошибка: в начале ввода должны быть cap и Q")
		os.Exit(1)
	}

	cache := NewLRU(capacity)
	results := []string{}
	for i := 0; i < q; i++ {
		switch op := strings.ToUpper(next()); op {
		case "SET":
			x, y := next(), next()
			cache.Set(x, y)
		case "GET":
			results = append(results, cache.Get(next()))
		default:
			fmt.Println("Ошибка: неизвестный запрос", op)
			os.Exit(1)
		}
	}
	fmt.Println(strings.Join(results, " "))
}

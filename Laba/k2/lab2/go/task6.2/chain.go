package main

import (
	"strings"
)

// sep разделяет ключ и значение внутри одной записи корзины
const sep = "\x1f"

func join(key, value string) string {
	return key + sep + value
}

func split(entry string) (string, string) {
	i := strings.Index(entry, sep)
	return entry[:i], entry[i+len(sep):]
}

// ChainTable — хеш-таблица методом цепочек.
// Корзина — односвязный список из ЛР1 (LinkedList).
type ChainTable struct {
	buckets    []LinkedList
	count      int
	hash       HashFunc
	Collisions int // сколько раз новый ключ попал в уже занятую корзину
	Rehashes   int // сколько раз таблица увеличивалась
}

// NewChain создаёт таблицу с заданным числом корзин и хеш-функцией
func NewChain(capacity int, hash HashFunc) *ChainTable {
	if capacity < 1 {
		capacity = 8
	}
	return &ChainTable{buckets: make([]LinkedList, capacity), hash: hash}
}

func (t *ChainTable) index(key string) int {
	return t.hash(key) % len(t.buckets)
}

// Get возвращает значение по ключу
func (t *ChainTable) Get(key string) (string, bool) {
	for _, e := range t.buckets[t.index(key)].Items() {
		k, v := split(e)
		if k == key {
			return v, true
		}
	}
	return "", false
}

// Put добавляет пару ключ-значение или переписывает значение существующего ключа
func (t *ChainTable) Put(key, value string) {
	b := &t.buckets[t.index(key)]
	if _, ok := t.Get(key); ok {
		for _, e := range b.Items() {
			if k, _ := split(e); k == key {
				b.DelVal(e)
				break
			}
		}
		b.PushFront(join(key, value))
		return
	}
	if b.Size() > 0 {
		t.Collisions++
	}
	b.PushFront(join(key, value)) // новый элемент — в голову корзины
	t.count++
	if t.count*4 > len(t.buckets)*3 { // заполнено больше 75 %
		t.resize(len(t.buckets) * 2)
	}
}

// Delete удаляет ключ, возвращает false, если ключа нет
func (t *ChainTable) Delete(key string) bool {
	b := &t.buckets[t.index(key)]
	for _, e := range b.Items() {
		if k, _ := split(e); k == key {
			b.DelVal(e)
			t.count--
			return true
		}
	}
	return false
}

func (t *ChainTable) resize(n int) {
	old := t.buckets
	t.buckets = make([]LinkedList, n)
	t.Rehashes++
	for i := range old {
		for _, e := range old[i].Items() {
			k, v := split(e)
			t.buckets[t.index(k)].PushFront(join(k, v))
		}
	}
}

// Count — количество ключей
func (t *ChainTable) Count() int {
	return t.count
}

// Keys — все ключи таблицы
func (t *ChainTable) Keys() []string {
	keys := []string{}
	for i := range t.buckets {
		for _, e := range t.buckets[i].Items() {
			k, _ := split(e)
			keys = append(keys, k)
		}
	}
	return keys
}

// MaxBucket — длина самой длинной цепочки (для анализа)
func (t *ChainTable) MaxBucket() int {
	max := 0
	for i := range t.buckets {
		if s := t.buckets[i].Size(); s > max {
			max = s
		}
	}
	return max
}

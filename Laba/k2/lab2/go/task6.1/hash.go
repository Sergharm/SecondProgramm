// Пакет hashtable: хеш-функции и хеш-таблицы (задание 6 и хеш-таблица для заданий 2 и 7).
package main

// HashFunc — хеш-функция: возвращает неотрицательное число
type HashFunc func(key string) int

// PolyHash — полиномиальная хеш-функция: h = h*31 + код символа
func PolyHash(key string) int {
	h := 0
	for _, c := range key {
		h = (h*31 + int(c)) & 0x7fffffff
	}
	return h
}

// altHash — вторая хеш-функция (для двойного хеширования)
func altHash(key string) int {
	h := 7
	for _, c := range key {
		h = (h*131 + int(c)) & 0x7fffffff
	}
	return h
}

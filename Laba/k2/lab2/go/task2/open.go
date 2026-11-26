package main

// Задание 6.1, вариант 1: хеш-таблица с открытой адресацией.
// Коллизии разрешаются:
//   - линейным апробированием: позиция i-й попытки (h1 + i) mod m;
//   - двойным хешированием:    позиция i-й попытки (h1 + i*h2) mod m.
// Размер таблицы — степень двойки, поэтому шаг h2 берём нечётным (он взаимно прост с m),
// и последовательность проб перебирает все ячейки.
// Когда таблица заполнена на 90 %, она пересоздаётся вдвое большего размера,
// и все ключи вставляются заново (повторное хеширование).
//
// Ячейка: "" — пусто; deletedMark — удалённый элемент (надгробие); иначе ключ\x1fзначение.

const deletedMark = "\x00"

type OpenTable struct {
	slots    *Array
	cap      int
	count    int
	double   bool // true — двойное хеширование, false — линейное апробирование
	Probes   int  // сколько ячеек проверено всего (для анализа)
	Rehashes int  // сколько раз таблица перестраивалась
}

// NewOpen создаёт таблицу; capacity — степень двойки (при других значениях округляется вверх)
func NewOpen(capacity int, doubleHashing bool) *OpenTable {
	c := 2
	for c < capacity {
		c *= 2
	}
	return &OpenTable{slots: newSlots(c), cap: c, double: doubleHashing}
}

func newSlots(n int) *Array {
	a := NewArray()
	for i := 0; i < n; i++ {
		a.PushBack("")
	}
	return a
}

// step — шаг пробирования для ключа
func (t *OpenTable) step(key string) int {
	if !t.double {
		return 1
	}
	return 1 + 2*(altHash(key)%(t.cap/2)) // нечётное число от 1 до m-1
}

// pos — позиция i-й пробы
func (t *OpenTable) pos(key string, i int) int {
	return (PolyHash(key) + i*t.step(key)) % t.cap
}

// find ищет ключ: возвращает позицию и true, если нашёл
func (t *OpenTable) find(key string) (int, bool) {
	for i := 0; i < t.cap; i++ {
		p := t.pos(key, i)
		t.Probes++
		s := t.slots.At(p)
		if s == "" {
			return -1, false // дальше ключа точно нет
		}
		if s == deletedMark {
			continue
		}
		if k, _ := split(s); k == key {
			return p, true
		}
	}
	return -1, false
}

// Get возвращает значение по ключу
func (t *OpenTable) Get(key string) (string, bool) {
	p, ok := t.find(key)
	if !ok {
		return "", false
	}
	_, v := split(t.slots.At(p))
	return v, true
}

// Put добавляет или переписывает значение
func (t *OpenTable) Put(key, value string) {
	if p, ok := t.find(key); ok {
		t.slots.Set(p, join(key, value))
		return
	}
	if (t.count+1)*10 > t.cap*9 { // заполнено больше 90 %
		t.rehash(t.cap * 2)
	}
	t.insertNew(join(key, value))
	t.count++
}

// insertNew ставит запись в первую свободную ячейку (пустую или удалённую) последовательности проб
func (t *OpenTable) insertNew(e string) {
	key, _ := split(e)
	for i := 0; i < t.cap; i++ {
		p := t.pos(key, i)
		t.Probes++
		s := t.slots.At(p)
		if s == "" || s == deletedMark {
			t.slots.Set(p, e)
			return
		}
	}
}

// Delete удаляет ключ, оставляя надгробие, чтобы не прервать цепочку проб
func (t *OpenTable) Delete(key string) bool {
	p, ok := t.find(key)
	if !ok {
		return false
	}
	t.slots.Set(p, deletedMark)
	t.count--
	return true
}

// Count — количество ключей
func (t *OpenTable) Count() int {
	return t.count
}

// rehash создаёт таблицу нового размера и заново вставляет все ключи
func (t *OpenTable) rehash(newCap int) {
	t.Rehashes++
	live := []string{}
	for i := 0; i < t.slots.Size(); i++ {
		if s := t.slots.At(i); s != "" && s != deletedMark {
			live = append(live, s)
		}
	}
	t.cap = newCap
	t.slots = newSlots(newCap)
	for _, e := range live {
		t.insertNew(e)
	}
}

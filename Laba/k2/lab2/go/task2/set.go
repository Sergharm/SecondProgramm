// Пакет setadt: АТД «Множество» (задание 2).
// Построено на хеш-таблице методом цепочек: все операции O(1) в среднем.
package main

// Set — множество строк без повторов
type Set struct {
	t *ChainTable
}

// New создаёт пустое множество
func New() *Set {
	return &Set{t: NewChain(8, PolyHash)}
}

// Add — добавление элемента (SETADD). false, если элемент уже есть
func (s *Set) Add(x string) bool {
	if s.Contains(x) {
		return false
	}
	s.t.Put(x, "")
	return true
}

// Del — удаление элемента (SETDEL). false, если элемента не было
func (s *Set) Del(x string) bool {
	return s.t.Delete(x)
}

// Contains — проверка наличия элемента (SET_AT)
func (s *Set) Contains(x string) bool {
	_, ok := s.t.Get(x)
	return ok
}

// Size — количество элементов
func (s *Set) Size() int {
	return s.t.Count()
}

// Items — все элементы (порядок не гарантирован)
func (s *Set) Items() []string {
	return s.t.Keys()
}

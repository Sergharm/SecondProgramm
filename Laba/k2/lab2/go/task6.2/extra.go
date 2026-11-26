package main

// Вспомогательные методы для лабораторной работы №2.
// Они не выводят ничего на экран, а возвращают данные — это нужно хеш-таблицам и алгоритмам.

// Items — элементы списка от головы к хвосту
func (l *LinkedList) Items() []string {
	res := []string{}
	for t := l.head; t != nil; t = t.next {
		res = append(res, t.data)
	}
	return res
}

// Items — элементы двусвязного списка от головы к хвосту
func (l *DoublyList) Items() []string {
	res := []string{}
	for t := l.head; t != nil; t = t.next {
		res = append(res, t.value)
	}
	return res
}

// Top — значение на вершине стека без вывода на экран
func (s *Stack) Top() (string, bool) {
	if s.top == nil {
		return "", false
	}
	return s.top.value, true
}

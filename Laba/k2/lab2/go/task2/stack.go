package main

import "fmt"

type stackNode struct {
	value string
	next  *stackNode
}

// Stack — стек строк на односвязном списке.
// Нулевое значение (Stack{}) — готовый пустой стек.
type Stack struct {
	top *stackNode
}

// положить на вершину
func (s *Stack) Push(value string) {
	s.top = &stackNode{value: value, next: s.top}
}

// снять с вершины
func (s *Stack) Pop() {
	if s.top == nil {
		fmt.Println("Error!")
		return
	}
	s.top = s.top.next
}

// посмотреть вершину, не снимая
func (s *Stack) Peek() {
	if s.top == nil {
		fmt.Println("Пуст")
		return
	}
	fmt.Println(s.top.value)
}

// вывод от вершины ко дну, по одному элементу в строке
func (s *Stack) Print() {
	if s.top == nil {
		fmt.Println("Пустой")
		return
	}
	temp := s.top
	for temp != nil {
		fmt.Println(temp.value)
		temp = temp.next
	}
}

// количество элементов
func (s *Stack) Size() int {
	count := 0
	temp := s.top
	for temp != nil {
		count++
		temp = temp.next
	}
	return count
}

// элементы через пробел от дна к вершине — для записи в файл
func (s *Stack) ToString() string {
	res := ""
	temp := s.top
	for temp != nil {
		if temp == s.top {
			res = temp.value
		} else {
			res = temp.value + " " + res
		}
		temp = temp.next
	}
	return res
}

package main

import "fmt"

type listNode struct {
	data string
	next *listNode
}

// LinkedList — односвязный список строк.
// Нулевое значение (LinkedList{}) — готовый пустой список.
type LinkedList struct {
	head *listNode
	tail *listNode
}

func (l *LinkedList) PushFront(value string) {
	newHead := &listNode{data: value, next: l.head}
	l.head = newHead
	if l.tail == nil {
		l.tail = l.head
	}
}

func (l *LinkedList) PushBack(value string) {
	newTail := &listNode{data: value}
	if l.tail == nil {
		l.tail = newTail
		l.head = l.tail
	} else {
		l.tail.next = newTail
		l.tail = newTail
	}
}

func (l *LinkedList) Print() {
	temp := l.head
	for temp != nil {
		fmt.Print(temp.data + " ")
		temp = temp.next
	}
	fmt.Println()
}

// добавить элемент до элемента с индексом indx
func (l *LinkedList) PushBefore(indx int, value string) {
	if indx < 0 {
		return
	}
	if indx == 0 {
		l.PushFront(value)
		return
	}
	temp := l.head
	for i := 0; i < indx-1; i++ {
		if temp == nil {
			return
		}
		temp = temp.next
	}
	if temp == nil || temp.next == nil {
		return
	}
	newNode := &listNode{data: value, next: temp.next}
	temp.next = newNode
}

// добавить элемент после элемента с индексом indx
func (l *LinkedList) PushAfter(indx int, value string) {
	if indx < 0 {
		return
	}
	temp := l.head
	for i := 0; i < indx; i++ {
		if temp == nil {
			return
		}
		temp = temp.next
	}
	if temp == nil {
		return
	}
	if temp == l.tail {
		l.PushBack(value)
		return
	}
	newNode := &listNode{data: value, next: temp.next}
	temp.next = newNode
}

// прочитать голову
func (l *LinkedList) GetHead() {
	if l.head == nil {
		return
	}
	fmt.Println(l.head.data)
}

// прочитать хвост
func (l *LinkedList) GetTail() {
	if l.tail == nil {
		return
	}
	fmt.Println(l.tail.data)
}

// прочитать по indx
func (l *LinkedList) GetIndx(indx int) {
	if indx < 0 {
		return
	}
	temp := l.head
	for i := 0; i < indx; i++ {
		if temp == nil {
			return
		}
		temp = temp.next
	}
	if temp == nil {
		return
	}
	fmt.Println(temp.data)
}

// удалить по значению (первое вхождение)
func (l *LinkedList) DelVal(value string) {
	if l.head == nil {
		return
	}
	if l.head.data == value {
		l.head = l.head.next
		if l.head == nil {
			l.tail = nil
		}
		return
	}
	temp := l.head
	for temp.next != nil && temp.next.data != value {
		temp = temp.next
	}
	if temp.next == nil {
		return
	}
	delNode := temp.next
	temp.next = delNode.next
	if delNode == l.tail {
		l.tail = temp
	}
}

// поиск элемента по значению (выводит индекс)
func (l *LinkedList) GetVal(value string) {
	temp := l.head
	indx := 0
	for temp != nil {
		if temp.data == value {
			fmt.Println(indx)
			return
		}
		temp = temp.next
		indx++
	}
}

// удалить голову
func (l *LinkedList) DelFront() {
	if l.head == nil {
		return
	}
	l.head = l.head.next
	if l.head == nil {
		l.tail = nil
	}
}

// удалить хвост
func (l *LinkedList) DelBack() {
	if l.head == nil {
		return
	}
	if l.head == l.tail {
		l.head = nil
		l.tail = nil
		return
	}
	temp := l.head
	for temp.next != l.tail {
		temp = temp.next
	}
	l.tail = temp
	l.tail.next = nil
}

// удалить элемент перед indx
func (l *LinkedList) DelBefore(indx int) {
	if indx <= 0 {
		return
	}
	if indx == 1 {
		if l.head == nil || l.head.next == nil {
			return
		}
		l.DelFront()
		return
	}
	temp := l.head
	for i := 0; i < indx-2; i++ {
		if temp == nil {
			return
		}
		temp = temp.next
	}
	if temp == nil || temp.next == nil || temp.next.next == nil {
		return
	}
	delNode := temp.next
	temp.next = delNode.next
}

// удалить элемент после indx
func (l *LinkedList) DelAfter(indx int) {
	if indx < 0 {
		return
	}
	temp := l.head
	for i := 0; i < indx; i++ {
		if temp == nil {
			return
		}
		temp = temp.next
	}
	if temp == nil || temp.next == nil {
		return
	}
	delNode := temp.next
	temp.next = delNode.next
	if delNode == l.tail {
		l.tail = temp
	}
}

// количество элементов
func (l *LinkedList) Size() int {
	count := 0
	temp := l.head
	for temp != nil {
		count++
		temp = temp.next
	}
	return count
}

// все элементы через пробел — для записи в файл
func (l *LinkedList) ToString() string {
	res := ""
	temp := l.head
	for temp != nil {
		if temp != l.head {
			res += " "
		}
		res += temp.data
		temp = temp.next
	}
	return res
}

// вернуть элемент по индексу
func (l *LinkedList) At(indx int) string {
	if indx < 0 {
		return ""
	}
	temp := l.head
	for i := 0; i < indx; i++ {
		if temp == nil {
			return ""
		}
		temp = temp.next
	}
	if temp == nil {
		return ""
	}
	return temp.data
}

// вернуть индекс, -1 если не найден
func (l *LinkedList) Find(value string) int {
	temp := l.head
	indx := 0
	for temp != nil {
		if temp.data == value {
			return indx
		}
		temp = temp.next
		indx++
	}
	return -1
}

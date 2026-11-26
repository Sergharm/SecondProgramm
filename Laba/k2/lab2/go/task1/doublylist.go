package main

import "fmt"

type dListNode struct {
	value string
	next  *dListNode
	prev  *dListNode
}

// DoublyList — двусвязный список строк.
// Нулевое значение (DoublyList{}) — готовый пустой список.
type DoublyList struct {
	head *dListNode
	tail *dListNode
}

func (l *DoublyList) PushFront(val string) {
	newHead := &dListNode{value: val}
	if l.head == nil {
		l.head = newHead
		l.tail = newHead
		return
	}
	newHead.next = l.head
	l.head.prev = newHead
	l.head = newHead
}

func (l *DoublyList) PushBack(val string) {
	newTail := &dListNode{value: val}
	if l.tail == nil {
		l.head = newTail
		l.tail = newTail
		return
	}
	newTail.prev = l.tail
	l.tail.next = newTail
	l.tail = newTail
}

// добавить элемент до элемента с индексом indx
func (l *DoublyList) PushBefore(indx int, value string) {
	if indx < 0 {
		fmt.Println("Error!")
		return
	}
	if indx == 0 {
		l.PushFront(value)
		return
	}

	temp := l.head
	for i := 0; i < indx && temp != nil; i++ {
		temp = temp.next
	}
	if temp == nil {
		fmt.Println("Error!")
		return
	}

	newNode := &dListNode{value: value, prev: temp.prev, next: temp}
	temp.prev.next = newNode
	temp.prev = newNode
}

// добавить элемент после элемента с индексом indx
func (l *DoublyList) PushAfter(indx int, value string) {
	if indx < 0 {
		fmt.Println("Error!")
		return
	}

	temp := l.head
	for i := 0; i < indx && temp != nil; i++ {
		temp = temp.next
	}
	if temp == nil {
		fmt.Println("Error!")
		return
	}
	if temp == l.tail {
		l.PushBack(value)
		return
	}

	newNode := &dListNode{value: value, prev: temp, next: temp.next}
	temp.next.prev = newNode
	temp.next = newNode
}

// прочитать голову
func (l *DoublyList) GetHead() {
	if l.head == nil {
		fmt.Println("ПУСТОЙ")
		return
	}
	fmt.Println(l.head.value)
}

// прочитать хвост
func (l *DoublyList) GetTail() {
	if l.tail == nil {
		fmt.Println("ПУСТОЙ")
		return
	}
	fmt.Println(l.tail.value)
}

// прочитать по indx
func (l *DoublyList) GetIndx(indx int) {
	if indx < 0 {
		fmt.Println("Error!")
		return
	}

	temp := l.head
	for i := 0; i < indx && temp != nil; i++ {
		temp = temp.next
	}
	if temp == nil {
		fmt.Println("Error!")
		return
	}
	fmt.Println(temp.value)
}

func (l *DoublyList) Print() {
	if l.head == nil {
		fmt.Println("ПУСТОЙ")
		return
	}
	temp := l.head
	for temp != nil {
		fmt.Print(temp.value)
		if temp.next != nil {
			fmt.Print(" <-> ")
		}
		temp = temp.next
	}
	fmt.Println()
}

// удалить голову
func (l *DoublyList) DelFront() {
	if l.head == nil {
		fmt.Println("Error!")
		return
	}
	l.head = l.head.next
	if l.head == nil {
		l.tail = nil
	} else {
		l.head.prev = nil
	}
}

// удалить хвост
func (l *DoublyList) DelBack() {
	if l.tail == nil {
		fmt.Println("Error!")
		return
	}
	l.tail = l.tail.prev
	if l.tail == nil {
		l.head = nil
	} else {
		l.tail.next = nil
	}
}

// удалить элемент перед indx
func (l *DoublyList) DelBefore(indx int) {
	if indx <= 0 {
		fmt.Println("Error!")
		return
	}

	temp := l.head
	for i := 0; i < indx && temp != nil; i++ {
		temp = temp.next
	}
	if temp == nil {
		fmt.Println("Error!")
		return
	}

	target := temp.prev
	if target == l.head {
		l.DelFront()
		return
	}

	target.prev.next = temp
	temp.prev = target.prev
}

// удалить элемент после indx
func (l *DoublyList) DelAfter(indx int) {
	if indx < 0 {
		fmt.Println("Error!")
		return
	}

	temp := l.head
	for i := 0; i < indx && temp != nil; i++ {
		temp = temp.next
	}
	if temp == nil || temp.next == nil {
		fmt.Println("Error!")
		return
	}

	target := temp.next
	if target == l.tail {
		l.DelBack()
		return
	}

	temp.next = target.next
	target.next.prev = temp
}

// удалить по значению (первое вхождение)
func (l *DoublyList) DelVal(value string) {
	temp := l.head
	for temp != nil && temp.value != value {
		temp = temp.next
	}
	if temp == nil {
		fmt.Println("Error!")
		return
	}
	if temp == l.head {
		l.DelFront()
		return
	}
	if temp == l.tail {
		l.DelBack()
		return
	}

	temp.prev.next = temp.next
	temp.next.prev = temp.prev
}

// поиск элемента по значению
func (l *DoublyList) GetVal(value string) {
	indx := l.Find(value)
	if indx == -1 {
		fmt.Println("ЗНАЧЕНИЕ НЕ НАЙДЕНО")
	} else {
		fmt.Printf("ЗНАЧЕНИЕ \"%s\" ИНДЕКС %d\n", value, indx)
	}
}

// количество элементов
func (l *DoublyList) Size() int {
	count := 0
	temp := l.head
	for temp != nil {
		count++
		temp = temp.next
	}
	return count
}

// вернуть элемент по индексу
func (l *DoublyList) At(indx int) string {
	if indx < 0 {
		fmt.Println("Error!")
		return ""
	}

	temp := l.head
	for i := 0; i < indx && temp != nil; i++ {
		temp = temp.next
	}
	if temp == nil {
		fmt.Println("Error!")
		return ""
	}
	return temp.value
}

// вернуть индекс, -1 если не найден
func (l *DoublyList) Find(value string) int {
	temp := l.head
	indx := 0
	for temp != nil {
		if temp.value == value {
			return indx
		}
		temp = temp.next
		indx++
	}
	return -1
}

// все элементы через пробел — для записи в файл
func (l *DoublyList) ToString() string {
	result := ""
	temp := l.head
	for temp != nil {
		result += temp.value
		if temp.next != nil {
			result += " "
		}
		temp = temp.next
	}
	return result
}

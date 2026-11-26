package main

import "fmt"

type queueNode struct {
	value string
	next  *queueNode
}

// Queue — очередь строк на односвязном списке.
// Нулевое значение (Queue{}) — готовая пустая очередь.
type Queue struct {
	head *queueNode // начало очереди — отсюда забираем
	tail *queueNode // конец очереди — сюда добавляем
}

// добавить в конец
func (q *Queue) Push(value string) {
	newTail := &queueNode{value: value}
	if q.tail == nil {
		q.head = newTail
		q.tail = newTail
		return
	}
	q.tail.next = newTail
	q.tail = newTail
}

// удалить из начала
func (q *Queue) Pop() {
	if q.head == nil {
		fmt.Println("Error!")
		return
	}
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil // удалили последний элемент
	}
}

// прочитать первый элемент
func (q *Queue) Peek() {
	if q.head == nil {
		fmt.Println("Пуста")
		return
	}
	fmt.Println(q.head.value)
}

func (q *Queue) Print() {
	if q.head == nil {
		fmt.Println("Пустая")
		return
	}
	temp := q.head
	for temp != nil {
		fmt.Print(temp.value + " ")
		temp = temp.next
	}
	fmt.Println()
}

// количество элементов
func (q *Queue) Size() int {
	count := 0
	temp := q.head
	for temp != nil {
		count++
		temp = temp.next
	}
	return count
}

// элементы через пробел от начала к концу — для записи в файл
func (q *Queue) ToString() string {
	res := ""
	temp := q.head
	for temp != nil {
		if temp != q.head {
			res += " "
		}
		res += temp.value
		temp = temp.next
	}
	return res
}

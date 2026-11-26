package main

import "fmt"

// Array — динамический массив строк.
// Память выделяется блоком, при переполнении блок увеличивается в 2 раза.
type Array struct {
	data        []string
	isUsed      int // записано элементов
	highlighted int // под сколько элементов выделено
}

// NewArray создаёт пустой массив
func NewArray() *Array {
	a := &Array{}
	a.Create()
	return a
}

func (a *Array) Create() {
	a.highlighted = 10
	a.data = make([]string, a.highlighted)
	a.isUsed = 0
}

// увеличиваем, если переполнен
func (a *Array) increaseArray() {
	if a.highlighted == 0 {
		a.highlighted = 10
	} else {
		a.highlighted *= 2
	}
	newData := make([]string, a.highlighted)
	for i := 0; i < a.isUsed; i++ {
		newData[i] = a.data[i]
	}
	a.data = newData
}

func (a *Array) PushBack(value string) {
	if a.isUsed == a.highlighted {
		a.increaseArray()
	}
	a.data[a.isUsed] = value
	a.isUsed++
}

func (a *Array) Print() {
	for i := 0; i < a.isUsed; i++ {
		fmt.Print(a.data[i] + " ")
	}
	fmt.Println()
}

// добавление по индексу
func (a *Array) Insert(indx int, value string) {
	if a.isUsed == a.highlighted {
		a.increaseArray()
	}
	if a.isUsed < indx || indx < 0 {
		return
	}
	for i := a.isUsed; indx < i; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[indx] = value
	a.isUsed++
}

// длина массива
func (a *Array) Length() {
	fmt.Println("Длина массива:", a.isUsed)
}

// получение по индексу (с выводом)
func (a *Array) Get(indx int) {
	if indx < 0 || indx >= a.isUsed {
		return
	}
	fmt.Println(a.data[indx])
}

// удаление по индексу
func (a *Array) DelIndx(indx int) {
	if indx < 0 || indx >= a.isUsed {
		return
	}
	for i := indx; i < a.isUsed-1; i++ {
		a.data[i] = a.data[i+1]
	}
	a.isUsed--
	a.data[a.isUsed] = "" // освобождаем последнюю ячейку
}

// замена по индексу
func (a *Array) Set(indx int, value string) {
	if indx < 0 || indx >= a.isUsed {
		return
	}
	a.data[indx] = value
}

// количество элементов (без вывода)
func (a *Array) Size() int {
	return a.isUsed
}

// вернуть элемент по индексу (без вывода)
func (a *Array) At(indx int) string {
	if indx < 0 || indx >= a.isUsed {
		return ""
	}
	return a.data[indx]
}

// все элементы через пробел — для записи в файл
func (a *Array) ToString() string {
	res := ""
	for i := 0; i < a.isUsed; i++ {
		if i > 0 {
			res += " "
		}
		res += a.data[i]
	}
	return res
}

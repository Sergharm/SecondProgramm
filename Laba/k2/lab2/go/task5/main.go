// Задание 5, вариант 3: АВЛ-сбалансированность.
// Вводится последовательность целых чисел, оканчивающаяся нулём (сам ноль не входит).
// По ней строится бинарное дерево поиска; выводится YES, если для каждой вершины
// высоты левого и правого поддеревьев различаются не более чем на 1, иначе NO.
//
// Последовательность сохраняется в Array (из ЛР1).
// Дерево строится на узлах с указателями: ЛР1 содержит полное (Complete) дерево,
// которое не является деревом поиска.
//
// Ввод (stdin): 7 3 2 1 9 5 4 6 8 0   ->   YES
package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

type node struct {
	key         int
	left, right *node
}

func insert(root *node, key int) *node {
	if root == nil {
		return &node{key: key}
	}
	switch {
	case key < root.key:
		root.left = insert(root.left, key)
	case key > root.key:
		root.right = insert(root.right, key)
	}
	return root // совпадающие ключи пропускаем
}

// check возвращает высоту поддерева и признак сбалансированности всех его вершин
func check(n *node) (int, bool) {
	if n == nil {
		return 0, true
	}
	hl, okL := check(n.left)
	hr, okR := check(n.right)
	diff := hl - hr
	if diff < 0 {
		diff = -diff
	}
	h := max(hl, hr) + 1
	return h, okL && okR && diff <= 1
}

func main() {
	sc := bufio.NewScanner(os.Stdin)
	sc.Split(bufio.ScanWords)

	seq := NewArray()
	terminated := false
	for sc.Scan() {
		v, err := strconv.Atoi(sc.Text())
		if err != nil {
			fmt.Println("Ошибка: ожидаются целые числа")
			os.Exit(1)
		}
		if v == 0 {
			terminated = true
			break
		}
		seq.PushBack(strconv.Itoa(v))
	}
	if !terminated {
		fmt.Println("Ошибка: последовательность должна оканчиваться нулём")
		os.Exit(1)
	}

	var root *node
	for i := 0; i < seq.Size(); i++ {
		v, _ := strconv.Atoi(seq.At(i))
		root = insert(root, v)
	}
	if _, ok := check(root); ok {
		fmt.Println("YES")
	} else {
		fmt.Println("NO")
	}
}

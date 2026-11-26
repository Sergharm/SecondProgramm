// Задание 1, вариант 3: преобразование инфиксной записи в постфиксную (обратную польскую).
// Алгоритм сортировочной станции на стеке из ЛР1 (Stack).
// Операнды — одиночные буквы или цифры. Приоритет: * и / выше, чем + и -.
// Пример: A+B*C -> ABC*+ ; (A+B)*C -> AB+C*
// Запуск: ./task1 --query "(A+B)*C"
package main

import (
	"fmt"
	"os"
	"strings"
)

var priority = map[string]int{"+": 1, "-": 1, "*": 2, "/": 2}

// toPostfix возвращает постфиксную запись или текст ошибки
func toPostfix(expr string) (string, error) {
	out := []string{}
	ops := &Stack{}

	for _, r := range expr {
		if r == ' ' {
			continue
		}
		tok := string(r)
		switch {
		case (r >= 'a' && r <= 'z') || (r >= 'A' && r <= 'Z') || (r >= '0' && r <= '9'):
			out = append(out, tok)
		case tok == "(":
			ops.Push(tok)
		case tok == ")":
			found := false
			for {
				top, ok := ops.Top()
				if !ok {
					break
				}
				ops.Pop()
				if top == "(" {
					found = true
					break
				}
				out = append(out, top)
			}
			if !found {
				return "", fmt.Errorf("лишняя закрывающая скобка")
			}
		case priority[tok] > 0:
			for {
				top, ok := ops.Top()
				if !ok || top == "(" || priority[top] < priority[tok] {
					break
				}
				out = append(out, top)
				ops.Pop()
			}
			ops.Push(tok)
		default:
			return "", fmt.Errorf("недопустимый символ %q", tok)
		}
	}

	for {
		top, ok := ops.Top()
		if !ok {
			break
		}
		ops.Pop()
		if top == "(" {
			return "", fmt.Errorf("незакрытая скобка")
		}
		out = append(out, top)
	}
	return strings.Join(out, ""), nil
}

func main() {
	query := ""
	for i := 1; i < len(os.Args); i++ {
		if os.Args[i] == "--query" && i+1 < len(os.Args) {
			query = os.Args[i+1]
			i++
		}
	}
	if query == "" {
		fmt.Println("Запуск: ./task1 --query \"A+B*C\"")
		os.Exit(1)
	}
	res, err := toPostfix(query)
	if err != nil {
		fmt.Println("Ошибка:", err)
		os.Exit(1)
	}
	fmt.Println(res)
}

# ЛР2 — команды запуска для всех заданий

---

## Задание 1 — инфикс в постфикс

**C++**
```powershell
cd task1
g++ -std=c++17 task1.cpp stack.cpp -o task1.exe
.\task1.exe "A+B*C"
.\task1.exe "(A+B)*C"
.\task1.exe "A*(B+C)/D-E"
cd ..
```

**Go**
```powershell
go run ./task1 --query "A+B*C"
go run ./task1 --query "(A+B)*C"
```

---

## Задание 2 — множество (хеш-таблица с цепочками)

Формат запуска из условия: `--file <файл данных> --query <запрос>`.
Перед новым прогоном файл `set.data` удаляется, чтобы начать с пустого множества.

**C++**
```powershell
cd task2
g++ -std=c++17 task2.cpp set.cpp linkedList.cpp -o task2.exe
Remove-Item set.data -ErrorAction SilentlyContinue
.\task2.exe --file set.data --query "SETADD A 1"
.\task2.exe --file set.data --query "SETADD A 2"
.\task2.exe --file set.data --query "SET_AT A 1"
.\task2.exe --file set.data --query "SETDEL A 1"
.\task2.exe --file set.data --query "PRINT A"
cd ..
```
Clear-Content set.data

**Go**
```powershell
Remove-Item task2\set.data -ErrorAction SilentlyContinue
go run ./task2 --file task2\set.data --query "SETADD A 1"
go run ./task2 --file task2\set.data --query "PRINT A"
```

---

## Задание 3 — операции над множествами

Перед операциями нужны множества A и B. Они создаются командами SETADD из задания 2.

**C++**
```powershell
cd task3
copy ..\task2\set.data .
g++ -std=c++17 task3.cpp set.cpp linkedList.cpp -o task3.exe
Remove-Item set.data -ErrorAction SilentlyContinue
.\task3.exe --file set.data --query "SETADD A 1"
.\task3.exe --file set.data --query "SETADD A 2"
.\task3.exe --file set.data --query "SETADD B 2"
.\task3.exe --file set.data --query "SETADD B 3"
.\task3.exe --file set.data --query "UNION A B C"
.\task3.exe --file set.data --query "INTERSECT A B D"
.\task3.exe --file set.data --query "DIFF A B E"
.\task3.exe --file set.data --query "PRINT E"
cd ..
```

**Go**
```powershell
Remove-Item task3\set.data -ErrorAction SilentlyContinue
go run ./task3 --file task3\set.data --query "SETADD A 1"
go run ./task3 --file task3\set.data --query "SETADD A 2"
go run ./task3 --file task3\set.data --query "SETADD B 2"
go run ./task3 --file task3\set.data --query "SETADD B 3"
go run ./task3 --file task3\set.data --query "UNION A B C"
go run ./task3 --file task3\set.data --query "INTERSECT A B D"
go run ./task3 --file task3\set.data --query "DIFF A B E"
go run ./task3 --file task3\set.data --query "PRINT E"
```

---

## Задание 4 — сопоставление с шаблоном (? и *)

**C++** (аргументы: шаблон, строка)
```powershell
cd task4
g++ -std=c++17 task4.cpp array.cpp -o task4.exe
.\task4.exe "*@stud.nstu.ru" "meow@stud.nstu.ru"
.\task4.exe "h?lo" "hello"
cd ..
```

**Go** (флаги `--pattern`, `--text`)
```powershell
go run ./task4 --pattern "*@stud.nstu.ru" --text "meow@stud.nstu.ru"
go run ./task4 --pattern "h?lo" --text "hello"
```

---

## Задание 5 — проверка АВЛ-условия (последовательность до 0)

**C++**
```powershell
cd task5
g++ -std=c++17 task5.cpp avl.cpp array.cpp -o task5.exe
echo "7 3 2 1 9 5 4 6 8 0" | .\task5.exe
cd ..
```

**Go**
```powershell
echo "7 3 2 1 9 5 4 6 8 0" | go run ./task5
```

---

## Задание 6.1 — хеш-таблица (линейное и двойное хеширование)

Режим задаётся аргументом. Команды вводятся в консоль: `PUT a 1`, `GET a`, `DEL a`. Завершение: Ctrl+Z и Enter.

**C++**
```powershell
cd task6.1
g++ -std=c++17 task6.1.cpp hashtable.cpp array.cpp -o task6.1.exe
.\task6.1.exe linear
.\task6.1.exe double
.\task6.1.exe bench 100000
cd ..
```

**Go**
```powershell
go run ./task6.1 --method linear
go run ./task6.1 --method double
go run ./task6.1 --bench 100000
```

---

## Задание 6.2 — римские цифры с кэшем

Числа вводятся построчно (1–3999). Для проверки можно передать их через echo.

**C++**
```powershell
cd task6.2
g++ -std=c++17 task6.2.cpp hashtable.cpp array.cpp -o task6.2.exe
echo "3749`n4`n1994" | .\task6.2.exe
cd ..
```

**Go**
```powershell
echo "3749`n4`n1994" | go run ./task6.2
```

---

## Задание 7 — LRU-кэш

Первая строка: `ёмкость количество_запросов`. Далее запросы `SET ключ значение` и `GET ключ`.

**C++**
```powershell
cd task7
g++ -std=c++17 task7.cpp lru.cpp hashtable.cpp array.cpp doublyList.cpp -o task7.exe
"2 5`nSET a 1`nSET b 2`nGET a`nSET c 3`nGET b" | .\task7.exe
cd ..
```
Ожидаемый вывод: `1 -1`

**Go**
```powershell
"2 5`nSET a 1`nSET b 2`nGET a`nSET c 3`nGET b" | go run ./task7
```

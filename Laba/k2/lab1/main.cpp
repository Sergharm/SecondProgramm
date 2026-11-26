#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "array.h"
#include "linkedList.h"
#include "doublyList.h"
#include "stack.h"
#include "Queue.h"
#include "CBTree.h"
#include <locale>

using namespace std;

bool isNumber(const string& s) {
    int len = s.length();
    int start = 0;
    if (len > 0 && s[0] == '-') {
        start = 1;
    }
    if (len - start < 1 || len - start > 9) {
        return false;
    }
    for (int i = start; i < len; ++i) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    return true;
}


int toIndex(const string& s) {
    if (!isNumber(s)) {
        return -1;
    }
    return stoi(s);
}

// -1 — такой команды нет
int wordsNeeded(const string& cmd) {
    // команда и имя
    if (cmd == "PRINT" || cmd == "MCREATE" || cmd == "MLEN" ||
        cmd == "FDELHEAD" || cmd == "FDELTAIL" || cmd == "FGETHEAD" || cmd == "FGETTAIL" ||
        cmd == "LDELHEAD" || cmd == "LDELTAIL" || cmd == "LGETHEAD" || cmd == "LGETTAIL" ||
        cmd == "SPOP" || cmd == "SPEEK" || cmd == "QPOP" || cmd == "QPEEK" ||
        cmd == "TINORDER" || cmd == "TPREORDER" || cmd == "TPOSTORDER" || cmd == "TBFS" || cmd == "TISCOMPLETE") {
        return 2;
    }
    if (cmd == "MPUSH" || cmd == "MGET" || cmd == "MDEL" ||
        cmd == "FPUSH" || cmd == "FPUSHHEAD" || cmd == "FPUSHTAIL" ||
        cmd == "FDEL" || cmd == "FDELVAL" || cmd == "FDELBEFORE" || cmd == "FDELAFTER" ||
        cmd == "FGET" || cmd == "FFIND" ||
        cmd == "LPUSH" || cmd == "LPUSHHEAD" || cmd == "LPUSHTAIL" ||
        cmd == "LDEL" || cmd == "LDELVAL" || cmd == "LDELBEFORE" || cmd == "LDELAFTER" ||
        cmd == "LGET" || cmd == "LFIND" ||
        cmd == "SPUSH" || cmd == "QPUSH" ||
        cmd == "TINSERT" || cmd == "TGET" || cmd == "ISMEMBER") {
        return 3;
    }

    if (cmd == "MINSERT" || cmd == "MSET" ||
        cmd == "FPUSHBEFORE" || cmd == "FPUSHAFTER" ||
        cmd == "LPUSHBEFORE" || cmd == "LPUSHAFTER") {
        return 4;
    }
    return -1;
}


bool isAddCommand(const string& cmd) {
    return cmd == "MCREATE" || cmd == "MPUSH" || cmd == "MINSERT" ||
           cmd == "FPUSH" || cmd == "FPUSHHEAD" || cmd == "FPUSHTAIL" ||
           cmd == "LPUSH" || cmd == "LPUSHHEAD" || cmd == "LPUSHTAIL" ||
           cmd == "SPUSH" || cmd == "QPUSH" || cmd == "TINSERT";
}


bool doArray(const string& cmd, const string& arg1, const string& arg2, string& data) {
    Array arr;
    stringstream ss(data);
    string w;
    while (ss >> w) {
        arr.PushBack(w);
    }
    bool changed = false;

    if (cmd == "MCREATE") {
        cout << "-> создан пустой массив" << endl;
        changed = true;
    } else if (cmd == "MPUSH") {
        arr.PushBack(arg1);
        cout << "-> " << arg1 << endl;
        changed = true;
    } else if (cmd == "MINSERT") {
        int indx = toIndex(arg1);
        if (indx < 0 || indx > arr.Size()) { // индекс == размер — вставка в конец
            cout << "Ошибка: неверный индекс" << endl;
        } else {
            arr.Insert(indx, arg2);
            cout << "-> " << arg2 << endl;
            changed = true;
        }
    } else if (cmd == "MGET") {
        int indx = toIndex(arg1);
        if (indx < 0 || indx >= arr.Size()) {
            cout << "Ошибка: неверный индекс" << endl;
        } else {
            cout << "-> ";
            arr.Get(indx);
        }
    } else if (cmd == "MDEL") {
        int indx = toIndex(arg1);
        if (indx < 0 || indx >= arr.Size()) {
            cout << "Ошибка: неверный индекс" << endl;
        } else {
            cout << "-> " << arr.At(indx) << endl;
            arr.DelIndx(indx);
            changed = true;
        }
    } else if (cmd == "MSET") {
        int indx = toIndex(arg1);
        if (indx < 0 || indx >= arr.Size()) {
            cout << "Ошибка: неверный индекс" << endl;
        } else {
            arr.Set(indx, arg2);
            cout << "-> " << arg2 << endl;
            changed = true;
        }
    } else if (cmd == "MLEN") {
        cout << "-> " << arr.Size() << endl;
    } else if (cmd == "PRINT") {
        if (arr.Size() == 0) {
            cout << "ПУСТОЙ" << endl;
        } else {
            arr.Print();
        }
    }

    if (changed) {
        data = arr.ToString();
    }
    return changed;
}

bool doList(const string& cmd, const string& arg1, const string& arg2, string& data) {
    linkedList flist;
    stringstream ss(data);
    string w;
    while (ss >> w) {
        flist.pushBack(w);
    }
    int size = flist.Size();
    bool changed = false;

    if (cmd == "FPUSHHEAD") {
        flist.pushFront(arg1);
        cout << "-> " << arg1 << endl;
        changed = true;
    } else if (cmd == "FPUSHTAIL" || cmd == "FPUSH") {
        flist.pushBack(arg1);
        cout << "-> " << arg1 << endl;
        changed = true;
    } else if (cmd == "FPUSHBEFORE") {
        if (flist.Find(arg1) == -1) {
            cout << "Ошибка: значение " << arg1 << " не найдено" << endl;
        } else {
            flist.pushBefore(arg1, arg2);
            cout << "-> " << arg2 << endl;
            changed = true;
        }
    } else if (cmd == "FPUSHAFTER") {
        if (flist.Find(arg1) == -1) {
            cout << "Ошибка: значение " << arg1 << " не найдено" << endl;
        } else {
            flist.pushAfter(arg1, arg2);
            cout << "-> " << arg2 << endl;
            changed = true;
        }
    } else if (cmd == "FDELHEAD") {
        if (size == 0) {
            cout << "Ошибка: список пуст" << endl;
        } else {
            cout << "-> " << flist.At(0) << endl;
            flist.delFront();
            changed = true;
        }
    } else if (cmd == "FDELTAIL") {
        if (size == 0) {
            cout << "Ошибка: список пуст" << endl;
        } else {
            cout << "-> " << flist.At(size - 1) << endl;
            flist.delBack();
            changed = true;
        }
    } else if (cmd == "FDELBEFORE") {
        int indx = flist.Find(arg1);
        if (indx == -1) {
            cout << "Ошибка: значение " << arg1 << " не найдено" << endl;
        } else if (indx == 0) {
            cout << "Ошибка: перед " << arg1 << " нет элементов" << endl;
        } else {
            cout << "-> " << flist.At(indx - 1) << endl;
            flist.delBefore(arg1);
            changed = true;
        }
    } else if (cmd == "FDELAFTER") {
        int indx = flist.Find(arg1);
        if (indx == -1) {
            cout << "Ошибка: значение " << arg1 << " не найдено" << endl;
        } else if (indx == size - 1) {
            cout << "Ошибка: после " << arg1 << " нет элементов" << endl;
        } else {
            cout << "-> " << flist.At(indx + 1) << endl;
            flist.delAfter(arg1);
            changed = true;
        }
    } else if (cmd == "FDELVAL" || cmd == "FDEL") {
        if (flist.Find(arg1) == -1) {
            cout << "Ошибка: значение не найдено" << endl;
        } else {
            flist.DelVal(arg1);
            cout << "-> " << arg1 << endl;
            changed = true;
        }
    } else if (cmd == "FGETHEAD") {
        if (size == 0) {
            cout << "Ошибка: список пуст" << endl;
        } else {
            cout << "-> ";
            flist.GetHead();
        }
    } else if (cmd == "FGETTAIL") {
        if (size == 0) {
            cout << "Ошибка: список пуст" << endl;
        } else {
            cout << "-> ";
            flist.GetTail();
        }
    } else if (cmd == "FGET") {
        int indx = toIndex(arg1);
        if (indx < 0 || indx >= size) {
            cout << "Ошибка: неверный индекс" << endl;
        } else {
            cout << "-> ";
            flist.GetIndx(indx);
        }
    } else if (cmd == "FFIND") {
        int indx = flist.Find(arg1);
        if (indx == -1) {
            cout << "-> не найдено" << endl;
        } else {
            cout << "-> " << indx << endl;
        }
    } else if (cmd == "PRINT") {
        if (size == 0) {
            cout << "ПУСТОЙ" << endl;
        } else {
            flist.Print();
        }
    }

    if (changed) {
        data = flist.ToString();
    }
    return changed;
}

bool doDoublyList(const string& cmd, const string& arg1, const string& arg2, string& data) {
    DoublyList dlist;
    stringstream ss(data);
    string w;
    while (ss >> w) {
        dlist.pushBack(w);
    }
    int size = dlist.Size();
    bool changed = false;

    if (cmd == "LPUSHHEAD") {
        dlist.pushFront(arg1);
        cout << "-> " << arg1 << endl;
        changed = true;
    } else if (cmd == "LPUSHTAIL" || cmd == "LPUSH") {
        dlist.pushBack(arg1);
        cout << "-> " << arg1 << endl;
        changed = true;
    } else if (cmd == "LPUSHBEFORE") {
        if (dlist.Find(arg1) == -1) {
            cout << "Ошибка: значение " << arg1 << " не найдено" << endl;
        } else {
            dlist.pushBefore(arg1, arg2);
            cout << "-> " << arg2 << endl;
            changed = true;
        }
    } else if (cmd == "LPUSHAFTER") {
        if (dlist.Find(arg1) == -1) {
            cout << "Ошибка: значение " << arg1 << " не найдено" << endl;
        } else {
            dlist.pushAfter(arg1, arg2);
            cout << "-> " << arg2 << endl;
            changed = true;
        }
    } else if (cmd == "LDELHEAD") {
        if (size == 0) {
            cout << "Ошибка: список пуст" << endl;
        } else {
            cout << "-> " << dlist.At(0) << endl;
            dlist.delFront();
            changed = true;
        }
    } else if (cmd == "LDELTAIL") {
        if (size == 0) {
            cout << "Ошибка: список пуст" << endl;
        } else {
            cout << "-> " << dlist.At(size - 1) << endl;
            dlist.delBack();
            changed = true;
        }
    } else if (cmd == "LDELBEFORE") {
        int indx = dlist.Find(arg1);
        if (indx == -1) {
            cout << "Ошибка: значение " << arg1 << " не найдено" << endl;
        } else if (indx == 0) {
            cout << "Ошибка: перед " << arg1 << " нет элементов" << endl;
        } else {
            cout << "-> " << dlist.At(indx - 1) << endl;
            dlist.delBefore(arg1);
            changed = true;
        }
    } else if (cmd == "LDELAFTER") {
        int indx = dlist.Find(arg1);
        if (indx == -1) {
            cout << "Ошибка: значение " << arg1 << " не найдено" << endl;
        } else if (indx == size - 1) {
            cout << "Ошибка: после " << arg1 << " нет элементов" << endl;
        } else {
            cout << "-> " << dlist.At(indx + 1) << endl;
            dlist.delAfter(arg1);
            changed = true;
        }
    } else if (cmd == "LDELVAL" || cmd == "LDEL") {
        if (dlist.Find(arg1) == -1) {
            cout << "Ошибка: значение не найдено" << endl;
        } else {
            dlist.DelVal(arg1);
            cout << "-> " << arg1 << endl;
            changed = true;
        }
    } else if (cmd == "LGETHEAD") {
        if (size == 0) {
            cout << "Ошибка: список пуст" << endl;
        } else {
            cout << "-> ";
            dlist.GetHead();
        }
    } else if (cmd == "LGETTAIL") {
        if (size == 0) {
            cout << "Ошибка: список пуст" << endl;
        } else {
            cout << "-> ";
            dlist.GetTail();
        }
    } else if (cmd == "LGET") {
        int indx = toIndex(arg1);
        if (indx < 0 || indx >= size) {
            cout << "Ошибка: неверный индекс" << endl;
        } else {
            cout << "-> ";
            dlist.GetIndx(indx);
        }
    } else if (cmd == "LFIND") {
        int indx = dlist.Find(arg1);
        if (indx == -1) {
            cout << "-> не найдено" << endl;
        } else {
            cout << "-> " << indx << endl;
        }
    } else if (cmd == "PRINT") {
        dlist.Print(); // пустой список сам выводит "ПУСТОЙ"
    }

    if (changed) {
        data = dlist.ToString();
    }
    return changed;
}

bool doStack(const string& cmd, const string& arg1, string& data) {
    Stack st;
    stringstream ss(data);
    string w;
    while (ss >> w) {
        st.push(w); // в файле элементы от дна к вершине
    }
    bool changed = false;

    if (cmd == "SPUSH") {
        st.push(arg1);
        cout << "-> " << arg1 << endl;
        changed = true;
    } else if (cmd == "SPOP") {
        if (st.Size() == 0) {
            cout << "Ошибка: стек пуст" << endl;
        } else {
            cout << "-> ";
            st.peek(); // выводим вершину
            st.pop(); // и удаляем её
            changed = true;
        }
    } else if (cmd == "SPEEK") {
        if (st.Size() == 0) {
            cout << "Ошибка: стек пуст" << endl;
        } else {
            cout << "-> ";
            st.peek();
        }
    } else if (cmd == "PRINT") {
        st.Print();
    }

    if (changed) {
        data = st.ToString();
    }
    return changed;
}

bool doQueue(const string& cmd, const string& arg1, string& data) {
    Queue q;
    stringstream ss(data);
    string w;
    while (ss >> w) {
        q.push(w); // в файле элементы от начала к концу
    }
    bool changed = false;

    if (cmd == "QPUSH") {
        q.push(arg1);
        cout << "-> " << arg1 << endl;
        changed = true;
    } else if (cmd == "QPOP") {
        if (q.Size() == 0) {
            cout << "Ошибка: очередь пуста" << endl;
        } else {
            cout << "-> ";
            q.peek(); // выводим первый элемент
            q.pop(); // и удаляем его
            changed = true;
        }
    } else if (cmd == "QPEEK") {
        if (q.Size() == 0) {
            cout << "Ошибка: очередь пуста" << endl;
        } else {
            cout << "-> ";
            q.peek();
        }
    } else if (cmd == "PRINT") {
        q.Print();
    }

    if (changed) {
        data = q.ToString();
    }
    return changed;
}

bool doTree(const string& cmd, const string& arg1, string& data) {
    CBTree tree;
    // в файле ключи записаны по уровням — восстанавливаем дерево один в один
    if (!tree.Load(data)) {
        cout << "Ошибка: в файле повреждены данные дерева" << endl;
        return false;
    }

    int key = 0;
    if (cmd == "TINSERT" || cmd == "TGET" || cmd == "ISMEMBER") {
        if (!isNumber(arg1)) {
            cout << "Ошибка: ключ дерева должен быть целым числом" << endl;
            return false;
        }
        key = stoi(arg1);
    }
    bool changed = false;

    if (cmd == "TINSERT") {
        tree.Insert(key);
        cout << "-> " << key << endl;
        changed = true;
    } else if (cmd == "TGET" || cmd == "ISMEMBER") {
        if (tree.Contains(key)) {
            cout << "-> TRUE" << endl;
        } else {
            cout << "-> FALSE" << endl;
        }
    } else if (cmd == "TISCOMPLETE") {
        if (tree.IsComplete()) {
            cout << "-> TRUE" << endl;
        } else {
            cout << "-> FALSE" << endl;
        }
    } else if (cmd == "TINORDER") {
        tree.PrintInorder();
    } else if (cmd == "TPREORDER") {
        tree.PrintPreorder();
    } else if (cmd == "TPOSTORDER") {
        tree.PrintPostorder();
    } else if (cmd == "TBFS") {
        tree.PrintBFS();
    } else if (cmd == "PRINT") {
        tree.Print();
    }

    if (changed) {
        data = tree.ToString();
    }
    return changed;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    string fileName;
    string query;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            fileName = argv[i + 1];
            ++i;
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[i + 1];
            ++i;
        }
    }
    if (fileName.empty() || query.empty()) {
        cout << "Запуск: ./dbms --file file.data --query 'КОМАНДА имя [аргументы]'" << endl;
        return 1;
    }

    Array words;
    stringstream qs(query);
    string word;
    while (qs >> word) {
        words.PushBack(word);
    }
    string cmd = words.At(0);
    string name = words.At(1);
    string arg1 = words.At(2);
    string arg2 = words.At(3);

    int need = wordsNeeded(cmd);
    if (need == -1) {
        cout << "Ошибка: неизвестная команда " << cmd << endl;
        return 1;
    }
    if (words.Size() != need) {
        cout << "Ошибка: неверное количество аргументов для " << cmd << endl;
        return 1;
    }

    Array lines;
    ifstream fin(fileName);
    string line;
    while (getline(fin, line)) {
        if (!line.empty()) {
            lines.PushBack(line);
        }
    }
    fin.close();

    int found = -1; // номер строки в файле; -1 — такой структуры нет
    string fileType; // тип найденной структуры
    string data; // её элементы через пробел
    for (int i = 0; i < lines.Size(); ++i) {
        stringstream ls(lines.At(i));
        string t;
        string n;
        ls >> t >> n;
        if (n == name) {
            found = i;
            fileType = t;
            string w;
            while (ls >> w) {
                if (!data.empty()) {
                    data += " ";
                }
                data += w;
            }
            break;
        }
    }

    if (found == -1 && !isAddCommand(cmd)) {
        cout << "Ошибка: структура " << name << " не найдена" << endl;
        return 1;
    }
    string type;
    if (cmd == "PRINT") {
        type = fileType;
    } else if (cmd == "ISMEMBER") {
        type = "T";
    } else {
        type = cmd.substr(0, 1);
    }
    if (found != -1 && fileType != type) {
        cout << "Ошибка: имя " << name << " уже занято структурой другого типа" << endl;
        return 1;
    }
    if (found != -1 && cmd == "MCREATE") {
        cout << "Ошибка: массив " << name << " уже существует" << endl;
        return 1;
    }

    bool changed = false;
    if (type == "M") {
        changed = doArray(cmd, arg1, arg2, data);
    } else if (type == "F") {
        changed = doList(cmd, arg1, arg2, data);
    } else if (type == "L") {
        changed = doDoublyList(cmd, arg1, arg2, data);
    } else if (type == "S") {
        changed = doStack(cmd, arg1, data);
    } else if (type == "Q") {
        changed = doQueue(cmd, arg1, data);
    } else if (type == "T") {
        changed = doTree(cmd, arg1, data);
    } else {
        cout << "Ошибка: в файле неизвестный тип структуры " << type << endl;
        return 1;
    }

    if (changed) {
        string newLine = type + " " + name;
        if (!data.empty()) {
            newLine += " " + data;
        }
        if (found == -1) {
            lines.PushBack(newLine);
        } else {
            lines.Set(found, newLine);
        }
        ofstream fout(fileName);
        if (!fout.is_open()) {
            cout << "Ошибка: не удалось записать файл " << fileName << endl;
            return 1;
        }
        for (int i = 0; i < lines.Size(); ++i) {
            fout << lines.At(i) << endl;
        }
        fout.close();
    }
    return 0;
}
#pragma once
#include <iostream>
#include <string>

struct Array {
private:
    std::string* data = nullptr;
    int isUsed = 0;       // записано элементов
    int highlighted = 10; // под сколько элементов выделено
    void clear();
    void increaseArrasy(); // увеличиваем, если переполнен

public:
    ~Array();
    Array();

    // Запрещаем случайное поверхностное копирование (чтобы не было double free)
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    // Разрешаем безопасное перемещение (нужно для возврата Array из функций вроде toArray)
    Array(Array&& other) noexcept;
    Array& operator=(Array&& other) noexcept;

    void Create();
    void Print() const;
    void PushBack(const std::string& value);
    void Insert(const int& indx, const std::string& value); // добавление по индексу
    void Get(const int& indx) const; // получение по индексу
    void DelIndx(const int& indx); // удаление по индексу
    void Length() const; // длина массива
    void Set(int indx, std::string value); // замена
    int Size() const; // количество элементов (без вывода)
    std::string At(const int& indx) const; // вернуть элемент по индексу (без вывода)
    std::string ToString() const; // все элементы через пробел — для записи в файл
};

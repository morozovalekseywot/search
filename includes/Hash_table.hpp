#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <json11.hpp>
#include <cmath>

#define hash1(s, table_size) HashFunctionHorner(s, table_size, table_size - 1)
#define hash2(s, table_size) HashFunctionHorner(s, table_size, table_size + 1)
using namespace std;

// Возвращаемое значение должно быть взаимопросто с размером таблицы
inline int HashFunctionHorner(const string &s, int table_size, const int key)
{
    int hash_result = 0;
    for (int i = 0; i < s.size(); ++i)
        hash_result = (key * hash_result + s[i]) % table_size;

    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}


//struct HashFunction1
//{
//    int operator()(const std::string& s, int table_size) const
//    {
//        return HashFunctionHorner(s, table_size, table_size - 1); // ключи должны быть взаимопросты, используем числа <размер таблицы> плюс и минус один.
//    }
//};
//struct HashFunction2
//{
//    int operator()(const std::string& s, int table_size) const
//    {
//        return HashFunctionHorner(s, table_size, table_size + 1);
//    }
//};

template<typename Key, class T>
class HashTable
{
private:
//    static const int default_size = 8; // начальный размер таблицы
    constexpr static const double rehash_size = 0.75; // коэффициент, при котором произойдет увеличение таблицы

    struct Node
    {
        T value;
        Key key;
        bool state; // существует элемент или нет

        explicit Node(const Key &key, const T &value) : key(key), value(value), state(true)
        {}

        ~Node() = default;
    };

    vector<Node *> arr;
    int size; // количество существующих элементов
    int buffer_size; // arr.size()
    int size_all_non_nullptr; // количество элементов с учётом удалённых

    void resize()
    {
        buffer_size *= 2;
        size_all_non_nullptr = 0;
        size = 0;
        vector<Node *> arr2(2 * arr.size(), nullptr);
        arr.swap(arr2);
        for (int i = 0; i < arr2.size(); ++i)
        {
            if (arr2[i] && arr2[i]->state)
                addWithoutCheck(arr2[i]->key, arr2[i]->value);
        }

        for (int i = 0; i < arr2.size(); ++i)
            if (arr2[i])
                delete arr2[i];
//        delete[] arr2;
    }

    void rehash()
    {
        size_all_non_nullptr = 0;
        size = 0;
        vector<Node *> arr2(buffer_size, nullptr);
        arr.swap(arr2);
        for (int i = 0; i < arr2.size(); ++i)
        {
            if (arr2[i] && arr2[i]->state)
                addWithoutCheck(arr2[i]->key, arr2[i]->value);
        }

        for (int i = 0; i < arr2.size(); ++i)
            if (arr2[i])
                delete arr2[i];
//        delete[] arr2;
    }

    // добавляет без замены удалённых
    bool addWithoutCheck(const Key &key, const T &value)
    {
        if (size + 1 > int(rehash_size * buffer_size))
            resize();
        else if (size_all_non_nullptr > 2 * size)
            rehash();

        int h1 = hash1(getString(key), buffer_size);
        int h2 = hash2(getString(key), buffer_size);
        int i = 0;
        while (arr[h1] != nullptr && i < buffer_size)
        {
            if (arr[h1]->key == key && arr[h1]->state)
                return false;
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }
        arr[h1] = new Node(key, value);
        ++size_all_non_nullptr;
        ++size;

        return true;
    }

public:
    function<string(const Key &)> getString; // Получение строки для хэширования

    explicit HashTable(const function<string(const Key &)> &getStr = [](const Key &key)
    { return static_cast<string>(key); }) : getString(getStr)
    {
        buffer_size = 8;
        size = 0;
        size_all_non_nullptr = 0;
        arr.resize(buffer_size, nullptr);
    }

    HashTable(int size_, const function<string(const Key &)> &getStr) : getString(getStr), size(0), size_all_non_nullptr(0)
    {
        buffer_size = 1 << (32 - __builtin_clz(size_ - 1));
        arr.resize(buffer_size, nullptr);
    }

    HashTable(vector<pair<Key, T>> &vec, const function<string(const Key &)> &getStr) : getString(getStr)
    {
        int u = vec.size();
        u = 1 << (32 - __builtin_clz(u - 1)); // https://gamedev.ru/code/forum/?id=173235
        buffer_size = u;
        size = 0;
        size_all_non_nullptr = 0;
        arr.resize(buffer_size, nullptr);
        for (int i = 0; i < vec.size(); ++i)
            addWithoutCheck(vec[i].first, vec[i].second);
    }

    ~HashTable()
    {
        for (int i = 0; i < buffer_size; ++i)
            if (arr[i])
                delete arr[i];
//        delete[] arr;
    }

    // вернёт false, если элемент уже существует
    bool add(const Key &key, const T &value)
    {
        if (size + 1 > int(rehash_size * buffer_size))
            resize();
        else if (size_all_non_nullptr > 2 * size)
            rehash();

        int h1 = hash1(getString(key), buffer_size);
        int h2 = hash2(getString(key), buffer_size);
        int i = 0;
        int first_deleted = -1;
        while (arr[h1] != nullptr && i < buffer_size)
        {
            if (arr[h1]->key == key && arr[h1]->state)
                return false;
            if (!arr[h1]->state)
            {
                first_deleted = h1;
                break;
            }
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }

        if (first_deleted == -1)
        {
            arr[h1] = new Node(key, value);
            ++size_all_non_nullptr;
        } else
        {
            arr[first_deleted]->key = key;
            arr[first_deleted]->value = value;
            arr[first_deleted]->state = true;
        }
        ++size;

        return true;
    }

    bool erase(const Key &key)
    {
        int h1 = hash1(getString(key), buffer_size);
        int h2 = hash2(getString(key), buffer_size);
        int i = 0;
        while (arr[h1] != nullptr && i < buffer_size)
        {
            if (arr[h1]->key == key && arr[h1]->state)
            {
                arr[h1]->state = false;
                --size;
                return true;
            }
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }

        return false;
    }

    /// Проверяет содержит хэш-таблица элемент с таким ключом или нет
    bool contains(const Key &key) const
    {
        int h1 = hash1(getString(key), buffer_size);
        int h2 = hash2(getString(key), buffer_size);
        int i = 0;
        while (arr[h1] != nullptr && i < buffer_size)
        {
            if (arr[h1]->key == key && arr[h1]->state)
                return true;
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }

        return false;
    }

    /// возвращает значение по ключу, если не находит вернёт false первым значением в паре
    pair<bool, T> getValue(const Key &key) const
    {
        int h1 = hash1(getString(key), buffer_size);
        int h2 = hash2(getString(key), buffer_size);
        int i = 0;
        while (arr[h1] != nullptr && i < buffer_size)
        {
            if (arr[h1]->key == key && arr[h1]->state)
                return make_pair(true, arr[h1]->value);
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }

        T ans;
        return make_pair(false, ans);
    }

    /// Получить количество элементов, без учёта удалённых
    [[nodiscard]] int getNumberOfElements() const
    {
        return size;
    }


    [[nodiscard]] int getSize() const
    {
        return buffer_size;
    }

//    int hashFunction(T &value)
//    {
//        int h1 = hash1(getString(key), buffer_size);
////        int h2 = hash2(getString(key), buffer_size);
////        assert(arr[h1]!= nullptr);
//        return h1;
//    }
};


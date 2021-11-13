#pragma once

#include "../includes/Hash_table.hpp"
#include <utility>
#include <unordered_map>

template<class T>
concept is_hashable =
requires(const T &elem)
{
    { elem.getStringForHash() } -> convertible_to<string>;
    equality_comparable<T>;
};

// В хэш таблице храним индексы наших элементов из массива, и когда делаем get по ключу ищем в хэш таблице
// значение индекса и возвращаем значение в массиве по этому индексу
template<class T>
//requires is_hashable<T>
class Cache2Q
{
private:
    using Key = string;
    HashTable<Key> in; // FIFO
    HashTable<Key> out; // FIFO
public:
    unordered_map<Key, T> mainMap;
    int size_of_cache; // Размер кэша в элементах+
    int size_of_in; // 0.25 * size_of_cache
    int size_of_out; // 0.5 * size_of_cache
    int size_of_hot;  // 0.25 * size_of_cache
    HashTable<Key> hot;

    explicit Cache2Q(int maxSize)
    {
        size_of_cache = 1 << (32 - __builtin_clz(maxSize - 1));
        size_of_in = size_of_cache / 4;
        size_of_out = size_of_cache / 2;
        size_of_hot = size_of_cache / 4;

        auto strHash = [](const string &str)
        {
            return str;
        };
//        auto strHash = [](const Key &key)
//        {
//            return to_string(key);
//        };
        mainMap.reserve(size_of_cache);
        in = HashTable<Key>(size_of_in, strHash);
        out = HashTable<Key>(size_of_out, strHash);
        hot = HashTable<Key>(size_of_hot, strHash);
    }

    T get(const Key &key)
    {
        if (mainMap.contains(key))
        {
            if (hot.contains(key))
            {
                // add & trim (LRU)
                hot.erase(key);
                hot.add(key);
            } else
            {
                if (out.contains(key))
                {
                    hot.add(key);
//                    sizeHot += safeSizeOf(key, mapValue);
                    trimMapHot();
//                    sizeOut -= safeSizeOf(key, mapValue);
                    out.erase(key);
                }
            }
            return mainMap[key];
        }
    }

    T put(const Key &key, const T &value)
    {
        if (mainMap.containsKey(key))
        {
            // if already have - replace it.
            // Cache size may be overheaded at this moment
            T oldValue = mainMap.get(key);
            if (in.contains(key))
            {
                sizeIn -= safeSizeOf(key, oldValue);
                sizeIn += safeSizeOf(key, value);
            }
            if (out.contains(key))
            {
                sizeOut -= safeSizeOf(key, oldValue);
                sizeOut += safeSizeOf(key, value);
            }
            if (hot.contains(key))
            {
                sizeHot -= safeSizeOf(key, oldValue);
                sizeHot += safeSizeOf(key, value);
            }
            return mainMap.put(key, value);
        }
        T result;
//        putCount++;
//        int sizeOfValue = safeSizeOf(key, value);
        //if there are free page slots then put value into a free page slot
        bool hasFreeSlot = add2slot(key, safeSizeOf(key, value));
        if (hasFreeSlot)
        {
            // add 2 free slot & exit
            mainMap.put(key, value);
            result = value;
        } else
        {
            // no free slot, go to trim in/out
            if (trimMapIn(sizeOfValue))
            {
                //put X into the reclaimed page slot
                mainMap.put(key, value);
                result = value;
            } else
            {
                mainMap.put(key, value);
                hot.add(key);
                sizeHot += safeSizeOf(key, value);
                trimMapHot();
                result = value;
            }
        }

        return result;
    }

    void trimMapHot()
    {
        while (true)
        {
            Key key;
            T value;

            if (sizeHot <= maxSizeHot || hot.isEmpty())
            {
                break;
            }
            // we add new item before, so next return first (LRU) item
            key = hot.iterator().next();
            hot.erase(key);
            value = mainMap.get(key);
            sizeHot -= safeSizeOf(key, value);
            mainMap.remove(key);
            evictionCount++;
        }
    }

    bool add2slot(const Key &key, int sizeOfValue)
    {
        bool hasFreeSlot = false;
        if (!hasFreeSlot && size_of_in >= sizeIn + sizeOfValue)
        {
            in.add(key);
            sizeIn += sizeOfValue;
            hasFreeSlot = true;
        }
        if (!hasFreeSlot && maxSizeOut >= sizeOut + sizeOfValue)
        {
            out.add(key);
            sizeOut += sizeOfValue;
            hasFreeSlot = true;
        }
        if (!hasFreeSlot && maxSizeHot >= sizeHot + sizeOfValue)
        {
            hot.add(key);
            sizeHot += sizeOfValue;
            hasFreeSlot = true;
        }

        return hasFreeSlot;
    }

    void remove(const Key &key)
    {
        mainMap.erase(key);
        if (in.contains(key))
        {
            sizeIn -= safeSizeOf(key, previous);
            in.erase(key);
        }
        if (out.contains(key))
        {
            sizeOut -= safeSizeOf(key, previous);
            out.erase(key);
        }
        if (hot.contains(key))
        {
            sizeHot -= safeSizeOf(key, previous);
            hot.erase(key);
        }
    }
};

// Пользователь создаёт Кэш и List<Key>?? в котором будут хранится ключи для получения элементов из Кэша.
// Пользователь начинает читать файл и хочет получить одну из структур из этого файла
// Он, используя свои ключи вызывает [ bool Find(const Key &key) ] у Кэша и проверяет есть ли структура в Кеше,
// если нет, то он читает её заново из файла и затем передаёт её в метод [ Key Add(const T &value) ] у Кэша, на выход от которого получает ключ,
// под которым она хранится и по которому в дальнейшем можно её получить через [T Get(const Key &key)].
// Кэш хранит 3 Хэш таблицы со всеми структурами In(FIFO), Out(FIFO), Hot(LRU). При добавлении нового элемента он кладётся в in.
// При вызове Get сначала проверяется наличие в Hot.


/*
class OldIndexing
{
public:
    vector<Man> vec;

    void miniFill()
    {
        vec = {randomMan("Lesha", "Rus"), randomMan("Masha", "Rus"), randomMan("Gosha", "USA"), randomMan("Alina", "USA"), randomMan("Max", "Rus"),
               randomMan("Nastya", "Rus"), randomMan("Roslovtsev", "Abstract"), randomMan("500FPS", "Abstract")};
    }

    void Fill(int n)
    {
        vec.resize(n);
        for (int i = 0; i < n; i++)
            vec[i] = randomMan();
    }

    void Print()
    {
        cout << "\n{\n";
        for (auto &man: vec)
            cout << man;
        cout << "}\n";
    }

    void Write(string filename)
    {
        string path = "../" + filename;
        ofstream file(path, ios_base::trunc);
        file << "[\n";
        for (auto &man: vec)
            file << man;
        file << "]\n";
        file.close();
    }

    void Index(PriorityMan &man)
    {
        // оптимизация вставки и удаления элементов, добавляем
        // оптимально обновить индексы???
        auto comp{
                [&man](Man &a, Man &b) -> bool
                {
                    const int size = 6;
                    int8_t p[size];
                    p[man.married] = cmp(a.married, b.married);
                    p[man.age] = cmp(a.age, b.age);
                    p[man.weight] = cmp(a.weight, b.weight);
                    p[man.height] = cmp(a.height, b.height);
                    p[man.name] = cmp(a.name, b.name);
                    p[man.country] = cmp(a.country, b.country);
                    for (int i = 0; i < size; i++)
                        if (p[i] != 0)
                            return p[i] < 0;

                    return false;
                }
        };

        sort(vec.begin(), vec.end(), comp);
    }

    ~OldIndexing() = default;
};
*/
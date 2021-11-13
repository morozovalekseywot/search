
#pragma once

#include <cassert>
#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <stdexcept>
//#include <unordered_map>
#include <utility>
#include "Hash_table.hpp"

template<typename Key, typename Value>
class LruCache
{
public:
    using KeyValuePair = typename std::pair<Key, Value>;

protected:
    using List = typename std::list<KeyValuePair>;
    using ListIterator = typename List::iterator;

//    using Map = typename std::unordered_map<Key, ListIterator>;

public:
    int max_size = 0;

    LruCache(int max_size_, const function<string(const Key &)> &getStr) : max_size(max_size_)
    {
        my_map = HashTable<Key, ListIterator>(max_size, getStr);
    }

    /// Чистит Кэш
    void clear()
    {
        lru_list.clear();
        my_map.clear();
    }

    /// положить или заменить элемент в Кэшэ
    void put(const Key &key, const Value &value)
    {
        // first try to find an existing key
        auto it = my_map.getValue(key);
        if (it.first)
        {
            lru_list.erase(it.second);
            my_map.erase(key);
        }

        if (my_map.getNumberOfElements() + 1 > max_size)
            pop();
        // insert key into linked list at the front (most recently used)
        lru_list.push_front(KeyValuePair(key, value));
        // store iterator to linked list entry in map
        my_map.add(key, lru_list.begin());
    }

    /// трогает элемент в Кэше по ключу. Выдаёт иключение, если такого ключа нет в Кэше
    void touch(const Key &key)
    {
        auto it = my_map.getValue(key);
        if (!it.first)
        {
            throw std::range_error("There is no such key in cache");
        } else
        {
            // сдвигает элемент на который указывает it.second в начало
            lru_list.splice(lru_list.begin(), lru_list, it.second);
        }
    }

    /// трогает элемент в Кэше по ключу и возвращает true, если оно существует
    bool touch_if_exists(const Key &key) noexcept
    {
        auto it = my_map.getValue(key);
        if (it.first)
        {
            lru_list.splice(lru_list.begin(), lru_list, it.second);
            return true;
        }
        return false;
    }

    /// удаляет значение из Кэша
    void erase(const Key &key)
    {
        auto it = my_map.getValue(key);
        if (!it.first)
        {
            throw std::range_error("There is no such key in cache");
        } else
        {
            lru_list.erase(it.second);
            my_map.erase(key);
        }
    }

    /// удаляет значение из Кэша, если оно существует
    bool erase_if_exists(const Key &key) noexcept
    {
        auto it = my_map.getValue(key);
        if (it.first)
        {
            lru_list.erase(it.second);
            my_map.erase(key);
            return true;
        }
        return false;
    }

    /// получает значение из Кэша, не трогая его
    Value get_without_touch(const Key &key)
    {
        auto it = my_map.getValue(key);
        if (!it.first)
        {
            throw std::range_error("There is no such key in cache");
        } else
        {
            return it.second->second;
        }
    }

    /// получает и трогает значение в Кэше
    Value get(const Key &key)
    {
        auto it = my_map.getValue(key);
        if (!it.first)
        {
            throw std::range_error("There is no such key in cache");
        } else
        {
            lru_list.splice(lru_list.begin(), lru_list, it.second);
            return it.second->second;
        }
    }

    /// получает и трогает значение в Кэше, если существует
    pair<bool, Value> get_if_exists(const Key &key)
    {
        auto it = my_map.getValue(key);
        if (!it.first)
        {
            Value v;
            return make_pair(false, v);
        } else
        {
            lru_list.splice(lru_list.begin(), lru_list, it.second);
            return make_pair(true, it.second->second);
        }
    }

    /// проверяет наличие ключа в Кэше
    bool contains(const Key &key) const noexcept
    {
        return my_map.contains(key);
    }

    /// возвращает количество элементов в Кэше
    size_t size() const noexcept
    {
        return my_map.getNumberOfElements();
    }

    /// Вытесняет  last recently used элемент и вовращает удалённый элемент
    KeyValuePair pop()
    {
        assert(size());
        typename List::iterator last = lru_list.end();
        --last;
        KeyValuePair out = *last;
        my_map.erase(last->first);
        lru_list.pop_back();
        return out;
    }

    inline friend ostream &operator<<(ostream &os, const LruCache<Key, Value> &a)
    {
        for (auto it = a.lru_list.begin(); it != a.lru_list.end(); it++)
        {
            os << "Key: " << it->first << "\nValue: " << it->second << "\n";
        }

        return os;
    }

private:
    /// Лист с pair<Key,Value>, для очереди
    List lru_list;

    HashTable<Key, ListIterator> my_map;
};


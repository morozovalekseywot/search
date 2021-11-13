#pragma once

#include "Man.hpp"
#include <json11.hpp>
#include <cstdlib>
#include "Multi_Set.hpp"

using namespace json11;

template<typename T>
requires equality_comparable<T>
inline int8_t mini_cmp(const T &a, const T &b)
{
    if (a == b)
        return 0;
    if (a < b)
        return 1;
    else
        return -1;
}

class Indexing
{
private:
    MultiSet<Man> multiSet;
public:
    vector<Man> vec; // Исходные данные
    PriorityMan priorityMan; // Компаратор

//    explicit Indexing(const function<bool(const Man &, const Man &)> &cmp = [](const Man &a, const Man &b)
//    { return a < b; }) : multiSet(cmp), vec(), cmp(cmp)
//    {}
    explicit Indexing(const PriorityMan &man) : vec(), priorityMan(man)
    {
        auto comp{
                [&man](const Man &a, const Man &b) -> bool
                {
                    const int size = 6;
                    int8_t p[size];
                    p[man.married] = mini_cmp(a.married, b.married);
                    p[man.age] = mini_cmp(a.age, b.age);
                    p[man.weight] = mini_cmp(a.weight, b.weight);
                    p[man.height] = mini_cmp(a.height, b.height);
                    p[man.name] = mini_cmp(a.name, b.name);
                    p[man.country] = mini_cmp(a.country, b.country);
                    for (int i = 0; i < size; i++)
                        if (p[i] != 0)
                            return p[i] < 0;

                    return false;
                }
        };
        multiSet = MultiSet<Man>(comp);
    }

    /// заполняет n рандомными элементами и автоматически индексирует их
    void fill(int n)
    {
        vec.resize(n);
        for (int i = 0; i < n; i++)
            vec[i] = randomMan();
        multiSet = MultiSet<Man>(vec, multiSet.cmp);
    }

    void insert(const Man &man)
    {
//        vec.emplace_back(man);
        multiSet.insert(man);
    }

    void remove(const Man &man)
    {
        multiSet.remove(man);
    }

    /// Изменить порядок сортировки
    void changePriorityMan(const PriorityMan &man)
    {
        priorityMan = man;
        auto comp{
                [&man](const Man &a, const Man &b) -> bool
                {
                    const int size = 6;
                    int8_t p[size];
                    p[man.married] = mini_cmp(a.married, b.married);
                    p[man.age] = mini_cmp(a.age, b.age);
                    p[man.weight] = mini_cmp(a.weight, b.weight);
                    p[man.height] = mini_cmp(a.height, b.height);
                    p[man.name] = mini_cmp(a.name, b.name);
                    p[man.country] = mini_cmp(a.country, b.country);
                    for (int i = 0; i < size; i++)
                        if (p[i] != 0)
                            return p[i] < 0;

                    return false;
                }
        };

        multiSet.changeComparator(comp);
    }

    void readFromJsonFile(string filename)
    {
        string path = "../" + filename;
        ifstream file(path, ios_base::in);

        string buf;
        string line;
        while (getline(file, line))
            buf += line + "\n";
        file.close();
        string err;
        auto json = Json::parse(buf, err);
        if (!err.empty())
            throw runtime_error(err.c_str());

        vec.resize(json.array_items().size());
        int i = 0;
        for (auto &str_man: json.array_items())
        {
            Man man;
            man.married = str_man["Married"].bool_value();
            man.age = str_man["Age"].int_value();
            man.weight = str_man["Weight"].number_value();
            man.height = str_man["Height"].number_value();
            man.name = str_man["Name"].string_value();
            man.country = str_man["Country"].string_value();
            vec[i++] = man;
        }

        multiSet = MultiSet<Man>(vec, multiSet.cmp);
    }

    int find(Man &man) const
    {
        return multiSet.search(man);
    }

    /// Запись в файл
    void write(string filename) const
    {
        string path = "../" + filename;
//        string json_out = Json(multiSet.vec).dump();
        ofstream file(path, ios_base::trunc);
        file << "[\n";
        for (auto &man: multiSet.vec)
            file << man;
        file << "]\n";
//        file << json_out;
        file.close();
    }

    friend ostream &operator<<(ostream &os, const Indexing &b)
    {
        os << "\n{\n";
        int index = 0;
        for (auto &man: b.multiSet.vec)
            os << index++ << ": " << man;
        os << "}\n";
        return os;
    }

    ~Indexing() = default;
};

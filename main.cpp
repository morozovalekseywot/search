#include <iostream>
#include "includes/Index.hpp"
#include <chrono>
#include <cassert>
#include "includes/Hash_table.hpp"
//#include "tls_cache.hpp"
//#include "hash_map.hpp"
#include "includes/lru_cache.hpp"

void testHashTable()
{
    int n = 1000;
//    auto HashMan = [](const Man &man) -> string
//    {
//        return man.name + to_string(man.age);
//    };
    auto getStr = [](const int &n) -> string
    {
        return to_string(n);
    };

    HashTable<int, Man> table(getStr);
    vector<Man> vec(n);
    for (int i = 0; i < n; i++)
    {
        vec[i] = randomMan();
        table.add(i, vec[i]);
        assert(table.contains(i) == true);
    }

    for (int i = 0; i < n; i++)
        assert(table.getValue(i).first == true);

    for (int i = 0; i < n; i++)
        if (i % 2 == 0)
            table.erase(i);

    for (int i = 0; i < n; i++)
        if (i % 2 == 0)
            assert(table.contains(i) == false);
        else
            assert(table.contains(i) == true);

    cout << "all good\n";
}

void testLruCache()
{
    int n = 1000;
    auto getStr = [](const string &str) -> string
    {
        return str;
    };
    vector<Man> vec(n);
    for (int i = 0; i < n; i++)
        vec[i] = randomMan();

    vector<Man> mini_vec = {randomMan("Lesha", "Rus"), randomMan("Masha", "Rus"), randomMan("Gosha", "USA"), randomMan("Alina", "USA"), randomMan("Max",
                                                                                                                                                  "Rus"),
                            randomMan("Nastya", "Rus"), randomMan("Roslovtsev", "Abstract"), randomMan("500FPS", "Abstract")};

    LruCache<string, Man> cache(mini_vec.size() * 2, getStr);
    for (int i = 0; i < n; i++)
    {
        Man man = vec[i];
        if (!cache.contains(man.name))
            cache.put(man.name, man);
        else
            cache.touch(man.name);

        man = mini_vec[i % mini_vec.size()];
        if (!cache.contains(man.name))
            cache.put(man.name, man);
        else
            cache.touch(man.name);
    }

//    cache.touch("Lesha");
    cout << cache;
}

int main()
{
//    testHashTable();
    testLruCache();
//    OldIndexing obj;
//    obj.Fill(100000);
//    obj.Write("before_sort.json");
//    obj.Print();
//    PriorityMan prior = PriorityMan(0, 1, 2, 3, 4, 5);
//    auto begin = chrono::steady_clock::now();
//    obj.Index(prior);
//    auto end = chrono::steady_clock::now();
//    auto time = duration_cast<chrono::microseconds>(end - begin);
//    cout << time.count() / 1e3;
//    cout << "\n\nAfterSorted\n\n";
//    obj.Write("after_sort.json");
//    obj.Print();
//    PriorityMan prior = PriorityMan(0, 1, 2, 3, 4, 5);
//    Indexing idx(prior);
//    auto begin = chrono::steady_clock::now();
//    idx.Fill(1000);
//    auto end = chrono::steady_clock::now();
//    auto time = duration_cast<chrono::microseconds>(end - begin);
//    cout << time.count() / 1e3 << "\n";
//    auto begin1 = chrono::steady_clock::now();
//    idx.ChangePriorityMan(PriorityMan(5, 4, 3, 2, 1, 0));
//    auto end1 = chrono::steady_clock::now();
//    auto time1 = duration_cast<chrono::microseconds>(end1 - begin1);
//    cout << time.count() / 1e3 << "\n";
//    idx.ReadFromJsonFile("in.json");
//    idx.Write("out.json");
}

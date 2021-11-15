#include <iostream>
#include "includes/Index.hpp"
#include <chrono>
#include <cassert>
#include "includes/Hash_table.hpp"
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

    cout << cache;
}

void benchLruCache()
{
    int n = 10000;
    auto getStr = [](const string &str) -> string
    {
        return str;
    };
    vector<Man> vec(n);
    for (int i = 0; i < n; i++)
        vec[i] = randomMan();

    int range = n / 20;
    LruCache<string, Man> cache(range, getStr);

    std::chrono::microseconds timespan(1);
//    auto begin = chrono::steady_clock::now();
//    for (int i = 0; i < n - range; i++)
//    {
//        this_thread::sleep_for(timespan);
//    }
//    auto end = chrono::steady_clock::now();
//    auto time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Time without cache: " << timespan.count() * (n - range) << "\n";

    auto begin = chrono::steady_clock::now();
    for (int i = 0; i < n - range; i++)
    {
        int idx = i + rand() % range;
        auto ans = cache.get_if_exists(vec[idx].name);
        if (!ans.first)
        {
            cache.put(vec[idx].name, vec[idx]);
            this_thread::sleep_for(timespan);
        }
    }
    auto end = chrono::steady_clock::now();
    auto time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Time with cache: " << time.count() / 1e3 << "\n";
}

void benchFuncLruCache()
{
    int n = 10000;
    auto getStr = [](const string &str) -> string
    {
        return str;
    };
    vector<Man> vec(n);
    for (int i = 0; i < n; i++)
        vec[i] = randomMan();

    LruCache<string, Man> cache(n, getStr);

    auto begin = chrono::steady_clock::now();
    for (auto &man: vec)
        cache.put(man.getStringForHash(), man);
    auto end = chrono::steady_clock::now();
    auto time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Time cache::put: " << time.count() / 1e3 << "\n";
    cout << "Size of cache: " << cache.size() << "\n";

    begin = chrono::steady_clock::now();
    for (auto &man: vec)
        cache.contains(man.getStringForHash());
    end = chrono::steady_clock::now();
    time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Time cache::contains: " << time.count() / 1e3 << "\n";

    begin = chrono::steady_clock::now();
    for (auto &man: vec)
        cache.get_if_exists(man.getStringForHash());
    end = chrono::steady_clock::now();
    time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Time cache::get: " << time.count() / 1e3 << "\n";

    begin = chrono::steady_clock::now();
    while (cache.size() > 0)
        cache.pop();
    end = chrono::steady_clock::now();
    time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Time cache::pop: " << time.count() / 1e3 << "\n";
}

void testIndexing()
{
    int n = 10000;
    cout << "n: " << n << "\n";

    PriorityMan prior = PriorityMan(0, 1, 2, 3, 4, 5);
    Indexing idx(prior);
    auto begin = chrono::steady_clock::now();
    idx.fill(n);
    auto end = chrono::steady_clock::now();
    auto time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Indexing and fill time: " << time.count() / 1e3 << "\n";
    idx.write("first_sort.json");
    assert(idx.vec.size() == n);

    begin = chrono::steady_clock::now();
    idx.changePriorityMan(PriorityMan(5, 4, 0, 1, 2, 3)); // возраст, вес, рост
    end = chrono::steady_clock::now();
    time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Change order of indexing time: " << time.count() / 1e3 << "\n";
    idx.write("second_sort.json");

//    idx.readFromJsonFile("in.json");
//    idx.write("out.json");

    begin = chrono::steady_clock::now();
    for (int i = 0; i < idx.vec.size(); i++)
        idx.find(idx.vec[i]);
    end = chrono::steady_clock::now();
    time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Time of Indexing::find: " << time.count() / 1e3 << "\n";

    begin = chrono::steady_clock::now();
    vector<Man> vec(n / 10);
    for (int j = 0; j < n / 10; j++)
    {
        Man man = randomMan();
        idx.insert(man);
        vec[j] = man;
    }
    end = chrono::steady_clock::now();
    time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Time of Indexing::insert: " << time.count() / 1e3 << "\n";

    begin = chrono::steady_clock::now();
    for (int j = 0; j < n / 10; j++)
        idx.remove(vec[j]);
    end = chrono::steady_clock::now();
    time = duration_cast<chrono::microseconds>(end - begin);
    cout << "Time of Indexing::remove: " << time.count() / 1e3 << "\n";
}

int main()
{
//    testHashTable();
//    testLruCache();
//    benchLruCache();
//    benchFuncLruCache();
//    testIndexing();
    cout << "Enter size of cache" << endl;
    int size;
    cin >> size;
    LruCache<string, Man> cache(size, [](const string &str)
    { return str; });

    cout << "Enter size of array with struct" << endl;
    int n, index = 0;
    cin >> n;
    vector<Man> people(n);
    for (auto &man: people)
    {
        man = randomMan();
        cout << "Index: " << index++ << "\n" << man << endl;
    }

    int number;
    cout << "Enter number of request" << endl;
    cin >> number;
    for (int i = 0; i < number; i++)
    {
        int idx = -1;
        while (idx < 0 || idx > n)
        {
            cout << "Enter index in array" << endl;
            cin >> idx;
        }
        Man man = people[idx];
        cout << man;

        auto resp = cache.get_if_exists(man.getStringForHash());
        if (resp.first)
        {
            cout << "This value contains in cache under the key: " << man.getStringForHash() << endl;
        } else
        {
            cout << "This value didn't contains in cache, put this value in cache?" << endl << "1 - Yes, 2 - No" << endl;
            int put;
            cin >> put;
            if (put == 1)
            {
//                auto del = cache.pop();
//                cout << "Deleted key" << del.first << endl;
                cache.put(man.getStringForHash(), man);
            }
        }
    }

    cout << "\nValue in cache\n" << cache;
}

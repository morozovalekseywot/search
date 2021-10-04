#pragma once

#include "Man.hpp"

template<typename T>
requires equality_comparable<T>
int16_t cmp(const T &a, const T &b)
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
public:
    vector<Man> vec;

    void Fill()
    {
        vec = {randomMan("Lesha", "Rus"), randomMan("Masha", "Rus"), randomMan("Gosha", "USA"), randomMan("Alina", "USA"), randomMan("Max", "Rus"),
               randomMan("Nastya", "Rus"), randomMan("Roslovtsev", "Abstract"), randomMan("500FPS", "Abstract")};
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
        auto comp{
                [&man](Man &a, Man &b) -> bool
                {
                    const int size = 6;
                    vector<int16_t> p(size);
                    p[man.married] = cmp(a.married, b.married);
                    p[man.age] = cmp(a.age, b.age);
                    p[man.weight] = cmp(a.weight, b.weight);
                    p[man.height] = cmp(a.height, b.height);
                    p[man.name] = cmp(a.name, b.name);
                    p[man.country] = cmp(a.country, b.country);
                    for (int i = 0; i < size; i++)
                    {
                        if (p[i] != 0)
                            return p[i] < 0;
                    }
                }
        };

        sort(vec.begin(), vec.end(), comp);
    }
};

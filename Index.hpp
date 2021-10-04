#pragma once

#include <iostream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdlib>

#define fr first
#define sc second

using namespace std;

struct Man
{
    bool married;
    int age;
    double weight;
    double height;
    string name;
    string country;
};

inline std::ostream &operator<<(std::ostream &os, const Man &man)
{
    os << "{\n\"Name\": \"" << man.name << "\",\n";
    if (man.married)
        os << "\"Married\": true,\n";
    else
        os << "\"Married\": false,\n";
    os << "\"Age\": " << man.age << ",\n";
    os << "\"Weight\": " << man.weight << ",\n";
    os << "\"Height\": " << man.height << ",\n";
    os << "\"Country\": \"" << man.country << "\"\n},\n";
    return os;
}

Man randomMan(string manName, string manCountry)
{
    Man man;
    man.married = rand() % 2;
    man.age = 10 + rand() % 30;
    man.height = 160 + rand() % 35;
    man.weight = 0.4 * man.height + rand() % 5;
    man.name = manName;
    man.country = manCountry;

    return man;
}

// Пара: (значение, приоритет) Приоритет от 0 до 6
struct PriorityMan
{
//    pair<bool, int> married;
//    pair<int, int> age;
//    pair<double, int> weight;
//    pair<double, int> height;
//    pair<string, int> name;
//    pair<string, int> country;
    int married;
    int age;
    int weight;
    int height;
    int name;
    int country;

    PriorityMan(int married, int age, int weight, int height, int name, int country) : married(married), age(age), weight(weight), height(height),
                                                                                       name(name), country(country)
    {}
};

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

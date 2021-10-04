#pragma once

#include <iostream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdlib>

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

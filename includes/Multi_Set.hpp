#pragma once

#include <ostream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <iterator>
#include <functional>

using namespace std;
#define Unique(x) x.erase(unique(all(x)), x.end())
#define all(x) x.begin(), x.end()

template<class T>
class MultiSet
{
public:
    vector<T> vec;
    function<bool(const T &, const T &)> cmp;

    explicit MultiSet<T>(const function<bool(const T &, const T &)> &comp =
    [](const T &a, const T &b)
    { return a < b; }) : vec(), cmp(comp)
    {};


    explicit MultiSet(const vector<T> &Vec, const function<bool(const T &, const T &)> &comp =
    [](const T &a, const T &b)
    { return a < b; }) : vec(Vec), cmp(comp)
    {
        sort(vec.begin(), vec.end(), cmp);
    }

    MultiSet(const MultiSet<T> &s) : vec(s.vec)
    {}

    void changeComparator(const function<bool(const T &, const T &)> &comp)
    {
        cmp = comp;
        sort(all(vec), cmp);
        if (!is_sorted(vec.begin(), vec.end(), comp))
            throw runtime_error("Error");
    }

    /// Ищет элемент и возращает его индекс если находит или -1 в противном случае
    int search(const T &elem) const
    {
        if (vec.size() == 0)
            return -1;
        auto it = lower_bound(all(vec), elem, cmp);

        if (it != vec.end() && *it == elem)
            return distance(vec.begin(), vec.end());
        else
            return -1;
    }

    void insert(const T &v)
    {
        if (vec.size() == 0)
        {
            vec.emplace_back(v);
            return;
        }
        auto it = lower_bound(all(vec), v, cmp);
        vec.insert(it, v);
    }

    void remove(const T &v)
    {
        if (vec.size() == 0)
            return;
        auto it = lower_bound(all(vec), v);
        if (it != vec.end())
            vec.erase(it);
    }

    bool include(const MultiSet<T> &b) const
    {
        int i = 0;
        for (int j = 0; j < b.size(); j++)
        {
            while (i < vec.size() && vec[i] != b.vec[j])
                i++;
            if (i == vec.size())
                return false;
        }
        return true;
    }

    [[nodiscard]] int size() const
    {
        return vec.size();
    }

    MultiSet<T> &operator+(const MultiSet<T> &b) const
    {
        if (b.cmp != cmp)
            throw runtime_error("Error, cannot add multisets with different comparators");

        vector<T> ans(vec.size() + b.size());
        int i = 0, j = 0, size = 0;
        while (i < vec.size() && j < b.size())
        {
            if (vec[i] == b.vec[j])
            {
                ans[size++] = vec[i];
                ans[size++] = b.vec[j];
                i++;
                j++;
                continue;
            }
            while (i < vec.size() && vec[i] < b.vec[j])
            {
                ans[size++] = vec[i++];
            }
            while (j < b.size() && b.vec[j] < vec[i])
            {
                ans[size++] = b.vec[j++];
            }
        }
        while (i < vec.size())
            ans[size++] = vec[i++];
        while (j < b.size())
            ans[size++] = b.vec[j++];

        ans.resize(size);
        return *(new MultiSet(ans));
    }

    MultiSet<T> &operator-(const MultiSet<T> &b) const
    {
        if (b.cmp != cmp)
            throw runtime_error("Error, cannot subtract multisets with different comparators");

        vector<T> ans(vec.size());
        int i = 0, j = 0, size = 0;
        while (i < vec.size() && j < b.size())
        {
            if (vec[i] == b.vec[j])
            {
                i++;
                j++;
                continue;
            }
            while (i < vec.size() && vec[i] < b.vec[j])
            {
                ans[size++] = vec[i++];
            }
            while (j < b.size() && b.vec[j] < vec[i])
            {
                j++;
            }
        }
        if (j >= b.size() && i < vec.size())
            while (i < vec.size())
                ans[size++] = vec[i++];
        ans.resize(size);
        return *(new MultiSet(ans));
    }

    MultiSet<T> &operator*(const MultiSet<T> &b) const
    {
        if (b.cmp != cmp)
            throw runtime_error("Error, cannot intersection multisets with different comparators");

        vector<T> ans(vec.size());
        int i = 0, j = 0, size = 0;
        while (i < vec.size() && j < b.size())
        {
            if (vec[i] == b.vec[j])
            {
                ans[size++] = vec[i];
                i++;
                j++;
                continue;
            }
            while (i < vec.size() && vec[i] < b.vec[j])
                i++;
            while (j < b.size() && b.vec[j] < vec[i])
                j++;
        }
        ans.resize(size);
        return *(new MultiSet(ans));
    }

    friend bool operator==(const MultiSet<T> &a, const MultiSet<T> &b)
    {
        return a.vec.data == b.vec.data;
    }

    friend ostream &operator<<(ostream &os, const MultiSet &b)
    {
        os << "{";
        for (int i = 0; i < b.size(); i++)
            os << b.vec[i];
        os << "}";
        return os;
    }

    void Write(string filename) const
    {
        string path = "../" + filename;
        ofstream file(path, ios_base::trunc);
        file << "[\n";
        for (int i = 0; i < vec.size(); i++)
            file << vec.vec[i];
        file << "]\n";
        file.close();
    }

    ~MultiSet() = default;
};

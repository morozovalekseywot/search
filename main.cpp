#include <iostream>
#include "Index.hpp"

int main()
{
    Indexing obj;
    obj.Fill();
    obj.Write("before_sort.json");
//    obj.Print();
    PriorityMan prior = PriorityMan(0,1, 2, 3, 4, 5);

    obj.Index(prior);
    cout << "\n\nAfterSorted\n\n";
    obj.Write("after_sort.json");
//    obj.Print();
}

#include<iostream>
#include<vector>

namespace VectorInsert1 {

void Test_VectorInsert1()
{
    std::vector<int> v { 1, 2, 3, 4, 5, 6 };
    std::vector<int> new_elements { 7, 8, 9, 10 };

    v.reserve(10);

    auto position = v.end();

    for (int i : new_elements)
    {
        position = v.insert(position, i); // I must re-assign "position", because in a vector, after insertion, 
                                          // the iterator at or after modified element is invalidated
    }

    for (int i : v)
    {
        std::cout << i << ' ' ;
    }

    std::cout << std::endl;

}
}
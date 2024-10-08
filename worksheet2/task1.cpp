#include "bump.hpp"
#include <cstdlib>
#include <iostream>

bump_allocator::bump_allocator(){
    bump_ptr = (char*)malloc(5 * sizeof(int));

    std::cout << static_cast<void*> (bump_ptr) << std::endl;
}

int main(){

    bump_allocator test;

    char* x = test.alloc<char>(1);

    std::cout << static_cast<void*> (x) << std::endl;

    int* y = test.alloc<int>(2);

    std::cout << y << std::endl;
    return 0;
}
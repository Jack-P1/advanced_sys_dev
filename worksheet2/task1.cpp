#include "bump.cpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

int main(){

    bump_allocator test(25);

    // 4 bytes
    char* x = test.alloc<char>(4);

    char testString[] = "abc";

    strcpy(x, testString);

    std::cout << "x address: " << static_cast<void*> (x) << std::endl;

    std::cout << "x value: " << x << std::endl;

    int* y = test.alloc<int>(2);

    std::cout << "y address: "<< y << std::endl;

    test.dealloc(x);

    test.dealloc(y);

    // 4 bytes
    int* test1 = test.alloc<int>(1);

    *test1 = 8;

    std::cout << "test 1 address: " << test1 << std::endl;

    std::cout << "test 1 value: " << *test1 << std::endl;

    int* nullTest = test.alloc<int>(7);

    if(!nullTest){
        std::cout << "Null test = nullptr as not enough space" << std::endl;
    }

    return 0;
}
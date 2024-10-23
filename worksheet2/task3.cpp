#include "bump.hpp"
#include "bump_down.hpp"
#include "./benchmark/benchmark.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

int main()
{
    // bump_allocator bumper(400100);

    // auto (bump_allocator::*funcPtr)(unsigned int, unsigned int) = &bump_allocator::alloc<int>;

    // benchmark bench;
    // bench.measureAverageFunctionRuntime(funcPtr, &bumper);

    bump_down_allocator bumpDown(20);

    // 1 byte
    char* test = bumpDown.alloc<char>(1);

    std::cout << "Test char address: " << static_cast<void*> (test) << std::endl;

    // 8 bytes
    int* x = bumpDown.alloc<int>(2);
    *x = 5;

    std::cout << "X address: " << x << std::endl;
    std::cout << "X value: " << *x << std::endl;

    // overflow
    int* y = bumpDown.alloc<int>(10);

    if (y == nullptr){
        std::cout << "Not enough memory, y = nullptr" << std::endl;
    }

    std::cout << "Benchmarks...." << std::endl;

    bump_allocator bumpUpBench(400100);
    bump_down_allocator bumpDownBench(400100);

    auto (bump_allocator::*bumpUpPtr)(unsigned int, unsigned int) = &bump_allocator::alloc<int>;
    auto (bump_down_allocator::*bumpDownPtr)(unsigned int, unsigned int) = &bump_down_allocator::alloc<int>;

    benchmark bench;
    bench.measureAverageFunctionRuntime(bumpUpPtr, &bumpUpBench);
    bench.measureAverageFunctionRuntime(bumpDownPtr, &bumpDownBench);

    return 0;
}
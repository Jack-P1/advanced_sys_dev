#ifndef BENCH_H
#define BENCH_H

#include <chrono>

// run clang with -O3 optimization flag
class benchmark {
    private:

    public:
        template <typename T>
        void measureAverageFunctionRuntime(T (bump_allocator::*alloc)(unsigned int, unsigned int), bump_allocator* obj)
        {
            
            auto startTime = std::chrono::high_resolution_clock::now();
            (obj->*alloc)(1, 0);
            auto endTime = std::chrono::high_resolution_clock::now();

            uint runs = 1;

            std::chrono::duration<double, std::milli> ms_double = endTime - startTime;
            for(int i = 0; i < 100000; i++)
            {
                auto startTime = std::chrono::high_resolution_clock::now();
                (obj->*alloc)(1, 0);
                auto endTime = std::chrono::high_resolution_clock::now();

                ms_double += endTime - startTime;

                
                runs += 1;
            }

            ms_double = ms_double / runs;
            std::cout << ms_double.count() << "ms\n";

        }
};


#endif
#ifndef BENCH_H
#define BENCH_H

#include <chrono>
#include <utility>

// run clang with -O3 optimization flag
class benchmark {
    private:

    public:
        template <typename T1, typename Ret, typename... Args>
        void measureAverageFunctionRuntime(Ret (T1::*alloc)(Args...), T1* obj, Args... args)
        {
            
            auto startTime = std::chrono::high_resolution_clock::now();
            (obj->*alloc)(std::forward<Args>(args)...);
            auto endTime = std::chrono::high_resolution_clock::now();

            uint runs = 1;

            std::chrono::duration<double, std::milli> ms_double = endTime - startTime;
            for(int i = 0; i < 100000; i++)
            {
                auto startTime = std::chrono::high_resolution_clock::now();
                (obj->*alloc)(std::forward<Args>(args)...);
                auto endTime = std::chrono::high_resolution_clock::now();

                ms_double += endTime - startTime;

                
                runs += 1;
            }

            ms_double = ms_double / runs;
            std::cout << ms_double.count() << "ms\n";

        }
};


#endif
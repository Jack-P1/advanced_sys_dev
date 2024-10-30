#ifndef BUMP_DOWN_H
#define BUMP_DOWN_H

#include <cstddef>
#include <iostream>

class bump_down_allocator {
    private:
        char* start;
        char* end;
        unsigned int allocation_count = 0;
        size_t total_size = 0;

    public:
        bump_down_allocator()
        {
            // 20 bytes as default
            start = (char*)malloc(5 * sizeof(int));
            end = start;
            end += 5 * sizeof(int);
        }

        bump_down_allocator(const size_t size_allocated)
        {
            total_size = size_allocated;
            start = (char*)malloc(total_size);
            end = start;
            end += total_size;
        }

        template <typename T>
        T* alloc(unsigned int num, unsigned int align = 0)
        {
            std::size_t size = sizeof(T) * num;

            // if no alignment provided use alignof type
            if(align == 0)
            {
                align = alignof(T);
            }
            
            // get aligned memory address
            unsigned long aligned_end = (unsigned long) end & ~(align-1);

            // check that there is enough space
            if(aligned_end - size < (unsigned long) start)
            {
                return nullptr;
            }

            // decrement memory address
            end -= size + ((unsigned long) end - aligned_end);

            return reinterpret_cast<T*> (end);
        }

        template <typename T>
        void dealloc(T* ptr)
        {
            if((--allocation_count) == 0){
                free(start);
                end = start;
                end += total_size;
                std::cout << "allocation 0, allocater reset: " << static_cast<void*> (end) << std::endl;
            }

        }
        
};

#endif
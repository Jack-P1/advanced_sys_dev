#ifndef BUMP_H
#define BUMP_H

#include <cstddef>
#include <iostream>

class bump_allocator {
    private:
        char* bump_ptr;
        char* next;
        unsigned int allocation_count = 0;
        unsigned int bytes_allocated = 0;
        size_t total_size = 0;

    public:
        bump_allocator();

        bump_allocator(size_t total_size);

        template <typename T>
        T* alloc(unsigned int num){

            std::size_t size = sizeof(T) * num;

            // add check here for enough memory
            if ((size + bytes_allocated) > total_size){
                return nullptr;
            }

            void* type_ptr = next;

            next = static_cast<char*>(next) + size;

            allocation_count++;

            bytes_allocated += size;

            return reinterpret_cast<T*> (type_ptr);
        }

        template <typename T>
        void dealloc(T* ptr){
            if((--allocation_count) == 0){
                free(bump_ptr);
                next = bump_ptr;
                std::cout << "allocation 0, allocated reset: " << static_cast<void*> (next) << std::endl;
            }

        }
        
};

#endif
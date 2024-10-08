#ifndef BUMP_H
#define BUMP_H

#include <cstddef>

class bump_allocator {
    private:
        char* bump_ptr;
        unsigned int allocation_count = 0;

    public:
        bump_allocator();

        template <typename T>
        T* alloc(unsigned int num){

            std::size_t size = sizeof(T) * num;

            void* type_ptr = bump_ptr;

            bump_ptr = static_cast<char*>(bump_ptr) + size;

            return reinterpret_cast<T*> (type_ptr);
        }
        
};

#endif
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
        bump_allocator()
        {
            // 20 bytes as default
            bump_ptr = (char*)malloc(5 * sizeof(int));
            next = bump_ptr;
        }

        bump_allocator(const size_t size_allocated)
        {
            total_size = size_allocated;
            bump_ptr = (char*)malloc(total_size);
            next = bump_ptr;
        }

        template <typename T>
        T* alloc(unsigned int num, unsigned int align = 0)
        {
            std::size_t size = sizeof(T) * num;

            // if no alignment given use alignment of type
            if (align == 0) {
                align = alignof(T);
            }

            // get aligned memory addresss and padding
            unsigned long aligned_next = ((unsigned long) next + align - 1) & ~(align - 1);
            std::size_t alignment_padding = aligned_next - (unsigned long) next;

            // check if there is enough space
            if ((size + bytes_allocated + alignment_padding) > total_size) {
                return nullptr;
            }

            void* type_ptr = (void*)aligned_next;

            next = (char *) aligned_next + size;

            allocation_count++;

            bytes_allocated += size + alignment_padding;

            return reinterpret_cast<T*> (type_ptr);
        }

        template <typename T>
        void dealloc(T* ptr)
        {
            if((--allocation_count) == 0){
                free(bump_ptr);
                next = bump_ptr;
                bytes_allocated = 0;
                std::cout << "allocation 0, allocater reset: " << static_cast<void*> (next) << std::endl;
            }

        }
        
};

#endif
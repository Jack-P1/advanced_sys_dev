#include "bump.hpp"
#include <cstdlib>
#include <cstring>

// default constructor
bump_allocator::bump_allocator(){
    // 20 bytes as default
    bump_ptr = (char*)malloc(5 * sizeof(int));
    next = bump_ptr;
}

bump_allocator::bump_allocator(const size_t size_allocated){
    total_size = size_allocated;
    bump_ptr = (char*)malloc(total_size);
    next = bump_ptr;
}
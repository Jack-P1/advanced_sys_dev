# Worksheet 2

This readme details my implementation for all tasks comprising worksheet 2. 

## Task 1
The goal of this task is to design a bump up allocator implementing `alloc` and `dealloc` methods. 

```c++
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
```
As shown above, I implemented both a default constructor with an arbritary amount of memory (20 bytes for testing purposes) and a constructor which accepts an argument of `size_t`. Both utilize `malloc` to reserve a chunk of memory on the heap which is used as the available memory for the bump allocator. 

The class variable `char* next` is assigned to the beginning of the available memory and will be the pointer used to increment and allocate memory.

```c++
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

            // increment the pointer by alignment padding plus size
            next = (char *) aligned_next + size;

            allocation_count++;

            bytes_allocated += size + alignment_padding;

            return reinterpret_cast<T*> (aligned_next);
        }
```

The above code shows my implementation to allocate memory. This is a templated function which accepts two arguments - the number of objects of type `T` (used for the size calculation `sizeof(T) * num;`) and `align`. This allows the allocation function to be generic for different types and memory requirement,this is called like so: `char* x = test.alloc<char>(4)`.

Align is used to arrange the memory address to the required byte boundary - e.g. one byte for a char, four bytes for an int. Typically, this speeds up operations and is even strictly enforced on architecture such as ARM. If no `align` argument is provided, the value just defaults to the result of the `alignof()` C++ function. Then it performs the necessary alignment calculations and stores the aligned address and padding. 

Next, a check is performed to ensure there is enough memory, taking into account the required padding and size. If this would result in an overflow then a `nullptr` is returned.

If there is enough available memory then the next pointer is incremented by the required bytes and a pointer of type `T` is returned.

```c++
        void dealloc()
        {
            if((--allocation_count) == 0){
                next = bump_ptr;
                bytes_allocated = 0;
                std::cout << "allocation 0, allocater reset: " << static_cast<void*> (next) << std::endl;
            }

        }
```

The deallocate function simply decrements the allocation count each time it is called. If this reaches zero, meaning there are no more references in use to the memory, then the next pointer is reset to the beginning of the heap and `bytes_allocated` is reset to zero.

```c++
        ~bump_allocator(){
            free(bump_ptr);
        }
```

`free()` is only called once the destructor is called for the bump allocator. This is because deallocate only resets the memory but may need it for future use. 

My test code for this task can be found in `task1.cpp` and demonstrates all functionality found above. The output of this program is:

```
x address: 0x80eeb0
x value: abc
y address: 0x80eeb4
allocation 0, allocater reset: 0x80eeb0
test 1 address: 0x80eeb0
test 1 value: 8
Null test = nullptr as not enough space
```

## Task 2









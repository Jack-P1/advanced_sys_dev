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

Align is used to arrange the memory address to the required byte boundary - e.g. one byte for a char, four bytes for an int. Typically, this speeds up operations and is even strictly enforced on architecture such as ARM. If no `align` argument is provided, the value just defaults to the result of the `alignof()` C++ function. Then it performs the necessary alignment calculations (rounds up to the nearest multiple of the alignment and clears the least significant bits) stores the aligned address and padding. 

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

For this task, I used the simpletest framework, which can be found here: https://github.com/kudaba/simpletest.

I developed various unit tests to validate my allocator:

```c++
char const * groups[] = {
    "BumpUp",
};

DEFINE_TEST_G(testMemoryAllocSuccessfullForCorrectInput, BumpUp)
{
    bump_allocator bumper(20 * sizeof(int));

    int * x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");

    int * y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");

}

DEFINE_TEST_G(testMemoryAllocSuccessfullForDifferentTypes, BumpUp)
{
    bump_allocator bumper(25 * sizeof(int));

    int * x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");

    int * y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");

    char * z = bumper.alloc<char>(20);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");

    char * t = bumper.alloc<char>(1);
    TEST_MESSAGE(t == nullptr, "Should have failed to allocate!!!!");

}

DEFINE_TEST_G(testMemoryAllocFailsIfSizeGreaterThanHeap, BumpUp)
{
    bump_allocator bumper(20 * sizeof(int));

    int * z = bumper.alloc<int>(30);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");

}

DEFINE_TEST_G(testMemoryAllocAddressIsCorrectlyAligned, BumpUp)
{
    bump_allocator bumper(20 * sizeof(int));

    char* x = bumper.alloc<char>(1);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");

    int* y = bumper.alloc<int>(1);
    TEST_MESSAGE((reinterpret_cast<uintptr_t>(y) % alignof(int)) == 0, "Memory address is not aligned!");

}

DEFINE_TEST_G(testMemoryDeallocWhenAllocationCountReachesZero, BumpUp)
{
    bump_allocator bumper(20 * sizeof(int));

    int * x = bumper.alloc<int>(10);
    int * y = bumper.alloc<int>(10);

    int * xMemoryAddress = x;

    bumper.dealloc(x);
    bumper.dealloc(y);

    int * z = bumper.alloc<int>(10);

    TEST_MESSAGE(z == xMemoryAddress, "Failed to reset pointer to beginning of heap!");
}
```
This can be found in`bump_tests.cpp`.

These cover the requirements for both the allocate and deallocate functions.

- **testMemoryAllocSuccessfullForCorrectInput**: Is a success case to validate that the allocator returns a valid pointer.

- **testMemoryAllocSuccessfullForDifferentTypes**: This is similar to the above but ensures that it functions with a variety of types.

- **testMemoryAllocFailsIfSizeGreaterThanHeap**: This test validates that a null pointer is returned if the requests size is greater than the available memory in the heap.

- **testMemoryAllocAddressIsCorrectlyAligned**: This validates that the memory address is properly aligned according to the type. This is done by first assigning a char and then an int. The int will not be properly aligned by default so will need to be padded by three bytes to a four byte boundary. This ensures the alignment calculation is correct.

- **testMemoryDeallocWhenAllocationCountReachesZero**: This tests that the deallocate function resets as expected. To do this, it compares the address of the first allocated pointer before resetting to that of the first pointer after resetting and ensures they are the same address.

These cover the scope of the functionality of the allocator so far. The output of the test file is:

```
Running all tests in groups [BumpUp].
Running [BumpUp/testMemoryAllocSuccessfullForCorrectInput]: Passed 2 out of 2 tests in 3e-06 seconds
Running [BumpUp/testMemoryAllocSuccessfullForDifferentTypes]: Passed 4 out of 4 tests in 1e-06 seconds
Running [BumpUp/testMemoryAllocFailsIfSizeGreaterThanHeap]: Passed 1 out of 1 tests in 1e-06 seconds
Running [BumpUp/testMemoryAllocAddressIsCorrectlyAligned]: Passed 2 out of 2 tests in 0 seconds
Running [BumpUp/testMemoryDeallocWhenAllocationCountReachesZero]allocation 0, allocater reset: 0x9232c0
: Passed 1 out of 1 tests in 1.9e-05 seconds
5 Tests finished. All 10 assertions are passing.
```
I organised these tests into a group called 'BumpUp' and then defined individual tests within that group. This means that the test output is verbose and if one fails it is easy to identify which specific case is failing.

## Task 3

The first part of this task is to implement a small benchmark library which accepts `void function(void)` and different numbers of arguments.

To achieve this I implemented the following function:

```c++
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
```

This is a templated function with a few template arguments:
- `T1` is the class being passed in
- `Ret` is the return type of the class method. I have implemented is this way due to the alloc function being templated, so the return type is not known until runtime. 
- `... Args` which allows the function to accept a variable number of arguments.

This utlizes the C++ library chrono as a way to measure the time taken to execute the function. The function is executed 100000 times in a loop, with the time elapsed added to `ms_double`, and then this is averaged across the 100000 runs. Due to the way the function is written, the line: `(obj->*alloc)(std::forward<Args>(args)...);` can be used with any combination of object, method, and function arguments. 

The next goal of this task was to refactor the bump allocator to bump down instead of up.

```c++
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

        void dealloc()
        {
            if((--allocation_count) == 0){
                end = start;
                end += total_size;
                std::cout << "allocation 0, allocater reset: " << static_cast<void*> (end) << std::endl;
            }

        }

        ~bump_down_allocator(){
            free(start);
        }
        
};
```

The only notable difference in the constructors is that there is now an `end` pointer which is set to the end of the heap. This is the pointer which will be decremented down to allocate available memory.

The primary difference is in the allocation function:

```c++
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
```

Like the bump up allocate fuction, this is a templated function which accepts any type `T` and is able to allocate `sizeof(T) * num` space if available.

This allocation function is more efficient for a couple of reasons:
- The alignment calculation is slightly simpler, it only has to do a bitwise AND (`&`) to clear the least significant bits and round down
- There are less control paths which produces less instructions

As previously mentioned the pointer in this allocater begins at the end of the memory and decrements down. Meaning it is returned after being decremented. 

The deallocator and destructor methods are nearly identical to the bump up allocator. The only different being that the `end` pointer is reset to the end of the heap.

The above implementation can be found in `bump_down.hpp`.

After implementing the bump down allocator I used the benchmark library I wrote to compare this against bump up. 

I ran this with no optimizations, -02, and -O3 - results below:

- No optimizations: bump up took 4.37124e-05ms,bump down took 3.13111e-05ms
- -O2 flag: bump up took 2.77898e-05ms, bump down took 2.77618e-05ms
- -O3 flag: bump up took 2.78578e-05ms, bump down took 2.79799e-05ms

As you can see, bump down is usually slightly faster, but took longer with the -O3 flag. I ran this a number of times and saw that the results were consistently close but neither allocator was consistently faster. I think this could be due to the -O3 flag adding more aggressive optimizations. Therefore, both solutions have been optimized to the point that there is not much difference between the two.

This code can be found in `task3.cpp`.
#include "bump.cpp"
#include "./simpletest/simpletest.h"
using namespace std;

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

    bumper.dealloc();
    bumper.dealloc();

    int * z = bumper.alloc<int>(10);

    TEST_MESSAGE(z == xMemoryAddress, "Failed to reset pointer to beginning of heap!");
}

int main() {
    bool pass = true;
    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }
    
    return pass ? 0 : 1;
}

#include "bump.cpp"
#include "./simpletest/simpletest.h"
using namespace std;

char const * groups[] = {
    "Bump",
};

DEFINE_TEST_G(testMemoryAllocSuccessfullForCorrectInput, Bump)
{
    bump_allocator bumper(20 * sizeof(int));

    int * x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");

    int * y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");

}

DEFINE_TEST_G(testMemoryAllocSuccessfullForDifferentTypes, Bump)
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

DEFINE_TEST_G(testMemoryAllocFailsIfSizeGreaterThanHeap, Bump)
{
    bump_allocator bumper(20 * sizeof(int));

    int * z = bumper.alloc<int>(30);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");

}

DEFINE_TEST_G(testMemoryDeallocWhenAllocationCountReachesZero, Bump)
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

int main() {
    bool pass = true;
    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }
    
    return pass ? 0 : 1;
}

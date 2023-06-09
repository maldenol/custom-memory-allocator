#include <thread>

#include <gtest/gtest.h>

#include "FreeListAllocator.h"


#pragma GCC diagnostic ignored "-Wunused-variable"


TEST(FreeListAllocator, testType01)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        200,
        10
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator1{};
    FreeListAllocator<bool, FreeListMode, AllocatorMode> allocator2{};
    FreeListAllocator<int, FreeListMode, AllocatorMode> allocator3{};
    FreeListAllocator<float, FreeListMode, AllocatorMode> allocator4{};
    FreeListAllocator<double, FreeListMode, AllocatorMode> allocator5{};
    FreeListAllocator<void *, FreeListMode, AllocatorMode> allocator6{};

    char *ptr1 = allocator1.allocate(176);
    bool *ptr2 = allocator2.allocate(176);
    int *ptr3 = allocator3.allocate(176);
    float *ptr4 = allocator4.allocate(176);
    double *ptr5 = allocator5.allocate(176);
    void **ptr6 = allocator6.allocate(176);

    EXPECT_TRUE(
        allocator1.isValid(ptr1) &&
        allocator2.isValid(ptr2) &&
        allocator3.isValid(ptr3) &&
        allocator4.isValid(ptr4) &&
        allocator5.isValid(ptr5) &&
        allocator6.isValid(ptr6)
    );
}

TEST(FreeListAllocator, testIsValid01)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        200,
        10
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    EXPECT_TRUE(!allocator.isValid(nullptr));
}

TEST(FreeListAllocator, testIsValid02)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        200,
        10
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValid(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 1) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 1)
    );
}

TEST(FreeListAllocator, testIsValid03)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        200,
        10
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testIsValidFast01)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        200,
        10
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    EXPECT_TRUE(!allocator.isValidFast(nullptr));
}

TEST(FreeListAllocator, testIsValidFast02)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        200,
        10
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValidFast(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 200) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 200)
    );
}

TEST(FreeListAllocator, testIsValidFast03)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        200,
        10
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValidFast(ptr1));
}

TEST(FreeListAllocator, testAllocateDeallocate01)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        200,
        2
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    char *ptr1 = allocator.allocate(100);
    char *ptr2 = allocator.allocate(100);
    char *ptr3 = allocator.allocate(100);
    allocator.deallocate(ptr2);
    bool isValidPtr2 = allocator.isValid(ptr2);
    char *ptr4 = allocator.allocate(100);

    EXPECT_TRUE(
        allocator.isValid(ptr1) &&
        !isValidPtr2 &&
        !allocator.isValid(ptr3) &&
        allocator.isValid(ptr4)
    );
}

TEST(FreeListAllocator, testAllocateDeallocate02)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        100,
        10
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    char *ptr1 = allocator.allocate(100);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocateDeallocate03)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        250,
        2
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    char *ptr1 = allocator.allocate(48);
    char *ptr2 = allocator.allocate(48);
    char *ptr3 = allocator.allocate(48);
    char *ptr4 = allocator.allocate(48);
    char *ptr5 = allocator.allocate(48);
    char *ptr6 = allocator.allocate(48);
    char *ptr7 = allocator.allocate(48);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr5);
    char *ptr8 = allocator.allocate(48);
    char *ptr9 = allocator.allocate(48);

    EXPECT_TRUE(
        allocator.isValid(ptr1) &&
        allocator.isValid(ptr2) &&
        allocator.isValid(ptr3) &&
        allocator.isValid(ptr4) &&
        allocator.isValid(ptr5) &&
        allocator.isValid(ptr6) &&
        !allocator.isValid(ptr7) &&
        allocator.isValid(ptr8) &&
        allocator.isValid(ptr9) &&
        ptr2 == ptr8 &&
        ptr5 == ptr9
    );
}

TEST(FreeListAllocator, testStateless01)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        300,
        2
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator1{};
    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator2{};

    char *ptr1 = allocator1.allocate(100);
    char *ptr2 = allocator1.allocate(100);
    allocator2.deallocate(ptr1);

    EXPECT_TRUE(
        !allocator1.isValid(ptr1) &&
        allocator2.isValid(ptr2)
    );
}

TEST(FreeListAllocator, testThreadSafe01)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        691,
        1
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    char *ptr1 = nullptr;

    auto func = [&allocator, &ptr1](){
        ptr1 = allocator.allocate(500);
    };

    std::thread thread1{func};

    thread1.join();

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testThreadSafe02)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        692,
        1
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    char *ptr1 = nullptr;
    char *ptr2 = nullptr;

    auto func = [&allocator](char *&ptr){
        ptr = allocator.allocate(500);
    };

    std::thread thread1{func, std::ref(ptr1)};
    std::thread thread2{func, std::ref(ptr2)};

    thread1.join();
    thread2.join();

    EXPECT_TRUE(!allocator.isValid(ptr1) && !allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testThreadSafe03)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        693,
        1
    };

    FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

    bool success1 = false;
    bool success2 = false;

    auto func = [&success1, &success2, &allocator](){
        for (size_t i = 0; i < 10000; ++i)
        {
            char *ptr1 = allocator.allocate(500);
            if (ptr1)
            {
                allocator.deallocate(ptr1);
            }
            else
            {
                return;
            }
        }

        static bool threadFlipFlop = true;

        if (threadFlipFlop)
        {
            success1 = true;
        }
        else
        {
            success2 = true;
        }

        threadFlipFlop = !threadFlipFlop;
    };

    std::thread thread1{func};
    std::thread thread2{func};

    thread1.join();
    thread2.join();

    EXPECT_TRUE(success1 && success2);
}

TEST(FreeListAllocator, testThreadSafe04)
{
    constexpr FreeListMode FreeListMode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    constexpr FreeListAllocatorMode AllocatorMode
    {
        694,
        1
    };

    bool success1 = false;
    bool success2 = false;

    auto func = [&success1, &success2, FreeListMode, AllocatorMode](){
        FreeListAllocator<char, FreeListMode, AllocatorMode> allocator{};

        char *ptr1 = allocator.allocate(500);
        char *ptr2 = allocator.allocate(500);

        bool success = allocator.isValid(ptr1) && !allocator.isValid(ptr2);

        static bool threadFlipFlop = true;

        if (threadFlipFlop)
        {
            success1 = success;
        }
        else
        {
            success2 = success;
        }

        threadFlipFlop = !threadFlipFlop;
    };

    std::thread thread1{func};
    std::thread thread2{func};

    thread1.join();
    thread2.join();

    EXPECT_TRUE(success1 && success2);
}

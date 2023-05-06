#include <iostream>

#include <gtest/gtest.h>

#include "FreeListAllocator.h"


#pragma GCC diagnostic ignored "-Wunused-variable"


TEST(FreeListAllocator, testType01)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator1(200);
    FreeListAllocator<bool, Mode> allocator2(200);
    FreeListAllocator<int, Mode> allocator3(200);
    FreeListAllocator<float, Mode> allocator4(200);
    FreeListAllocator<double, Mode> allocator5(200);
    FreeListAllocator<void *, Mode> allocator6(200);

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
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    EXPECT_TRUE(!allocator.isValid(nullptr));
}

TEST(FreeListAllocator, testIsValid02)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValid(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 1) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 1)
    );
}

TEST(FreeListAllocator, testIsValid03)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testIsValidFast01)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    EXPECT_TRUE(!allocator.isValidFast(nullptr));
}

TEST(FreeListAllocator, testIsValidFast02)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValidFast(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 200) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 200)
    );
}

TEST(FreeListAllocator, testIsValidFast03)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValidFast(ptr1));
}

TEST(FreeListAllocator, testAllocate01)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(0);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate02)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(-1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate03)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate04)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate05a)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(177);
    char *ptr2 = allocator.allocate(176);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testAllocate05b)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(169);
    char *ptr2 = allocator.allocate(168);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testAllocate06a)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_DONT_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(76);
    char *ptr2 = allocator.allocate(76);
    char *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeListAllocator, testAllocate06b)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(76);
    char *ptr2 = allocator.allocate(72);
    char *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeListAllocator, testAllocate06c)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_DONT_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(68);
    char *ptr2 = allocator.allocate(68);
    char *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeListAllocator, testAllocate06d)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(68);
    char *ptr2 = allocator.allocate(64);
    char *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeListAllocator, testAllocate07)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    char *ptr6 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr5) && !allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testAllocate08)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(200);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testDeallocate01)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testDeallocate02)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);
    char *ptr2 = allocator.allocate(24);

    EXPECT_TRUE(ptr1 == ptr2);
}

TEST(FreeListAllocator, testDeallocate03)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(24);
    char *ptr2 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testDeallocate04)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(24);
    char *ptr2 = allocator.allocate(24);
    allocator.deallocate(ptr2);

    EXPECT_TRUE(allocator.isValid(ptr1) && !allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testDeallocate05a)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr1);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr3);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr5);
    char *ptr6 = allocator.allocate(176);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate05b)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr1);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr3);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr5);
    char *ptr6 = allocator.allocate(168);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate06a)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr5);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr3);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr1);
    char *ptr6 = allocator.allocate(176);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate06b)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr5);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr3);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr1);
    char *ptr6 = allocator.allocate(168);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate07a)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr3);
    allocator.deallocate(ptr1);
    allocator.deallocate(ptr5);
    char *ptr6 = allocator.allocate(176);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate07b)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr3);
    allocator.deallocate(ptr1);
    allocator.deallocate(ptr5);
    char *ptr6 = allocator.allocate(168);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate08a)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr3);
    char *ptr6 = allocator.allocate(96);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate08b)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr3);
    char *ptr6 = allocator.allocate(96);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testFirstFit01a)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(2000);

    char *ptr1 = allocator.allocate(500);
    char *ptr2 = allocator.allocate(200);
    char *ptr3 = allocator.allocate(100);
    char *ptr4 = allocator.allocate(50);
    char *ptr5 = allocator.allocate(10);
    char *ptr6 = allocator.allocate(5);
    char *ptr7 = allocator.allocate(1);

    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr6);

    char *ptr8 = allocator.allocate(50);

    EXPECT_TRUE(ptr8 == ptr2);
}

TEST(FreeListAllocator, testFirstFit01b) {
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(2000);

    char *ptr1 = allocator.allocate(500);
    char *ptr2 = allocator.allocate(200);
    char *ptr3 = allocator.allocate(100);
    char *ptr4 = allocator.allocate(50);
    char *ptr5 = allocator.allocate(10);
    char *ptr6 = allocator.allocate(5);
    char *ptr7 = allocator.allocate(1);

    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr6);

    char *ptr8 = allocator.allocate(50);

    EXPECT_TRUE(ptr8 == ptr2);
}

TEST(FreeListAllocator, testNextFit01a)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_NEXT_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(4000);

    char *ptr1 = allocator.allocate(100);
    char *ptr2 = allocator.allocate(1000);
    char *ptr3 = allocator.allocate(100);
    char *ptr4 = allocator.allocate(1000);
    char *ptr5 = allocator.allocate(100);
    char *ptr6 = allocator.allocate(1000);
    char *ptr7 = allocator.allocate(100);

    allocator.deallocate(ptr4);
    allocator.deallocate(ptr6);

    char *ptr8 = allocator.allocate(1000);

    allocator.deallocate(ptr2);

    char *ptr9 = allocator.allocate(1);

    EXPECT_TRUE(ptr8 == ptr4 && ptr9 == ptr6);
}

TEST(FreeListAllocator, testNextFit01b) {
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_NEXT_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(4000);

    char *ptr1 = allocator.allocate(100);
    char *ptr2 = allocator.allocate(1000);
    char *ptr3 = allocator.allocate(100);
    char *ptr4 = allocator.allocate(1000);
    char *ptr5 = allocator.allocate(100);
    char *ptr6 = allocator.allocate(1000);
    char *ptr7 = allocator.allocate(100);

    allocator.deallocate(ptr4);
    allocator.deallocate(ptr6);

    char *ptr8 = allocator.allocate(1000);

    allocator.deallocate(ptr2);

    char *ptr9 = allocator.allocate(1);

    EXPECT_TRUE(ptr8 == ptr4 && ptr9 == ptr6);
}

TEST(FreeListAllocator, testBestFit01a)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_BEST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(2000);

    char *ptr1 = allocator.allocate(500);
    char *ptr2 = allocator.allocate(200);
    char *ptr3 = allocator.allocate(100);
    char *ptr4 = allocator.allocate(50);
    char *ptr5 = allocator.allocate(10);
    char *ptr6 = allocator.allocate(5);
    char *ptr7 = allocator.allocate(1);

    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr6);

    char *ptr8 = allocator.allocate(50);

    EXPECT_TRUE(ptr8 == ptr4);
}

TEST(FreeListAllocator, testBestFit01b)
{
    constexpr FreeListAllocatorMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_BEST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeListAllocator<char, Mode> allocator(2000);

    char *ptr1 = allocator.allocate(500);
    char *ptr2 = allocator.allocate(200);
    char *ptr3 = allocator.allocate(100);
    char *ptr4 = allocator.allocate(50);
    char *ptr5 = allocator.allocate(10);
    char *ptr6 = allocator.allocate(5);
    char *ptr7 = allocator.allocate(1);

    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr6);

    char *ptr8 = allocator.allocate(50);

    EXPECT_TRUE(ptr8 == ptr4);
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

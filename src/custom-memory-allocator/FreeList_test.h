#include <gtest/gtest.h>

#include "FreeList.h"


#pragma GCC diagnostic ignored "-Wunused-variable"


TEST(FreeList, testType01)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator1{200};
    FreeList<bool, Mode> allocator2{200};
    FreeList<int, Mode> allocator3{200};
    FreeList<float, Mode> allocator4{200};
    FreeList<double, Mode> allocator5{200};
    FreeList<void *, Mode> allocator6{200};

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

TEST(FreeList, testIsValid01)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    EXPECT_TRUE(!allocator.isValid(nullptr));
}

TEST(FreeList, testIsValid02)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValid(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 1) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 1)
    );
}

TEST(FreeList, testIsValid03)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeList, testIsValidFast01)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    EXPECT_TRUE(!allocator.isValidFast(nullptr));
}

TEST(FreeList, testIsValidFast02)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValidFast(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 200) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 200)
    );
}

TEST(FreeList, testIsValidFast03)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValidFast(ptr1));
}

TEST(FreeList, testAllocate01)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(0);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeList, testAllocate02)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(-1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeList, testAllocate03)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr1));
}

TEST(FreeList, testAllocate04)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(allocator.isValid(ptr1));
}

TEST(FreeList, testAllocate05a)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(177);
    char *ptr2 = allocator.allocate(176);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeList, testAllocate05b)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(169);
    char *ptr2 = allocator.allocate(168);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeList, testAllocate06a)
{
    constexpr FreeListMode Mode{
        FLA_DONT_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(76);
    char *ptr2 = allocator.allocate(76);
    char *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeList, testAllocate06b)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(76);
    char *ptr2 = allocator.allocate(72);
    char *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeList, testAllocate06c)
{
    constexpr FreeListMode Mode{
        FLA_DONT_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(68);
    char *ptr2 = allocator.allocate(68);
    char *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeList, testAllocate06d)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(68);
    char *ptr2 = allocator.allocate(64);
    char *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeList, testAllocate07)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    char *ptr6 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr5) && !allocator.isValid(ptr6));
}

TEST(FreeList, testAllocate08)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(200);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeList, testDeallocate01)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeList, testDeallocate02)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);
    char *ptr2 = allocator.allocate(24);

    EXPECT_TRUE(ptr1 == ptr2);
}

TEST(FreeList, testDeallocate03)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(24);
    char *ptr2 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeList, testDeallocate04)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

    char *ptr1 = allocator.allocate(24);
    char *ptr2 = allocator.allocate(24);
    allocator.deallocate(ptr2);

    EXPECT_TRUE(allocator.isValid(ptr1) && !allocator.isValid(ptr2));
}

TEST(FreeList, testDeallocate05a)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

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

TEST(FreeList, testDeallocate05b)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

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

TEST(FreeList, testDeallocate06a)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

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

TEST(FreeList, testDeallocate06b)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

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

TEST(FreeList, testDeallocate07a)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

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

TEST(FreeList, testDeallocate07b)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

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

TEST(FreeList, testDeallocate08a)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

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

TEST(FreeList, testDeallocate08b)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{200};

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

TEST(FreeList, testFirstFit01a)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{2000};

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

TEST(FreeList, testFirstFit01b) {
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_FIRST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{2000};

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

TEST(FreeList, testNextFit01a)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_NEXT_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{4000};

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

TEST(FreeList, testNextFit01b) {
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_NEXT_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{4000};

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

TEST(FreeList, testBestFit01a)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_BEST_FIT,
        FLA_SINGLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{2000};

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

TEST(FreeList, testBestFit01b)
{
    constexpr FreeListMode Mode{
        FLA_ALIGN_BY_MEMORY_BLOCKS,
        FLA_ALLOCATION_STRATEGY_BEST_FIT,
        FLA_DOUBLE_MEMORY_BLOCKS
    };

    FreeList<char, Mode> allocator{2000};

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

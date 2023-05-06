#include <iostream>

#include <gtest/gtest.h>

#include "FreeListAllocator.h"


TEST(FreeListAllocator, testIsValid00)
{
    FreeListAllocator allocator(200);

    EXPECT_TRUE(!allocator.isValid(nullptr));
}

TEST(FreeListAllocator, testIsValid01)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValid(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 1) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 1)
    );
}

TEST(FreeListAllocator, testIsValid02)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testIsValidFast00)
{
    FreeListAllocator allocator(200);

    EXPECT_TRUE(!allocator.isValidFast(nullptr));
}

TEST(FreeListAllocator, testIsValidFast01)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValidFast(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 200) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 200)
    );
}

TEST(FreeListAllocator, testIsValidFast02)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValidFast(ptr1));
}

TEST(FreeListAllocator, testAllocate00)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(0);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate01)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(-1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate02)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate03)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate04)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(177);
    void *ptr2 = allocator.allocate(176);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testAllocate05)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(76);
    void *ptr2 = allocator.allocate(76);
    void *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeListAllocator, testAllocate06)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(16);
    void *ptr2 = allocator.allocate(16);
    void *ptr3 = allocator.allocate(16);
    void *ptr4 = allocator.allocate(16);
    void *ptr5 = allocator.allocate(16);
    void *ptr6 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr5) && !allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testAllocate07)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(200);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testDeallocate00)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testDeallocate01)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);
    void *ptr2 = allocator.allocate(24);

    EXPECT_TRUE(ptr1 == ptr2);
}

TEST(FreeListAllocator, testDeallocate02)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(24);
    void *ptr2 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testDeallocate03)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(24);
    void *ptr2 = allocator.allocate(24);
    allocator.deallocate(ptr2);

    EXPECT_TRUE(allocator.isValid(ptr1) && !allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testDeallocate04)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(16);
    void *ptr2 = allocator.allocate(16);
    void *ptr3 = allocator.allocate(16);
    void *ptr4 = allocator.allocate(16);
    void *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr1);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr3);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr5);
    void *ptr6 = allocator.allocate(176);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate05)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(16);
    void *ptr2 = allocator.allocate(16);
    void *ptr3 = allocator.allocate(16);
    void *ptr4 = allocator.allocate(16);
    void *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr5);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr3);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr1);
    void *ptr6 = allocator.allocate(176);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate06)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(16);
    void *ptr2 = allocator.allocate(16);
    void *ptr3 = allocator.allocate(16);
    void *ptr4 = allocator.allocate(16);
    void *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr3);
    allocator.deallocate(ptr1);
    allocator.deallocate(ptr5);
    void *ptr6 = allocator.allocate(176);

    EXPECT_TRUE(allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testDeallocate07)
{
    FreeListAllocator allocator(200);

    void *ptr1 = allocator.allocate(16);
    void *ptr2 = allocator.allocate(16);
    void *ptr3 = allocator.allocate(16);
    void *ptr4 = allocator.allocate(16);
    void *ptr5 = allocator.allocate(16);
    allocator.deallocate(ptr2);
    allocator.deallocate(ptr4);
    allocator.deallocate(ptr3);
    void *ptr6 = allocator.allocate(96);

    EXPECT_TRUE(allocator.isValid(ptr6));
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

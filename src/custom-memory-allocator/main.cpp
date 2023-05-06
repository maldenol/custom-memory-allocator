#include <iostream>

#include <gtest/gtest.h>

#include "FreeListAllocator.h"


TEST(FreeListAllocator, testType00)
{
    FreeListAllocator<char> allocator1(200);
    FreeListAllocator<int> allocator2(200);
    FreeListAllocator<float> allocator3(200);
    FreeListAllocator<double> allocator4(200);
    FreeListAllocator<void> allocator5(200);
    FreeListAllocator<void *> allocator6(200);
    FreeListAllocator<long *> allocator7(200);

    char *ptr1 = allocator1.allocate(176);
    int *ptr2 = allocator2.allocate(176);
    float *ptr3 = allocator3.allocate(176);
    double *ptr4 = allocator4.allocate(176);
    void *ptr5 = allocator5.allocate(176);
    void **ptr6 = allocator6.allocate(176);
    long **ptr7 = allocator7.allocate(176);

    EXPECT_TRUE(
        allocator1.isValid(ptr1) &&
        allocator2.isValid(ptr2) &&
        allocator3.isValid(ptr3) &&
        allocator4.isValid(ptr4) &&
        allocator5.isValid(ptr5) &&
        allocator6.isValid(ptr6) &&
        allocator7.isValid(ptr7)
    );
}

TEST(FreeListAllocator, testIsValid00)
{
    FreeListAllocator<char> allocator(200);

    EXPECT_TRUE(!allocator.isValid(nullptr));
}

TEST(FreeListAllocator, testIsValid01)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValid(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 1) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 1)
    );
}

TEST(FreeListAllocator, testIsValid02)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testIsValidFast00)
{
    FreeListAllocator<char> allocator(200);

    EXPECT_TRUE(!allocator.isValidFast(nullptr));
}

TEST(FreeListAllocator, testIsValidFast01)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(
        allocator.isValidFast(ptr1) &&
        !allocator.isValid(static_cast<char *>(ptr1) + 200) &&
        !allocator.isValid(static_cast<char *>(ptr1) - 200)
    );
}

TEST(FreeListAllocator, testIsValidFast02)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValidFast(ptr1));
}

TEST(FreeListAllocator, testAllocate00)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(0);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate01)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(-1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate02)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate03)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(24);

    EXPECT_TRUE(allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testAllocate04)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(177);
    char *ptr2 = allocator.allocate(176);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testAllocate05)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(76);
    char *ptr2 = allocator.allocate(76);
    char *ptr3 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr2) && !allocator.isValid(ptr3));
}

TEST(FreeListAllocator, testAllocate06)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(16);
    char *ptr2 = allocator.allocate(16);
    char *ptr3 = allocator.allocate(16);
    char *ptr4 = allocator.allocate(16);
    char *ptr5 = allocator.allocate(16);
    char *ptr6 = allocator.allocate(1);

    EXPECT_TRUE(allocator.isValid(ptr5) && !allocator.isValid(ptr6));
}

TEST(FreeListAllocator, testAllocate07)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(200);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testDeallocate00)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1));
}

TEST(FreeListAllocator, testDeallocate01)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(24);
    allocator.deallocate(ptr1);
    char *ptr2 = allocator.allocate(24);

    EXPECT_TRUE(ptr1 == ptr2);
}

TEST(FreeListAllocator, testDeallocate02)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(24);
    char *ptr2 = allocator.allocate(24);
    allocator.deallocate(ptr1);

    EXPECT_TRUE(!allocator.isValid(ptr1) && allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testDeallocate03)
{
    FreeListAllocator<char> allocator(200);

    char *ptr1 = allocator.allocate(24);
    char *ptr2 = allocator.allocate(24);
    allocator.deallocate(ptr2);

    EXPECT_TRUE(allocator.isValid(ptr1) && !allocator.isValid(ptr2));
}

TEST(FreeListAllocator, testDeallocate04)
{
    FreeListAllocator<char> allocator(200);

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

TEST(FreeListAllocator, testDeallocate05)
{
    FreeListAllocator<char> allocator(200);

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

TEST(FreeListAllocator, testDeallocate06)
{
    FreeListAllocator<char> allocator(200);

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

TEST(FreeListAllocator, testDeallocate07)
{
    FreeListAllocator<char> allocator(200);

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


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

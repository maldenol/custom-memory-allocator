#ifndef FREELISTALLOCATOR_H
#define FREELISTALLOCATOR_H

#include <stddef.h>
#include <type_traits>
#include <vector>

#include "FreeList.h"


struct FreeListAllocatorMode
{
    size_t freeListSize;
    size_t maxFreeListCount;
};

template<typename T, FreeListMode FreeListMode, FreeListAllocatorMode AllocatorMode>
class FreeListAllocator
{
public:
    static T *allocate(size_t size);
    static void deallocate(T *ptr);

    static bool isValid(const T *ptr);
    static bool isValidFast(const T *ptr);

private:
    inline static std::vector<FreeList<T, FreeListMode>> freeLists = {};
};


#include "FreeListAllocator.inl"

#endif

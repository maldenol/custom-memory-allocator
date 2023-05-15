#include "FreeListAllocator.h"


template<typename T, FreeListMode FreeListMode, FreeListAllocatorMode AllocatorMode>
T *FreeListAllocator<T, FreeListMode, AllocatorMode>::allocate(size_t size)
{
    if (size > AllocatorMode.freeListSize)
    {
        return nullptr;
    }

    for (FreeList<T, FreeListMode> &freeList: freeLists)
    {
        T *ptr = freeList.allocate(size);

        if (ptr) return ptr;
    }

    if (freeLists.size() < AllocatorMode.maxFreeListCount)
    {
        FreeList<T, FreeListMode> freeList{AllocatorMode.freeListSize};

        T *ptr = freeList.allocate(size);

        if (ptr)
        {
            freeLists.push_back(std::move(freeList));

            return ptr;
        }
    }

    return nullptr;
}

template<typename T, FreeListMode FreeListMode, FreeListAllocatorMode AllocatorMode>
void FreeListAllocator<T, FreeListMode, AllocatorMode>::deallocate(T *ptr)
{
    for (typename std::vector<FreeList<T, FreeListMode>>::iterator it = freeLists.begin(); it != freeLists.end();)
    {
        if (it->isValidFast(ptr))
        {
            it->deallocate(ptr);

            if (it->isEmpty())
            {
                it = freeLists.erase(it);
                continue;
            }
        }

        ++it;
    }
}

template<typename T, FreeListMode FreeListMode, FreeListAllocatorMode AllocatorMode>
bool FreeListAllocator<T, FreeListMode, AllocatorMode>::isValid(const T *ptr)
{
    for (const FreeList<T, FreeListMode> &freeList: freeLists)
    {
        if (freeList.isValid(ptr)) return true;
    }

    return false;
}

template<typename T, FreeListMode FreeListMode, FreeListAllocatorMode AllocatorMode>
bool FreeListAllocator<T, FreeListMode, AllocatorMode>::isValidFast(const T *ptr)
{
    for (const FreeList<T, FreeListMode> &freeList: freeLists)
    {
        if (freeList.isValidFast(ptr)) return true;
    }

    return false;
}

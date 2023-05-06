#ifndef FREELISTALLOCATOR_H
#define FREELISTALLOCATOR_H

#include <stddef.h>


template<typename T>
class FreeListAllocator
{
private:
    struct MemoryBlock;

public:
    FreeListAllocator(size_t inMemoryPoolSize);
    ~FreeListAllocator();

    T *allocate(size_t size);
    void deallocate(T *ptr);

    bool isValid(const T *ptr) const;
    bool isValidFast(const T *ptr) const;

private:
    void splitMemoryBlock(MemoryBlock *leftBlock, size_t leftBlockSize);
    void mergeMemoryBlocks(MemoryBlock *leftBlock);

    bool isValidBlock(const MemoryBlock *block) const;
    bool isValidBlockFast(const MemoryBlock *block) const;

private:
    size_t memoryPoolSize = 0;
    T *memoryPool = nullptr;

private:
    struct MemoryBlock
    {
        MemoryBlock *next = nullptr;
        size_t size = 0;
        bool isFree = true;
    };
};

#include "FreeListAllocator.inl"

#endif

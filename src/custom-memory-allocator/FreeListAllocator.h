#ifndef FREELISTALLOCATOR_H
#define FREELISTALLOCATOR_H

#include <stddef.h>


class FreeListAllocator
{
private:
    struct MemoryBlock;

public:
    FreeListAllocator(size_t inMemoryPoolSize);
    ~FreeListAllocator();

    void *allocate(size_t size);
    void deallocate(void *ptr);

    bool isValid(const void *ptr) const;
    bool isValidFast(const void *ptr) const;

private:
    void splitMemoryBlock(MemoryBlock *leftBlock, size_t leftBlockSize);
    void mergeMemoryBlocks(MemoryBlock *leftBlock);

    bool isValidBlock(const MemoryBlock *block) const;
    bool isValidBlockFast(const MemoryBlock *block) const;

private:
    size_t memoryPoolSize = 0;
    void *memoryPool = nullptr;

private:
    struct MemoryBlock
    {
        MemoryBlock *next = nullptr;
        size_t size = 0;
        bool isFree = true;
    };
};

#endif

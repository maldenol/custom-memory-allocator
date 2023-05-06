#ifndef FREELISTALLOCATOR_H
#define FREELISTALLOCATOR_H

#include <stddef.h>
#include <type_traits>


#define FLA_DONT_ALIGN_BY_MEMORY_BLOCKS   0x00
#define FLA_ALIGN_BY_MEMORY_BLOCKS        0x01
#define FLA_ALLOCATION_STRATEGY_FIRST_FIT 0x01
#define FLA_ALLOCATION_STRATEGY_NEXT_FIT  0x02
#define FLA_ALLOCATION_STRATEGY_BEST_FIT  0x03
#define FLA_SINGLE_MEMORY_BLOCKS          0x00
#define FLA_DOUBLE_MEMORY_BLOCKS          0x01


struct FreeListAllocatorMode
{
    bool alignByMemoryBlocks = FLA_ALIGN_BY_MEMORY_BLOCKS;
    char allocationStrategy = FLA_ALLOCATION_STRATEGY_FIRST_FIT;
    bool doubleMemoryBlocks = FLA_DOUBLE_MEMORY_BLOCKS;
};


template<typename T, FreeListAllocatorMode Mode>
class FreeListAllocator
{
private:
    struct SingleMemoryBlock;
    struct DoubleMemoryBlock;

    typedef typename std::conditional<Mode.doubleMemoryBlocks == FLA_DOUBLE_MEMORY_BLOCKS, DoubleMemoryBlock, SingleMemoryBlock>::type MemoryBlock;

public:
    FreeListAllocator(size_t inMemoryPoolSize);
    ~FreeListAllocator();

    T *allocate(size_t size);
    void deallocate(T *ptr);

    bool isValid(const T *ptr) const;
    bool isValidFast(const T *ptr) const;

private:
    T *allocateFirstFit(size_t size);
    T *allocateNextFit(size_t size);
    T *allocateBestFit(size_t size);

    void splitMemoryBlock(MemoryBlock *leftBlock, size_t leftBlockSize);
    void mergeSingleMemoryBlocks(MemoryBlock *leftBlock);
    void mergeDoubleMemoryBlocks(MemoryBlock *block);

    bool isValidBlock(const MemoryBlock *block) const;
    bool isValidBlockFast(const MemoryBlock *block) const;

private:
    T *memoryPool = nullptr;

    size_t memoryPoolSize = 0;

    MemoryBlock *currentNextFitBlock = nullptr;

private:
    struct SingleMemoryBlock
    {
        size_t size = 0;
        bool isFree = true;
        MemoryBlock *next = nullptr;
    };

    struct DoubleMemoryBlock : SingleMemoryBlock
    {
        MemoryBlock *previous = nullptr;
    };
};

#include "FreeListAllocator.inl"

#endif

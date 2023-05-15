#include "FreeList.h"

#include <assert.h>
#include <stdlib.h>
#include <utility>


template<typename T, FreeListMode Mode>
FreeList<T, Mode>::FreeList(size_t inMemoryPoolSize)
    : memoryPool{static_cast<T *>(malloc(inMemoryPoolSize * sizeof(T)))},
      memoryPoolSize{memoryPool ? inMemoryPoolSize : 0}
{
    MemoryBlock *const firstBlock = reinterpret_cast<MemoryBlock *>(memoryPool);
    firstBlock->size = memoryPoolSize - sizeof(MemoryBlock);
    firstBlock->isFree = true;
    firstBlock->next = nullptr;
    if constexpr (Mode.doubleMemoryBlocks == FLA_DOUBLE_MEMORY_BLOCKS)
    {
        firstBlock->previous = nullptr;
    }

    currentNextFitBlock = reinterpret_cast<MemoryBlock *>(memoryPool);
}

template<typename T, FreeListMode Mode>
FreeList<T, Mode>::FreeList(FreeList<T, Mode> &&other)
    : memoryPool{std::exchange(other.memoryPool, nullptr)},
      memoryPoolSize{std::exchange(other.memoryPoolSize, 0)},
      currentNextFitBlock{std::exchange(other.currentNextFitBlock, nullptr)}
{}

template<typename T, FreeListMode Mode>
FreeList<T, Mode> &FreeList<T, Mode>::operator=(FreeList<T, Mode> &&other)
{
    std::swap(memoryPool, other.memoryPool);
    std::swap(memoryPoolSize, other.memoryPoolSize);
    std::swap(currentNextFitBlock, other.currentNextFitBlock);

    return *this;
}

template<typename T, FreeListMode Mode>
FreeList<T, Mode>::~FreeList()
{
    free(memoryPool);
}

template<typename T, FreeListMode Mode>
T *FreeList<T, Mode>::allocate(size_t size)
{
    if (size > 0)
    {
        if constexpr (Mode.allocationStrategy == FLA_ALLOCATION_STRATEGY_FIRST_FIT)
        {
            return allocateFirstFit(size);
        }
        else if constexpr (Mode.allocationStrategy == FLA_ALLOCATION_STRATEGY_NEXT_FIT)
        {
            return allocateNextFit(size);
        }
        else if constexpr (Mode.allocationStrategy ==  FLA_ALLOCATION_STRATEGY_BEST_FIT)
        {
            return allocateBestFit(size);
        }
    }

    return nullptr;
}

template<typename T, FreeListMode Mode>
void FreeList<T, Mode>::deallocate(T *ptr)
{
    if (!isValid(ptr)) return;

    MemoryBlock *const block = reinterpret_cast<MemoryBlock *>(ptr) - 1;

    block->isFree = true;

    if constexpr (Mode.doubleMemoryBlocks == FLA_DOUBLE_MEMORY_BLOCKS)
    {
        mergeDoubleMemoryBlocks(static_cast<MemoryBlock *>(block));
    }
    else
    {
        mergeSingleMemoryBlocks(reinterpret_cast<MemoryBlock *>(memoryPool));
    }
}

template<typename T, FreeListMode Mode>
bool FreeList<T, Mode>::isValid(const T *ptr) const
{
    if (!ptr) return false;

    const MemoryBlock *const block = reinterpret_cast<const MemoryBlock *>(ptr) - 1;

    return isValidBlock(block) && !block->isFree;
}

template<typename T, FreeListMode Mode>
bool FreeList<T, Mode>::isValidFast(const T *ptr) const
{
    if (!ptr) return false;

    const MemoryBlock *const block = reinterpret_cast<const MemoryBlock *>(ptr) - 1;

    return isValidBlockFast(block) && !block->isFree;
}

template<typename T, FreeListMode Mode>
bool FreeList<T, Mode>::isEmpty() const
{
    MemoryBlock *block = reinterpret_cast<MemoryBlock *>(memoryPool);

    return !block->next && block->isFree;
}

template<typename T, FreeListMode Mode>
T *FreeList<T, Mode>::allocateFirstFit(size_t size)
{
    MemoryBlock *currentBlock = reinterpret_cast<MemoryBlock *>(memoryPool);

    while (true)
    {
        if (currentBlock->isFree && currentBlock->size >= size)
        {
            splitMemoryBlock(currentBlock, size);

            currentBlock->isFree = false;

            return reinterpret_cast<T *>(currentBlock + 1);
        }

        MemoryBlock *const nextBlock = currentBlock->next;

        if (!nextBlock) return nullptr;

        currentBlock = nextBlock;
    }

    return nullptr;
}

template<typename T, FreeListMode Mode>
T *FreeList<T, Mode>::allocateNextFit(size_t size)
{
    MemoryBlock *firstBlock = currentNextFitBlock;

    do
    {
        MemoryBlock *const nextBlock = currentNextFitBlock->next;

        if (currentNextFitBlock->isFree && currentNextFitBlock->size >= size)
        {
            firstBlock = nextBlock ? nextBlock : reinterpret_cast<MemoryBlock *>(memoryPool);

            splitMemoryBlock(currentNextFitBlock, size);

            currentNextFitBlock->isFree = false;

            return reinterpret_cast<T *>(currentNextFitBlock + 1);
        }

        currentNextFitBlock = nextBlock ? nextBlock : reinterpret_cast<MemoryBlock *>(memoryPool);
    }
    while (currentNextFitBlock != firstBlock);

    return nullptr;
}

template<typename T, FreeListMode Mode>
T *FreeList<T, Mode>::allocateBestFit(size_t size)
{
    MemoryBlock *currentBlock = reinterpret_cast<MemoryBlock *>(memoryPool);
    MemoryBlock *bestBlock = nullptr;

    while (currentBlock)
    {
        if (currentBlock->isFree && currentBlock->size >= size)
        {
            if (!bestBlock || currentBlock->size < bestBlock->size)
            {
                bestBlock = currentBlock;
            }
        }

        currentBlock = currentBlock->next;
    }

    if (bestBlock)
    {
        splitMemoryBlock(bestBlock, size);

        bestBlock->isFree = false;

        return reinterpret_cast<T *>(bestBlock + 1);
    }

    return nullptr;
}

template<typename T, FreeListMode Mode>
void FreeList<T, Mode>::splitMemoryBlock(MemoryBlock *leftBlock, size_t leftBlockSize)
{
    assert(isValidBlock(leftBlock));
    assert(leftBlockSize > 0);

    size_t requiredLeftBlockSize = leftBlockSize;
    if constexpr (Mode.alignByMemoryBlocks == FLA_ALIGN_BY_MEMORY_BLOCKS)
    {
        constexpr size_t memoryBlockAlignment = alignof(MemoryBlock);
        const size_t alignmentRemainder = requiredLeftBlockSize % memoryBlockAlignment;
        if (alignmentRemainder)
        {
            requiredLeftBlockSize += memoryBlockAlignment - alignmentRemainder;
        }
    }

    if (leftBlock->size < requiredLeftBlockSize + sizeof(MemoryBlock) + 1) return;

    MemoryBlock *const rightBlock = reinterpret_cast<MemoryBlock *>(reinterpret_cast<char *>(leftBlock) + sizeof(MemoryBlock) + requiredLeftBlockSize);
    rightBlock->size = leftBlock->size - requiredLeftBlockSize - sizeof(MemoryBlock);
    rightBlock->isFree = true;
    rightBlock->next = leftBlock->next;
    if constexpr (Mode.doubleMemoryBlocks == FLA_DOUBLE_MEMORY_BLOCKS)
    {
        rightBlock->previous = leftBlock;
    }

    leftBlock->size = requiredLeftBlockSize;
    leftBlock->next = rightBlock;
}

template<typename T, FreeListMode Mode>
void FreeList<T, Mode>::mergeSingleMemoryBlocks(MemoryBlock *leftBlock)
{
    assert(isValidBlock(leftBlock));

    MemoryBlock *const rightBlock = leftBlock->next;

    if (!rightBlock) return;

    if (!leftBlock->isFree || !rightBlock->isFree)
    {
        mergeSingleMemoryBlocks(rightBlock);
        return;
    }

    leftBlock->size += sizeof(MemoryBlock) + rightBlock->size;
    leftBlock->next = rightBlock->next;

    mergeSingleMemoryBlocks(leftBlock);
}

template<typename T, FreeListMode Mode>
void FreeList<T, Mode>::mergeDoubleMemoryBlocks(MemoryBlock *block)
{
    assert(isValidBlock(block));
    assert(block->isFree);

    MemoryBlock *const leftBlock = block->previous;
    MemoryBlock *const rightBlock = block->next;

    if (leftBlock && leftBlock->isFree)
    {
        leftBlock->size += sizeof(MemoryBlock) + block->size;
        leftBlock->next = block->next;

        if (rightBlock)
        {
            rightBlock->previous = leftBlock;
        }

        block = leftBlock;
    }

    if (rightBlock && rightBlock->isFree)
    {
        block->size += sizeof(MemoryBlock) + rightBlock->size;
        block->next = rightBlock->next;

        if (rightBlock->next)
        {
            rightBlock->next->previous = block;
        }
    }
}

template<typename T, FreeListMode Mode>
bool FreeList<T, Mode>::isValidBlock(const MemoryBlock *block) const
{
    if (!block) return false;

    const MemoryBlock *currentBlock = reinterpret_cast<MemoryBlock *>(memoryPool);

    while (true)
    {
        if (block == currentBlock) return true;

        const MemoryBlock *const nextBlock = currentBlock->next;

        if (!nextBlock) return false;

        currentBlock = nextBlock;
    }

    return false;
}

template<typename T, FreeListMode Mode>
bool FreeList<T, Mode>::isValidBlockFast(const MemoryBlock *block) const
{
    return reinterpret_cast<unsigned long int>(block) >= reinterpret_cast<unsigned long int>(memoryPool) &&
           reinterpret_cast<unsigned long int>(block) < reinterpret_cast<unsigned long int>(memoryPool) + reinterpret_cast<unsigned long int>(memoryPoolSize);
}

#include "FreeListAllocator.h"

#include <assert.h>
#include <stdlib.h>


template<typename T>
FreeListAllocator<T>::FreeListAllocator(size_t inMemoryPoolSize)
    : memoryPoolSize{inMemoryPoolSize},
    memoryPool{static_cast<T *>(malloc(memoryPoolSize * sizeof(T)))}
{
    MemoryBlock *const firstBlock = reinterpret_cast<MemoryBlock *>(memoryPool);
    firstBlock->next = nullptr;
    firstBlock->size = memoryPoolSize - sizeof(MemoryBlock);
    firstBlock->isFree = true;
}

template<typename T>
FreeListAllocator<T>::~FreeListAllocator()
{
    free(memoryPool);
}

template<typename T>
T *FreeListAllocator<T>::allocate(size_t size)
{
    if (size > 0)
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
    }

    return nullptr;
}

template<typename T>
void FreeListAllocator<T>::deallocate(T *ptr)
{
    if (!isValid(ptr)) return;

    MemoryBlock *const block = reinterpret_cast<MemoryBlock *>(ptr) - 1;

    block->isFree = true;

    mergeMemoryBlocks(reinterpret_cast<MemoryBlock *>(memoryPool));
}

template<typename T>
bool FreeListAllocator<T>::isValid(const T *ptr) const
{
    if (!ptr) return false;

    const MemoryBlock *const block = reinterpret_cast<const MemoryBlock *>(ptr) - 1;

    return isValidBlock(block) && !block->isFree;
}

template<typename T>
bool FreeListAllocator<T>::isValidFast(const T *ptr) const
{
    if (!ptr) return false;

    const MemoryBlock *const block = reinterpret_cast<const MemoryBlock *>(ptr) - 1;

    return isValidBlockFast(block) && !block->isFree;
}

template<typename T>
void FreeListAllocator<T>::splitMemoryBlock(MemoryBlock *leftBlock, size_t leftBlockSize)
{
    assert(isValidBlock(leftBlock));
    assert(leftBlockSize > 0);

    if (leftBlock->size < leftBlockSize + sizeof(MemoryBlock) + 1) return;

    MemoryBlock *const rightBlock = reinterpret_cast<MemoryBlock *>(reinterpret_cast<char *>(leftBlock) + sizeof(MemoryBlock) + leftBlockSize);
    rightBlock->next = leftBlock->next;
    rightBlock->size = leftBlock->size - leftBlockSize - sizeof(MemoryBlock);
    rightBlock->isFree = true;

    leftBlock->next = rightBlock;
    leftBlock->size = leftBlockSize;
}

template<typename T>
void FreeListAllocator<T>::mergeMemoryBlocks(MemoryBlock *leftBlock)
{
    assert(isValidBlock(leftBlock));

    MemoryBlock *const rightBlock = leftBlock->next;

    if (!rightBlock) return;

    if (!leftBlock->isFree || !rightBlock->isFree)
    {
        mergeMemoryBlocks(rightBlock);
        return;
    }

    leftBlock->next = rightBlock->next;
    leftBlock->size += sizeof(MemoryBlock) + rightBlock->size;

    mergeMemoryBlocks(leftBlock);
}

template<typename T>
bool FreeListAllocator<T>::isValidBlock(const MemoryBlock *block) const
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

template<typename T>
bool FreeListAllocator<T>::isValidBlockFast(const MemoryBlock *block) const
{
    return reinterpret_cast<unsigned long int>(block) >= reinterpret_cast<unsigned long int>(memoryPool) &&
           reinterpret_cast<unsigned long int>(block) < reinterpret_cast<unsigned long int>(memoryPool) + reinterpret_cast<unsigned long int>(memoryPoolSize);
}

#include "FreeListAllocator.h"

#include <assert.h>
#include <stdlib.h>


FreeListAllocator::FreeListAllocator(size_t inMemoryPoolSize)
    : memoryPoolSize{inMemoryPoolSize},
      memoryPool{malloc(memoryPoolSize)}
{
    MemoryBlock *const firstBlock = static_cast<MemoryBlock *>(memoryPool);
    firstBlock->next = nullptr;
    firstBlock->size = memoryPoolSize - sizeof(MemoryBlock);
    firstBlock->isFree = true;
}

FreeListAllocator::~FreeListAllocator()
{
    free(memoryPool);
}

void *FreeListAllocator::allocate(size_t size)
{
    if (size > 0)
    {
        MemoryBlock *currentBlock = static_cast<MemoryBlock *>(memoryPool);

        while (true)
        {
            if (currentBlock->isFree && currentBlock->size >= size)
            {
                splitMemoryBlock(currentBlock, size);

                currentBlock->isFree = false;

                return currentBlock + 1;
            }

            MemoryBlock *const nextBlock = currentBlock->next;

            if (!nextBlock) return nullptr;

            currentBlock = nextBlock;
        }
    }

    return nullptr;
}

void FreeListAllocator::deallocate(void *ptr)
{
    if (!isValid(ptr)) return;

    MemoryBlock *const block = static_cast<MemoryBlock *>(ptr) - 1;

    block->isFree = true;

    mergeMemoryBlocks(static_cast<MemoryBlock *>(memoryPool));
}

bool FreeListAllocator::isValid(const void *ptr) const
{
    if (!ptr) return false;

    const MemoryBlock *const block = static_cast<const MemoryBlock *>(ptr) - 1;

    return isValidBlock(block) && !block->isFree;
}

bool FreeListAllocator::isValidFast(const void *ptr) const
{
    if (!ptr) return false;

    const MemoryBlock *const block = static_cast<const MemoryBlock *>(ptr) - 1;

    return isValidBlockFast(block) && !block->isFree;
}

void FreeListAllocator::splitMemoryBlock(MemoryBlock *leftBlock, size_t leftBlockSize)
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

void FreeListAllocator::mergeMemoryBlocks(MemoryBlock *leftBlock)
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

bool FreeListAllocator::isValidBlock(const MemoryBlock *block) const
{
    if (!block) return false;

    const MemoryBlock *currentBlock = static_cast<MemoryBlock *>(memoryPool);

    while (true)
    {
        if (block == currentBlock) return true;

        const MemoryBlock *const nextBlock = currentBlock->next;

        if (!nextBlock) return false;

        currentBlock = nextBlock;
    }

    return false;
}

bool FreeListAllocator::isValidBlockFast(const MemoryBlock *block) const
{
    return reinterpret_cast<unsigned long int>(block) >= reinterpret_cast<unsigned long int>(memoryPool) &&
           reinterpret_cast<unsigned long int>(block) < reinterpret_cast<unsigned long int>(memoryPool) + reinterpret_cast<unsigned long int>(memoryPoolSize);
}

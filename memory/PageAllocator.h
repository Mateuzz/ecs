#pragma once

#include <cstddef>
#include <cstdlib>


// @TODO not implemented
class PageAllocator {
public:
    PageAllocator() {}
    ~PageAllocator() {}

    void* allocate(size_t size)
    {
        return malloc(size);
    }

    void deallocate(void* memory)
    {

    }

    void clear()
    {

    }
};

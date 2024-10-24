#include "AtomicPtrSwap.hpp"


// =============================================================================
void AtomicPtrSwap::write(std::vector<char>&& data)
{
    auto dataPtr = std::make_shared<std::vector<char>>(data);
    atomDataPtr.store(dataPtr, std::memory_order_release);
}

// =============================================================================
std::shared_ptr<std::vector<char>> AtomicPtrSwap::read()
{
    return atomDataPtr.load(std::memory_order_acquire);
}


#ifndef ATOMIC_PTR_SWAP
#define ATOMIC_PTR_SWAP

#include <vector>
#include <memory>
#include <atomic>

class AtomicPtrSwap {
public:
    void write(std::vector<char>&& data);
    std::shared_ptr<std::vector<char>> read();
private:
    std::atomic<std::shared_ptr<std::vector<char>>> atomDataPtr;
};

#endif // !ATOMIC_PTR_SWAP

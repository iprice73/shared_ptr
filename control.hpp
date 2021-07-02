#pragma once

#include <stddef.h>
#include <atomic>
#include <functional>

template <class T>
using Deleter = std::function<void(T* ptr)>;
auto default_deleter = [](auto* ptr) { delete ptr; };

template <class T>
class control_block {
public:
    control_block() = default;
    control_block(Deleter<T> deleter, size_t shared_cnt = 1);

    size_t get_shared_refs() const { return shared_refs; }
    size_t get_weak_refs() const { return weak_refs; }
    Deleter<T> get_deleter() const { return deleter_; }

    void increment_shared_refs() noexcept { shared_refs++; };
    void increment_weak_refs() noexcept { weak_refs++; };
    void decrement_shared_refs() noexcept { if (shared_refs) { shared_refs--; } };
    void decrement_weak_refs() noexcept { if (weak_refs) { weak_refs--; } };

private:
    Deleter<T> deleter_{default_deleter};
    std::atomic<size_t> shared_refs{1};
    std::atomic<size_t> weak_refs{0};
};

template <class T>
control_block<T>::control_block(Deleter<T> deleter, size_t shared_cnt)
    : deleter_(deleter), shared_refs(shared_cnt) {
}

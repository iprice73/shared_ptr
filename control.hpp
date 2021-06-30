#pragma once

#include <stddef.h>
#include <atomic>

template <class T>
class control_block {
public:
    int get_shared_refs() const { return shared_refs; }
    int get_weak_refs() const { return weak_refs; }

    void increment_shared_refs() noexcept;
    void increment_weak_refs() noexcept;
    void decrement_shared_refs() noexcept;
    void decrement_weak_refs() noexcept;

private:
    std::atomic<size_t> shared_refs{1};
    std::atomic<size_t> weak_refs{};
};

template <class T>
void control_block<T>::increment_shared_refs() noexcept {
    shared_refs++;
}

template <class T>
void control_block<T>::increment_weak_refs() noexcept {
    weak_refs++;
}

template <class T>
void control_block<T>::decrement_shared_refs() noexcept {
    shared_refs--;
}

template <class T>
void control_block<T>::decrement_weak_refs() noexcept {
    weak_refs--;
}

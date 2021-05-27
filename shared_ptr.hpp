#pragma once

#include <stddef.h>
#include <atomic>

class control_block {
public:
private:
    std::atomic<size_t> shared_refs{};
    std::atomic<size_t> weak_refs{};
};

template <class T>
class shared_ptr {
public:

    // constructors
    shared_ptr(T* new_ptr = nullptr);
    shared_ptr(const shared_ptr<T>& other_ptr);
    shared_ptr(shared_ptr<T>&& other_ptr);

    // destructor
    ~shared_ptr();


    // methods
    T* get() const;
    void reset(T* other_ptr);
    void swap(shared_ptr<T>& other_ptr);
    size_t use_count();

    // operators
    shared_ptr<T>& operator=(const shared_ptr<T>& other_ptr);
    shared_ptr<T>& operator=(shared_ptr<T>&& other_ptr);
    T operator*();
    T operator->();
    operator bool();

private:
    T* m_ptr{};
    control_block* m_block{};
};

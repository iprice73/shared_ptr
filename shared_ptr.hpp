#pragma once

#include <stddef.h>
#include <atomic>

template <class T>
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
    shared_ptr() = default;
    shared_ptr(T* new_ptr);
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
    T& operator*() const;
    T* operator->() const;
    operator bool();

private:
    T* m_ptr{};
    control_block<T>* m_block{};
};

// Constructors

template <class T>
shared_ptr<T>::shared_ptr(T* new_ptr)
    : m_ptr(new_ptr) {}

template <class T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& other_ptr) {}

template <class T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& other_ptr) {}

// Destructor

template<class T>
shared_ptr<T>::~shared_ptr<T>() {
    delete m_ptr;
}

template <class T>
T* shared_ptr<T>::get() const {
    return m_ptr;
}

template<class T>
T& shared_ptr<T>::operator*() const {
    return *m_ptr;
}

template<class T>
T* shared_ptr<T>::operator->() const {
    return m_ptr;
}

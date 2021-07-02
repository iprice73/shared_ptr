#pragma once

#include "control.hpp"

template <class T>
class shared_ptr {
public:
    // Constructors
    explicit shared_ptr(T* new_ptr = nullptr, Deleter<T> deleter = default_deleter);
    shared_ptr(const shared_ptr<T>& other_ptr);
    shared_ptr(shared_ptr<T>&& other_ptr) noexcept;

    // Destructor
    ~shared_ptr();

    // Methods
    T* get() const;
    void reset(T* other_ptr = nullptr, Deleter<T> deleter = default_deleter);
    void swap(shared_ptr<T>& other_ptr);
    size_t use_count() const { return m_block->get_shared_refs(); }

    // Operators
    shared_ptr<T>& operator=(const shared_ptr<T>& other_ptr);
    shared_ptr<T>& operator=(shared_ptr<T>&& other_ptr) noexcept;
    T& operator*() const;
    T* operator->() const;
    operator bool() const;

    // Others
    template <class Ts, class... Args>
    friend shared_ptr<Ts> make_shared(Args&&... args);

private:
    T* m_ptr{nullptr};
    control_block<T>* m_block{nullptr};

    void release_memory();
};

// Constructors

template <class T>
shared_ptr<T>::shared_ptr(T* new_ptr, Deleter<T> deleter)
    : m_ptr(new_ptr), m_block((m_ptr) ? new control_block<T>(deleter) : new control_block<T>(deleter, 0)) {
}

template <class T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& other_ptr)
    : m_ptr(other_ptr.m_ptr), m_block(other_ptr.m_block) {
    m_block->increment_shared_refs();
}

template <class T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& other_ptr) noexcept
    : m_ptr(other_ptr.m_ptr), m_block(other_ptr.m_block) {
    other_ptr.m_ptr = nullptr;
    other_ptr.m_block = nullptr;
}

// Destructor

template <class T>
shared_ptr<T>::~shared_ptr<T>() {
    release_memory();
}

// Methods

template <class T>
void shared_ptr<T>::release_memory() {
    if (m_block) {
        m_block->decrement_shared_refs();
        if (!m_block->get_shared_refs()) {
            m_block->get_deleter()(m_ptr);
            delete m_block;
        }
    }
}

template <class T>
T* shared_ptr<T>::get() const {
    return m_ptr;
}

template <class T>
void shared_ptr<T>::swap(shared_ptr<T>& other) {
    auto tmp = other.m_ptr;
    other.m_ptr = m_ptr;
    m_ptr = tmp;
}

template <class T>
void shared_ptr<T>::reset(T* other_ptr, Deleter<T> deleter) {
    release_memory();
    m_ptr = other_ptr;
    m_block = (m_ptr) ? new control_block<T>(deleter) : new control_block<T>(deleter, 0);
}

// Operators

template <class T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& other_ptr) {
    if (&other_ptr != this) {
        release_memory();
        m_ptr = other_ptr.m_ptr;
        m_block = other_ptr.m_block;
        m_block->increment_shared_refs();
    }
    return *this;
}

template <class T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& other_ptr) noexcept {
    if (&other_ptr != this) {
        release_memory();
        m_ptr = other_ptr.m_ptr;
        m_block = other_ptr.m_block;
        other_ptr.m_ptr = nullptr;
        other_ptr.m_block = nullptr;
    }
    return *this;
}

template <class T>
T& shared_ptr<T>::operator*() const {
    return *m_ptr;
}

template <class T>
T* shared_ptr<T>::operator->() const {
    return m_ptr;
}

template <class T>
shared_ptr<T>::operator bool() const {
    return (m_ptr) ? true : false;
}

// Others

template <class Ts, class... Args>
shared_ptr<Ts> make_shared(Args&&... args) {
    return shared_ptr<Ts>(new Ts{std::forward<Args>(args)...});
}

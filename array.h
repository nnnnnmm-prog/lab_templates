#pragma once

#include <cstddef>

template <typename T>
class Array {
public:
    // ======================== Constructors ========================

    Array();                                // empty array, size 0
    explicit Array(size_t size);            // default-constructed elements
    Array(size_t size, const T& value);     // all elements = value

    // =================== Copy and Move ===========================

    Array(const Array& other);
    Array(Array&& other);
    Array& operator=(const Array& other);
    Array& operator=(Array&& other);

    // =================== Destructor ==============================

    ~Array();

    // =================== Element access ==========================

    T&       operator[](size_t index);
    const T& operator[](size_t index) const;

    T&       front();
    const T& front() const;

    T&       back();
    const T& back() const;

    T*       data();
    const T* data() const;

    // =================== Capacity ================================

    size_t size() const;
    bool   empty() const;

    // =================== Operations ==============================

    void fill(const T& value);
    void swap(Array& other);

    // =================== Comparison ==============================

    bool operator==(const Array& rhs) const;
    bool operator!=(const Array& rhs) const;

private:
    T*     data_ = nullptr;
    size_t size_ = 0;
};

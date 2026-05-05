#pragma once

#include <new>
#include <type_traits>
#include <utility>

template <typename T>
class Optional {
public:
    // ======================== Constructors ========================

    Optional();
    Optional(const T& value);
    Optional(T&& value);

    // =================== Copy and Move ===========================

    Optional(const Optional& other);
    Optional(Optional&& other);
    Optional& operator=(const Optional& other);
    Optional& operator=(Optional&& other);
    Optional& operator=(const T& value);
    Optional& operator=(T&& value);

    // =================== Destructor ==============================

    ~Optional();

    // =================== Observers ===============================

    bool has_value() const;
    explicit operator bool() const;

    T&       value();
    const T& value() const;

    T        value_or(const T& default_value) const;

    T&       operator*();
    const T& operator*() const;

    T*       operator->();
    const T* operator->() const;

    // =================== Modifiers ===============================

    template <typename... Args>
    T& emplace(Args&&... args);

    void reset();
    void swap(Optional& other);

    // =================== Comparison ==============================

    bool operator==(const Optional& rhs) const;
    bool operator!=(const Optional& rhs) const;

private:
    alignas(T) unsigned char storage_[sizeof(T)];
    bool has_value_ = false;
};

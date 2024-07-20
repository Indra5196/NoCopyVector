#ifndef MyNoCopyVector_H
#define MyNoCopyVector_H

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <memory_resource>
#include <algorithm> // For std::move, std::swap
#include <limits>    // For std::numeric_limits
#include <memory>    // For std::uninitialized_copy

// Enclosed in unnamed namespace so that its contents are not visible outside the header
namespace {
template <typename T>
struct CustomDeleter {
    std::pmr::polymorphic_allocator<T> allocator;

    void operator()(T* ptr) {
        // Explicitly call the destructor and deallocate the memory using the allocator
        std::allocator_traits<std::pmr::polymorphic_allocator<T>>::destroy(allocator, ptr);
        std::allocator_traits<std::pmr::polymorphic_allocator<T>>::deallocate(allocator, ptr, 1);
    }
};

// Trait to check if T is a primitive type or a pointer
template <typename T>
struct is_primitive_or_pointer : std::integral_constant<bool,
    std::is_fundamental<T>::value || std::is_pointer<T>::value> {};


// Using this wrapper, user can use the pointers in MyNoCopyVector like objects of the type they stored
template<typename T>
class Element {
    private:
    std::unique_ptr<T, CustomDeleter<T>> data_;
    std::pmr::polymorphic_allocator<T> allocator;
    public:
    Element() = delete;
    Element(const T& value, std::unique_ptr<std::pmr::memory_resource> resource) : allocator(*resource) {
        T* rawPtr = allocator.allocate(1);
        std::allocator_traits<std::pmr::polymorphic_allocator<T>>::construct(allocator, rawPtr, value);
        data_(rawPtr, CustomDeleter<T>{allocator});
    }

    Element(Element& elem) {
        T* rawPtr = allocator.allocate(1);
        std::allocator_traits<std::pmr::polymorphic_allocator<T>>::construct(allocator, rawPtr, *elem.data_);
        data_(rawPtr, CustomDeleter<T>{allocator});
    }

    Element& operator=(Element& elem) {
        T* rawPtr = allocator.allocate(1);
        std::allocator_traits<std::pmr::polymorphic_allocator<T>>::construct(allocator, rawPtr, *elem.data_);
        data_(rawPtr, CustomDeleter<T>{allocator});
        return *this;
    }

    Element(Element&& elem) {
        data_ = std::move(elem.data_);
        allocator = std::move(elem.allocator);
    }

    Element& operator=(Element&& elem) {
        data_ = std::move(elem.data_);
        allocator = std::move(elem.allocator);
        return *this;
    }

    T& operator&() {
        return *data_;
    }

    operator T() const {
        return *data_;
    }
};

template <typename T>
class MyNoCopyVector {
public:
    // Types
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using DataType = std::vector<Element<T>>;

    // Constructors
    MyNoCopyVector();
    MyNoCopyVector(size_type count, const T& value);
    MyNoCopyVector(std::initializer_list<T> init);
    MyNoCopyVector(const MyNoCopyVector& other) = delete; // Disable copy constructor
    MyNoCopyVector(MyNoCopyVector&& other) noexcept;

    // Destructor
    ~MyNoCopyVector();

    // Assignment operators
    MyNoCopyVector& operator=(const MyNoCopyVector& other) = delete; // Disable copy assignment
    MyNoCopyVector& operator=(MyNoCopyVector&& other) noexcept;
    MyNoCopyVector& operator=(std::initializer_list<T> ilist);

    // Element access
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    T* data() noexcept;
    const T* data() const noexcept;

    // Iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // Capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void reserve(size_type new_cap);
    size_type capacity() const noexcept;
    void shrink_to_fit();

    // Modifiers
    void clear() noexcept;
    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_type count, const T& value);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);
    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args);
    void push_back(const T& value);
    void push_back(T&& value);
    template <typename... Args>
    reference emplace_back(Args&&... args);
    void pop_back();
    void resize(size_type count);
    void resize(size_type count, const value_type& value);
    void swap(MyNoCopyVector& other) noexcept;

private:
    std::unique_ptr<DataType> data_;
    void reallocate(size_type new_cap);
    std::unique_ptr<std::pmr::unsynchronized_pool_resource> pool;
    std::pmr::pool_options options;
};

}

template <typename T>
using NoCopyVector = typename std::conditional<
    is_primitive_or_pointer<T>::value,
    std::vector<T>,
    MyNoCopyVector<T>
>::type;

#include "NoCopyVector.tpp"

#endif // MyNoCopyVector_H

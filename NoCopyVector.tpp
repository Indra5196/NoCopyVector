#include "MyNoCopyVector.h"
#include <algorithm> // For std::move, std::swap
#include <limits>    // For std::numeric_limits
#include <memory>    // For std::uninitialized_copy

template <typename T>
MyNoCopyVector<T>::MyNoCopyVector() : pool({sizeof(T), 1}) {
    data_ = std::make_unique<DataType>();
}

template <typename T>
MyNoCopyVector<T>::MyNoCopyVector(size_type count, const T& value) : pool(std::make_unique<std::pmr::unsynchronized_pool_resource>({count * sizeof(T), 1})) {
    data_ = std::make_unique<DataType>(count);
    // Allocate memory for an instance of A using the allocator
    for (size_t i = 0; i < count; ++i) {
        data_->emplace_back(value, pool);
    }
}

template <typename T>
MyNoCopyVector<T>::MyNoCopyVector(std::initializer_list<T> init) : pool(std::make_unique<std::pmr::unsynchronized_pool_resource>({init.size() * sizeof(T), 1})) {
    data_ = std::make_unique<DataType>(init.size());
    for (auto i : init)
        data_->emplace_back(i, pool);
}

template <typename T>
MyNoCopyVector<T>::MyNoCopyVector(MyNoCopyVector&& other) noexcept {
    data_ = std::move(other.data_);
    pool = std::move(other.pool);
}

template <typename T>
MyNoCopyVector<T>::~MyNoCopyVector() {}

template <typename T>
MyNoCopyVector<T>& MyNoCopyVector<T>::operator=(MyNoCopyVector&& other) noexcept {
    if (this != &other) {
        data_ = std::move(other.data_);
        pool = std::move(other.pool);
    }
    return *this;
}

template <typename T>
MyNoCopyVector<T>& MyNoCopyVector<T>::operator=(std::initializer_list<T> ilist) {
    MyNoCopyVector temp(ilist);
    swap(temp);
    return *this;
}

template <typename T>
typename MyNoCopyVector<T>::reference MyNoCopyVector<T>::at(size_type pos) {
    return *data_->at(pos);
}

template <typename T>
typename MyNoCopyVector<T>::const_reference MyNoCopyVector<T>::at(size_type pos) const {
    return *data_->at(pos);
}

template <typename T>
typename MyNoCopyVector<T>::reference MyNoCopyVector<T>::operator[](size_type pos) {
    return *data_->at(pos);
}

template <typename T>
typename MyNoCopyVector<T>::const_reference MyNoCopyVector<T>::operator[](size_type pos) const {
    return *data_->at(pos);
}

template <typename T>
typename MyNoCopyVector<T>::reference MyNoCopyVector<T>::front() {
    return *data_->front();
}

template <typename T>
typename MyNoCopyVector<T>::const_reference MyNoCopyVector<T>::front() const {
    return *data_->front();
}

template <typename T>
typename MyNoCopyVector<T>::reference MyNoCopyVector<T>::back() {
    return *data_->back();
}

template <typename T>
typename MyNoCopyVector<T>::const_reference MyNoCopyVector<T>::back() const {
    return *data_->back();
}

template <typename T>
T* MyNoCopyVector<T>::data() noexcept {
    return data_.get();
}

template <typename T>
const T* MyNoCopyVector<T>::data() const noexcept {
    return data_.get();
}

template <typename T>
typename MyNoCopyVector<T>::iterator MyNoCopyVector<T>::begin() noexcept {
    return *data_->begin();
}

template <typename T>
typename MyNoCopyVector<T>::const_iterator MyNoCopyVector<T>::begin() const noexcept {
    return *data_->begin();
}

template <typename T>
typename MyNoCopyVector<T>::const_iterator MyNoCopyVector<T>::cbegin() const noexcept {
    return *data_->begin();
}

template <typename T>
typename MyNoCopyVector<T>::iterator MyNoCopyVector<T>::end() noexcept {
    return *data_->end();
}

template <typename T>
typename MyNoCopyVector<T>::const_iterator MyNoCopyVector<T>::end() const noexcept {
    return *data_->end();
}

template <typename T>
typename MyNoCopyVector<T>::const_iterator MyNoCopyVector<T>::cend() const noexcept {
    return *data_->end();
}

template <typename T>
bool MyNoCopyVector<T>::empty() const noexcept {
    return data_->empty();
}

template <typename T>
typename MyNoCopyVector<T>::size_type MyNoCopyVector<T>::size() const noexcept {
    return data_->size();
}

template <typename T>
typename MyNoCopyVector<T>::size_type MyNoCopyVector<T>::max_size() const noexcept {
    return data_->max_size();
}

template <typename T>
void MyNoCopyVector<T>::reserve(size_type new_cap) {
    data_->reserve(new_cap);
}

template <typename T>
typename MyNoCopyVector<T>::size_type MyNoCopyVector<T>::capacity() const noexcept {
    return capacity_;
}

template <typename T>
void MyNoCopyVector<T>::shrink_to_fit() {
    data_->shrink_to_fit();
}

template <typename T>
void MyNoCopyVector<T>::clear() noexcept {
    data_->clear();
}

template <typename T>
typename MyNoCopyVector<T>::iterator MyNoCopyVector<T>::insert(const_iterator pos, const T& value) {
    data_->insert(pos, Element<T>(value, pool));
}

template <typename T>
typename MyNoCopyVector<T>::iterator MyNoCopyVector<T>::insert(const_iterator pos, T&& value) {
    data_->insert(pos, Element<T>(value, pool));
}

template <typename T>
typename MyNoCopyVector<T>::iterator MyNoCopyVector<T>::insert(const_iterator pos, size_type count, const T& value) {
    data_->insert(pos, count, Element<T>(value, pool));
}

template <typename T>
typename MyNoCopyVector<T>::iterator MyNoCopyVector<T>::insert(const_iterator pos, const T& value) {
    for (auto i : init) {
        data_->insert(pos, Element<T>(i, pool));
    }
}

template <typename T>
void MyNoCopyVector<T>::push_back(const T& value) {
    data_->emplace_back(value, pool);
}

template <typename... Args>
reference MyNoCopyVector<T>::emplace_back(Args&&... args);
 {
    data_->emplace_back(args...);
 }
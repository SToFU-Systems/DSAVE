/*
* This Software is part of the SToFU Systems S.L. project and is licensed under the
* SToFU Non-Commercial License Agreement (Based on AGPL).
*
* Use, modification, and distribution are permitted exclusively by private individuals
* for personal, non-commercial purposes. Any use by corporations, organizations,
* institutions, or for any commercial or profit-oriented activity is strictly prohibited
* without prior written permission from SToFU Systems S.L.
*
* This Software may include third-party components under separate licenses. In such cases,
* their terms take precedence for those components.
*
* For full details, disclaimers, and commercial licensing inquiries, please refer to:
* https://stofu.io or the LICENSE file included with this Software.
*
* All implied warranties, including merchantability and fitness, are disclaimed.
* Neither copyright owners nor contributors are liable for damages, including
* loss of data, profits, or business interruption, arising from use,
* even if warned of such possibilities.
*
* Commercial Licensing Contact: contacts@stofu.io
*/

#ifndef __THREADSAFEVECTOR_H
#define __THREADSAFEVECTOR_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <algorithm>

template <typename T>
class tsvector
{
public:
    // Constructor
    // If 0 - the size is unlimited; 
    // if not 0, the application will wait for elements to be removed 
    // when the size reaches or exceeds the specified maximum (maxSize).
    tsvector() : maxSize_(0) {} 

    // Delete copy constructor and assignment operator
    tsvector(const tsvector&) = delete;
    tsvector& operator=(const tsvector&) = delete;

    // Allow move constructor and move assignment
    tsvector(tsvector&& other) noexcept
    {
        std::lock_guard<std::mutex> lock(other.mutex_);
        data_ = std::move(other.data_);
        maxSize_ = other.maxSize_;
    }

    tsvector& operator=(tsvector&& other) noexcept
    {
        if (this != &other)
        {
            std::lock_guard<std::mutex> lock_this(mutex_);
            std::lock_guard<std::mutex> lock_other(other.mutex_);
            data_ = std::move(other.data_);
            maxSize_ = other.maxSize_;
        }
        return *this;
    }

    // Add an element to the vector (copy)
    void push_back(const T& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this]() { return maxSize_ == 0 || data_.size() < maxSize_; });
        data_.push_back(value);
        condition_.notify_all();
    }

    // Add an element to the vector (move)
    void push_back(T&& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this]() { return maxSize_ == 0 || data_.size() < maxSize_; });
        data_.push_back(std::move(value));
        condition_.notify_all();
    }

    // Remove and return the last element
    T pop_back()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (data_.empty()) throw std::out_of_range("Vector is empty");
        T value = std::move(data_.back());
        data_.pop_back();
        condition_.notify_all();
        return value;
    }

    // Remove and return element by index
    T pop_by_index(size_t index)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (index >= data_.size()) throw std::out_of_range("Index out of range");
        T value = std::move(data_[index]);
        data_.erase(data_.begin() + index);
        condition_.notify_all();
        return value;
    }

    // Remove and return element by iterator
    T pop_by_iterator(typename std::vector<T>::iterator it)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (it == data_.end()) throw std::out_of_range("Iterator out of range");
        T value = std::move(*it);
        data_.erase(it);
        condition_.notify_all();
        return value;
    }

    // Access an element by index
    T& operator[](size_t index)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_[index];
    }

    const T& operator[](size_t index) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_[index];
    }

    // Size of the vector
    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.size();
    }

    bool empty() const
    {
        return !size();
    }

    // Resize the vector
    void resize(size_t new_size, const T& value = T())
    {
        std::lock_guard<std::mutex> lock(mutex_);
        data_.resize(new_size, value);
        condition_.notify_all();
    }

    // Reserve the vector
    void reserve(size_t reserved_size)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        data_.reserve(reserved_size);
        condition_.notify_all();
    }

    // Iterators
    typename std::vector<T>::iterator begin()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.begin();
    }

    typename std::vector<T>::iterator end()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.end();
    }

    typename std::vector<T>::reverse_iterator rbegin()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.rbegin();
    }

    typename std::vector<T>::reverse_iterator rend()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.rend();
    }

    // Set maximum size
    void set_max(size_t max_size)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        maxSize_ = max_size;
        condition_.notify_all();
    }

private:
    std::vector<T> data_;                // Internal vector
    mutable std::mutex mutex_;           // Mutex for thread safety
    std::condition_variable condition_;  // Condition variable for blocking on size limit
    size_t maxSize_;                     // Maximum size of the vector (0 = unlimited)
};
#endif


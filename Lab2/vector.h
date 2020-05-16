#include <cstdio>
#include <algorithm>
#include <assert.h>

template <typename T>
struct vector
{
    typedef T* iterator;
    typedef T const* const_iterator;

    vector();                               // O(1) nothrow
    vector(vector const&);                  // O(N) strong
    vector& operator=(vector const& other); // O(N) strong

    ~vector();                              // O(N) nothrow


    T& operator[](size_t i);                // O(1) nothrow
    T const& operator[](size_t i) const;    // O(1) nothrow

    T* data();                              // O(1) nothrow
    T const* data() const;                  // O(1) nothrow
    size_t size() const;                    // O(1) nothrow

    T& front();                             // O(1) nothrow
    T const& front() const;                 // O(1) nothrow

    T& back();                              // O(1) nothrow
    T const& back() const;                  // O(1) nothrow
    void push_back(T const&);               // O(1)* strong
    void pop_back();                        // O(1) nothrow

    bool empty() const;                     // O(1) nothrow

    size_t capacity() const;                // O(1) nothrow
    void reserve(size_t);                   // O(N) strong
    void shrink_to_fit();                   // O(N) strong

    void clear();                           // O(N) nothrow

    void swap(vector&);                     // O(1) nothrow

    iterator begin() {
        return data_;
    }

    iterator end() {
        return data_ + size_;
    }

    const_iterator begin() const {
        return data_;
    }

    const_iterator end() const {
        return data_ + size_;
    }

    iterator insert(const_iterator pos, T const&); // O(N) weak

    iterator erase(const_iterator pos);     // O(N) weak

    iterator erase(const_iterator first, const_iterator last); // O(N) weak

private:
    size_t increase_capacity() const;
    void push_back_realloc(T const&);

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

template<typename T>
vector<T>::vector() {
    size_ = 0;
    capacity_ = 0;
    data_ = nullptr;
}

template<typename T>
vector<T>::vector(vector const &other) : vector() {
    T* new_data = nullptr;
    if (other.size_ > 0 ) {
        new_data = static_cast<T*>(operator new(other.size_ * sizeof(T)));
    }
    for (size_t i = 0; i < other.size_; i++) {
        new(new_data + i) T(other.data_[i]);
    }
    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.size_;
}

template<typename T>
vector<T> &vector<T>::operator=(vector const &other) {
    vector<T> new_vector (other);
    swap(new_vector);
    return *this;
}

template<typename T>
vector<T>::~vector() {
    for (size_t i = 0; i < size_; i++) {
        data_[i].~T();
    }
    operator delete(data_);
}

template<typename T>
T &vector<T>::operator[](size_t i) {
    assert(i < size_);
    return data_[i];
}

template<typename T>
T const &vector<T>::operator[](size_t i) const {
    assert(i < size_);
    return data_[i];
}

template<typename T>
T *vector<T>::data() {
    return data_;
}

template<typename T>
T const *vector<T>::data() const {
    return data_;
}

template<typename T>
size_t vector<T>::size() const {
    return size_;
}

template<typename T>
T &vector<T>::front() {
    return data_[0];
}

template<typename T>
T const &vector<T>::front() const {
    return data_[0];
}

template<typename T>
T &vector<T>::back() {
    return data_[size_ - 1];
}

template<typename T>
T const &vector<T>::back() const {
    return data_[size_ - 1];
}

template<typename T>
void vector<T>::push_back(const T &value) {
    if (size_ < capacity_) {
        new (data_ + size_) T(value);
    } else {
        push_back_realloc(value);
    }
    size_++;
}

template<typename T>
void vector<T>::pop_back() {
    assert(size_ != 0);
    data_[size_ - 1].~T();
    size_--;
}

template<typename T>
bool vector<T>::empty() const {
    return size_ == 0;
}

template<typename T>
size_t vector<T>::capacity() const {
    return capacity_;
}

template<typename T>
void vector<T>::reserve(size_t len) {
    if (len > capacity_) {
        T* new_data = static_cast<T*>(operator new(len * sizeof(T)));
        size_t i;
        try {
            for (i = 0; i < size_; i++) {
                new(new_data + i) T(data_[i]);
            }
        } catch (...) {
            for (size_t j = 0; j < i; j++) {
                new_data[j].~T();
            }
            throw;
        }
        this->~vector();
        data_ = new_data;
        capacity_ = len;
    }
}

template<typename T>
void vector<T>::shrink_to_fit() {
    if (size_ == 0) {
        vector <T> empty;
        swap(empty);
    } else if (size_ < capacity_) {
        T *new_data = static_cast<T *>(operator new(size_ * sizeof(T)));
        for (size_t i = 0; i < size_; i++) {
            new(new_data + i) T(data_[i]);
        }
        for (size_t i = 0; i < size_; i++) {
            data_[i].~T();
        }
        operator delete(data_);
        data_ = new_data;
    }
    capacity_ = size_;
}

template<typename T>
void vector<T>::clear() {
    for (size_t i = 0; i < size_; i++) {
        data_[i].~T();
    }
    size_ = 0;
}

template<typename T>
void vector<T>::swap(vector & other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template<typename T>
typename vector<T>::iterator vector<T>::insert(vector::const_iterator pos, const T &value) {
    push_back(value);
    iterator curr = end() - 1;
    while (curr != pos && curr != begin()) {
        std::swap(*curr, *(curr - 1));
        curr--;
    }
    return (pos - begin()) + begin();
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator pos) {
    iterator curr = (pos - begin()) + begin();
    while (curr + 1 != end()) {
        std::swap(*(curr), *(curr + 1));
        curr++;
    }
    pop_back();
    return (pos - begin()) + begin();
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator first, vector::const_iterator last) {
    int del = last - first;
    iterator curr = (first - begin()) + begin();
    while (curr < last) {
        std::swap(*(curr), *(curr + del));
        curr++;
    }
    for (int i = 0; i < del; i++) {
        pop_back();
    }
    return (first - begin()) + begin();
}

template<typename T>
size_t vector<T>::increase_capacity() const {
    return (capacity_ == 0) ? 1 : 2 * capacity_;
}

template<typename T>
void vector<T>::push_back_realloc(const T &value) {
    T to_ins(value);
    capacity_ = increase_capacity();
    reserve(2 * capacity_);
    new (data_ + size_) T(to_ins);
}
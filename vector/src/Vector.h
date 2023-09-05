#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same
#include <iostream>


template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() { 
        if (_capacity == 0) {
            _capacity = 1;
        }
        else {
            _capacity *= 2;
        }
        
        T* array_temp = new T[_capacity];

        for (size_t i = 0; i < _size; i++) {
            array_temp[i] = std::move(array[i]);
        }

        delete [] array;
        array = array_temp;

    }

public:

    //default constructor
    Vector() noexcept { 
        array = nullptr;
        _capacity = 0;
        _size = 0;
        array = nullptr;

    }

    //paramaterized constructor
    Vector(size_t count, const T& value) {
        _capacity = count;
        _size = count;
        array = new T[_capacity];
        for (size_t i = 0; i < _size; i++) {
            array[i] = value;
        }
    }

    explicit Vector(size_t count) : array(new T[count]), _capacity(count), _size(count) {
        for (size_t i = 0; i < _size; i++) {
            array[i] = T();
        }
    }

    Vector(const Vector& other) {
        array = new T[other.capacity()];
        _size = other.size();
        _capacity = other.capacity();
        for (size_t i = 0; i < _size; i++) {
            array[i] = other.array[i];
        }

    }
    
    Vector(Vector&& other) noexcept : array(std::move(other.array)), _size(std::move(other._size)), _capacity(std::move(other._capacity)) {
        other.array = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    ~Vector() {
        delete [] array;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] array;
            array = new T[other._size];
            _size = other._size;
            _capacity = other._capacity;
            for (size_t i = 0; i < _size; i++) {
                array[i] = other.array[i];
            }
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] array;

            array = other.array;
            _size = other._size;
            _capacity = other._capacity;

            other.array = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
        return *this;
    }

    iterator begin() noexcept { return iterator(array); }
    iterator end() noexcept { return iterator(array) + _size; }

    [[nodiscard]] bool empty() const noexcept { return _size == 0; }

    size_t size() const noexcept { 
        return _size;
    }
    size_t capacity() const noexcept {
        return _capacity;
    }

    T& at(size_t pos) {
        if (pos >= _size) throw (std::out_of_range("index is out of range"));
        return array[pos];
    }
    const T& at(size_t pos) const {
        if (pos >= _size) throw (std::out_of_range("index is out of range"));
        return array[pos];
    }

    T& operator[](size_t pos) { return array[pos]; }

    const T& operator[](size_t pos) const { return array[pos]; }

    T& front() { return array[0]; }
    const T& front() const { return array[0]; }
    T& back() { return array[_size - 1]; }
    const T& back() const { return array[_size - 1]; }

    void push_back(const T& value) {
        if (_size == _capacity) {
            grow();
        }
        array[_size] = value;
        _size++;
    }

    void push_back(T&& value) {
        if (_size == _capacity) {
            grow();
        }
        array[_size] = std::move(value);
        _size++;
    }

    void pop_back() {
        if (_size > 0) {
            _size--; //remove last element
        }
    }

    iterator insert(iterator pos, const T& value) {
        size_t index = pos - begin(); //gets the index of the insertion point

        if (_size == _capacity) {
            grow();
        }

        for (size_t i = _size; i > index; --i) {
            array[i] = std::move(array[i - 1]); 
        }
        array[index] = value;
        _size++;
        return iterator(&(array[index]));

    }

    iterator insert(iterator pos, T&& value) {
        size_t index = pos - begin(); //gets the index of the insertion point

        if (_size == _capacity) {
            grow();
        }

        for (size_t i = _size; i > index; --i) {
            array[i] = std::move(array[i - 1]); 
        }
        array[index] = std::move(value);
        _size++;
        return iterator(&(array[index]));
    }

    iterator insert(iterator pos, size_t count, const T& value) {
        
        size_t index = pos - begin(); //index of insertion point
        size_t temp = index;

        while (_size + count >= _capacity) { //grow the vector to be able to hold every element
            grow();
        }
        
        //shift over elements to make space for the elements inserted
        for (size_t i = _size + count - 1; i > index + count - 1; --i) {
            array[i] = std::move(array[i - count]); 
        }

        //adds the number of count elements to the array
        for (size_t i = 0; i < count; i++) {
            array[index] = value;
            index++;
        }

        //increments size and returns the pointer to the new iterator position
        _size += count;
        return iterator(&(array[temp]));
    }

    iterator erase(iterator pos) {
        if (pos < begin() || pos >= end()) {
            return end();
        }
        std::move(pos + 1, end(), pos);
        _size--;
        return pos;
    }

    iterator erase(iterator first, iterator last) {
        //checks if the vector is one element
        if (first == last) {
            return first;
        }
        size_t num = last - first; //distance between the two
        for (iterator temp = first; temp != end() - num; temp++) {
            *temp = *(temp + num); //shifts the elements over to the left
        }
        //corrects the size
        _size -= num;
        return first;

    }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

    private:
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
        pointer ptr;

    public:

        iterator() : ptr(nullptr) {}
        iterator(pointer point) : ptr(point) {}

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept { return *ptr; }
        [[nodiscard]] pointer operator->() const noexcept { return ptr; }

        // Prefix Increment: ++a
        iterator& operator++() noexcept {
            ++ptr;
            return *this;
        }

        // Postfix Increment: a++
        iterator operator++(int) noexcept {
            iterator temp = *this;
            ++ptr;
            return temp;
        }
        // Prefix Decrement: --a
        iterator& operator--() noexcept {
            --ptr;
            return *this;
        }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept {
            iterator temp = *this;
            --ptr;
            return temp;
        }

        iterator& operator+=(difference_type offset) noexcept {
            ptr += offset;
            return *this;
        }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept {
            return iterator(ptr + offset);
        }
        
        iterator& operator-=(difference_type offset) noexcept {
            ptr -= offset;
            return *this;
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept {
            return iterator(ptr - offset);
        }

        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { return this->ptr - rhs.ptr;}

        [[nodiscard]] reference operator[](difference_type offset) const noexcept { return *(ptr + offset); }

        
        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { return ptr == rhs.ptr; }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { return ptr != rhs.ptr; }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { return ptr < rhs.ptr; }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { return ptr > rhs.ptr; }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { return ptr <= rhs.ptr; }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { return ptr >= rhs.ptr; }
        
    };


    void clear() noexcept { _size = 0; }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept {
    return iterator + offset;
}

#endif

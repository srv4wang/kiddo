#pragma once
#include <atomic>
const int BUFF_SIZE = 1024; // buffer size is power of 2

/**
 *  ring buffer is useful when only one thread write
 */
template <typename T>
class RingBuffer {
public:
    RingBuffer();
    T& operator[](int index); 

    inline bool empty();
    inline bool full();

    int put(T item);
    int get();

private:
    std::atomic<int> _head;
    std::atomic<int> _tail;
    T _buffer[BUFF_SIZE]; 
};

template <typename T>
RingBuffer<T>::RingBuffer():_head(0),_tail(0) {
    
}

template <typename T>
T& RingBuffer<T>::operator[](int index) {
    return _buffer[index];
}

template <typename T>
bool RingBuffer<T>::empty() {
    return _head == _tail;
}

template <typename T>
bool RingBuffer<T>::full() {
    return ((_tail + 1) & (BUFF_SIZE - 1)) == _head;
}

template <typename T>
int RingBuffer<T>::put(T item) {
    if (full()) {
        return -1;
    }
    _buffer[_tail] = item;
    _tail = (_tail + 1) & (BUFF_SIZE - 1);
    return _tail;
}

template <typename T>
int RingBuffer<T>::get() {
    if (empty()) {
        return -1;
    }
    int index = _head;
    if (index == _head) {
        _head = (_head + 1) & (BUFF_SIZE - 1);
    } else {
        return -1;
    }
    return index;
}

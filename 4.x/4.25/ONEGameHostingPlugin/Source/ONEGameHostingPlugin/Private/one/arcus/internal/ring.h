// Copyright i3D.net, 2021. All Rights Reserved.
#pragma once

#include <assert.h>

#include <one/arcus/allocator.h>

namespace i3d {
namespace one {

// FIFO ring buffer with a fixed capacity.
template <typename T>
class Ring final {
public:
    Ring(size_t capacity)
        : _buffer(nullptr), _capacity(capacity), _size(0), _last(0), _next(0) {
        assert(_capacity > 0);
        void *p = allocator::create_array<T>(_capacity);
        assert(p);
        _buffer = reinterpret_cast<T *>(p);
    }
    ~Ring() {
        assert(_buffer);
        allocator::destroy_array<T>(_buffer);
        _buffer = nullptr;
    }

    Ring(const Ring &) = delete;
    Ring &operator=(const Ring &) = delete;

    void clear() {
        _next = _last = 0;
        _size = 0;
    }

    size_t capacity() const {
        return _capacity;
    }
    size_t size() const {
        return _size;
    }

    void push(const T &val) {
        _buffer[_next] = val;
        _next++;
        if (_next >= _capacity) _next = 0;
        if (_size < _capacity) _size++;
    }

    // Returns the last element, if any, or null if none.
    T *peek() {
        if (_size == 0) {
            return nullptr;
        }
        return &_buffer[_last];
    }

    // Pops the oldest pushed value. Asserts if size is zero.
    T &pop() {
        assert(_size > 0);
        // Cache the current last before incrementing so that the new
        // last can be error checked against the front of the ring, before
        // referencing the current last in the buffer.
        const auto prev_last = _last;
        _last++;
        if (_last >= _capacity) _last = 0;
        if (_size > 0) _size--;
        return _buffer[prev_last];
    }

private:
    T *_buffer;

    const size_t _capacity;
    size_t _size;

    unsigned int _last;  // The oldest pushed item that is not yet popped.
    unsigned int _next;  // The most recent pushed item.
};

}  // namespace one
}  // namespace i3d

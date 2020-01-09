#pragma once
#include <KPFoundation.hpp>
#include <functional>
#include <array>
#include <iterator>

template <typename T, size_t N>
class KPArray {
private:
    std::array<T, N> buffer;
    size_t _size = 0;

public:
    KPArray<T, N>() {
    }

    size_t size() {
        return _size;
    }

	std::array<T, N> & getBuffer() {
		return buffer;
	}

    typename std::array<T, N>::iterator begin() {
        return buffer.begin();
    }

    typename std::array<T, N>::iterator begin() const {
        return buffer.cbegin();
    }

    typename std::array<T, N>::iterator cbegin() const {
        return buffer.cbegin();
    }

    typename std::array<T, N>::iterator crbegin() const {
        return buffer.crbegin();
    }

    typename std::array<T, N>::iterator end() {
        return buffer.begin() + _size;
    }

    typename std::array<T, N>::iterator end() const {
        return buffer.cbegin() + _size;
    }

    typename std::array<T, N>::iterator cend() const {
        return buffer.cbegin() + _size;
    }

    typename std::array<T, N>::iterator crend() const {
        return buffer.crbegin() + _size;
    }

	void removeWithCondition(std::function<bool(const T & e)> condition) {
		int distance = 0;
		for (size_t i = 0; i < _size; i++) {
			if (condition(buffer[i])) {
				distance++;
			} else {
				buffer[i - distance] = buffer[i];
			}
		}

		_size -= distance;
	}

    void append(const T & e) {
        if (_size == buffer.max_size()) {
            raise(Exception::OutOfRange.withMessage(KPLocalString<20>(_size).c_str()));
        }

        buffer[_size++] = e;
    }

    T & operator[](size_t index) {
        if (index >= _size) {
            raise(Exception::OutOfRange.withMessage(KPLocalString<20>(index).c_str()));
        }

        return buffer[index];
    }

	operator bool() const {
		return _size > 0;
	}
};
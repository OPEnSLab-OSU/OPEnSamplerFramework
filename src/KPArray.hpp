#pragma once
#include <KPFoundation.hpp>
#include <array>
#include <functional>
#include <iterator>

template <typename T, size_t N>
class KPArray {
private:
	std::array<T, N> buffer;
	size_t _size = 0;

public:
	KPArray<T, N>() {}

	size_t size() const {
		return _size;
	}

	const std::array<T, N> & getBuffer() const {
		return buffer;
	}

	std::array<T, N> & getBuffer() {
		return buffer;
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
			raise(Exception::OutOfRange.withMessage(KPStringBuilder<20>(_size).c_str()));
		}

		buffer[_size++] = e;
	}

	void resize(size_t size) {
		if (size > buffer.max_size()) {
			raise(Exception::OutOfRange.withMessage(KPStringBuilder<20>(_size).c_str()));
		}

		_size = size;
	}

	T & operator[](size_t index) {
		if (index >= _size) {
			raise(Exception::OutOfRange.withMessage(KPStringBuilder<20>(index).c_str()));
		}

		return buffer[index];
	}

	size_t capacity() const {
		return N;
	}

	operator bool() const {
		return _size > 0;
	}

	//
	// ─── ITERATOR COMPATABILIY ──────────────────────────────────────────────────────
	//

	typename std::array<T, N>::iterator begin() {
		return buffer.begin();
	}

	typename std::array<T, N>::const_iterator begin() const {
		return buffer.cbegin();
	}

	typename std::array<T, N>::const_iterator cbegin() const {
		return buffer.cbegin();
	}

	typename std::array<T, N>::const_iterator crbegin() const {
		return buffer.crbegin();
	}

	typename std::array<T, N>::iterator end() {
		return buffer.begin() + _size;
	}

	typename std::array<T, N>::const_iterator end() const {
		return buffer.cbegin() + _size;
	}

	typename std::array<T, N>::const_iterator cend() const {
		return buffer.cbegin() + _size;
	}

	typename std::array<T, N>::const_iterator crend() const {
		return buffer.crbegin() + _size;
	}
};
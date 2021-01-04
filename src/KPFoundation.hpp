#pragma once
#include <Arduino.h>
#undef min
#undef max

#include <algorithm>
#include <numeric>

#define TRACE "[Trace ", millis() / 1000, " ", __FILE__, ":", __LINE__, __PRETTY_FUNCTION__, "] "

extern "C" char * sbrk(int i);

class KPController;
class KPComponent {
public:
	const char * name;
	KPController * controller;

	KPComponent(const char * name, KPController * controller = nullptr)
		: name(name),
		  controller(controller) {}

	virtual bool enabled() {
		return true;
	}

	virtual void setup(){};
	virtual void update(){};
};

//
// ────────────────────────────────────────────────── I ──────────
//   :::::: P R I N T : :  :   :    :     :        :          :
// ────────────────────────────────────────────────────────────
//

// Overload unsupported types here...
inline size_t printTo(Print & printer, time_t val) {
	return printer.print((long) val);
}

template <typename T0, typename T1>
size_t printTo(Print & printer, std::pair<T0, T1> val) {
	using namespace std;
	return printer.print("(") + printTo(printer, val.first) + printer.print(",")
		   + printTo(printer, val.second) + printer.print(")");
}

//...

template <typename T>
size_t printTo(Print & printer, T && val) {
	return printer.print(std::forward<T>(val));
}

template <typename... Types>
size_t printTo(Print & printer, Types &&... values) {
	size_t sizes[sizeof...(Types)] = {printTo(printer, std::forward<Types>(values))...};
	return std::accumulate(sizes, sizes + sizeof...(Types), 0);
}

template <typename T>
Print & operator<<(Print & printer, T && val) {
	printTo(printer, std::forward<T>(val));
	return printer;
}

// ────────────────────────────────────────────────────────────────────────────────
// Output to Serial
// ────────────────────────────────────────────────────────────────────────────────
template <typename... Types>
size_t print(Types &&... values) {
	return printTo(Serial, std::forward<Types>(values)...);
}

inline size_t println() {
	return Serial.println();
}

template <typename... Types>
size_t println(Types... values) {
	return print(std::forward<Types>(values)...) + println();
}

[[noreturn]] inline void halt() {
	while (true) {}
}

template <typename... Types>
[[noreturn]] void halt(Types &&... values) {
	println(std::forward<Types>(values)...);
	halt();
}

class KPString {
private:
	const char * ptr = nullptr;

public:
	KPString() = default;
	KPString(const char * _ptr) : ptr(_ptr) {}
	bool operator==(const char * rhs) const {
		return strcmp(ptr, rhs) == 0;
	}

	bool operator!=(const char * rhs) const {
		return !this->operator==(rhs);
	}

	operator const char *() const {
		return ptr;
	}
};

template <size_t capacity>
class KPStringBuilder : public Print {
private:
	char buffer[capacity + 1]{0};
	size_t _size = 0;

public:
	template <typename... Types>
	KPStringBuilder(Types &&... args) {
		using expand_type = int[];
		expand_type{0, (::printTo(*this, std::forward<Types>(args)), 0)...};
	}

	size_t write(const uint8_t * p, size_t n) override {
		size_t bound = std::min(capacity - _size, n);
		memcpy(buffer + _size, p, bound);
		_size += bound;
		buffer[_size] = 0;
		return bound;
	}

	size_t write(uint8_t c) override {
		if (c == 0 || _size >= capacity) {
			return 0;
		}

		buffer[_size++] = static_cast<char>(c);
		buffer[_size]	= 0;
		return 1;
	}

	void clear() {
		_size	  = 0;
		buffer[0] = 0;
	}

	size_t size() const {
		return _size;
	}

	operator char *() {
		return buffer;
	}

	bool operator==(const char * rhs) const {
		return strncmp(buffer, rhs, size()) == 0;
	}

	bool operator!=(const char * rhs) const {
		return !(operator==(rhs));
	}
};

inline double secsToMillis(double seconds) {
	return seconds * 1000;
}

inline double millisToSecs(double ms) {
	return ms / 1000;
}

inline size_t free_ram() {
	char stack_dummy;
	return &stack_dummy - sbrk(0);
}

inline int strcmpi(const char * left, const char * right) {
	for (int i = 0;; i++) {
		int l = left[i], r = right[i];
		if (l == 0 && r == 0) {
			return 0;
		}

		if (toupper(l) != toupper(r)) {
			return l - r;
		}
	}
}

inline size_t printFreeRam() {
	return println("Free Memory: ", free_ram());
}
#pragma once
#include <Arduino.h>
#undef min
#undef max

#include <KPConfiguration.hpp>
#include <algorithm>
#include <numeric>

#define TRACE                                                                                      \
	"[Trace", millis() / 1000, " ", __FILE__, ":", __LINE__, " in ", __PRETTY_FUNCTION__, "]: "

enum class Verbosity : int { none, error, info, debug };

struct PrintConfig {
private:
	static Verbosity printVerbose;

public:
#ifdef DEBUG
	static const Verbosity defaultPrintVerbose = Verbosity::debug;
#else
	static const Verbosity defaultPrintVerbose = Verbosity::error;
#endif

	PrintConfig() = delete;
	static const char * printSeparator;

	static Verbosity getPrintVerbose() {
		return printVerbose;
	}

	static void setPrintVerbose(Verbosity level) {
		printVerbose = level;
	}

	static void setDefaultVerbose() {
		printVerbose = defaultPrintVerbose;
	}
};

#define secsToMillis(x) ((x) *1000)
#define millisToSecs(x) ((x) / 1000)

extern "C" char * sbrk(int i);
extern size_t free_ram();
extern size_t printFreeRam();
extern int strcmpi(const char *, const char *);

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
	if (PrintConfig::getPrintVerbose() > PrintConfig::defaultPrintVerbose) {
		return 0;
	}

	return printTo(Serial, std::forward<Types>(values)...);
}

inline size_t println() {
	if (PrintConfig::getPrintVerbose() > PrintConfig::defaultPrintVerbose) {
		return 0;
	}

	return Serial.println();
}

template <typename... Types>
size_t println(Types... values) {
	return print(std::forward<Types>(values)...) + println();
}

/** ────────────────────────────────────────────────────────────────────────────
 *  This function doesn't return
 *  ──────────────────────────────────────────────────────────────────────────── */
inline void halt() {
	while (true) {}
}

class KPString {
private:
	const char * ptr = nullptr;

public:
	KPString()				   = default;
	KPString(const KPString &) = default;
	KPString(const char * _ptr) : ptr(_ptr) {}

	KPString & operator=(const char * rhs) {
		ptr = rhs;
		return *this;
	}

	bool operator==(const char * rhs) const {
		return strcmp(ptr, rhs) == 0;
	}

	bool operator!=(const char * rhs) const {
		return !(*this == rhs);
	}

	operator const char *() const {
		return ptr;
	}
};

template <size_t capacity>
class KPStringBuilder : public Print, public Printable {
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
		for (size_t i = 0; i < bound; i++) {
			uint8_t c = p[i];
			if (c == 0) {
				return i;
			} else {
				write(c);
			}
		}

		return bound;
	}

	size_t write(uint8_t c) override {
		if (c == 0 || _size >= capacity) {
			return 0;
		}

		buffer[_size++] = static_cast<char>(c);
		return 1;
	}

	void clear() {
		_size = 0;
	}

	size_t size() const {
		return _size;
	}

	const char * c_str() const {
		return buffer;
	}

	operator const char *() const {
		return buffer;
	}

	operator String() const {
		return String(c_str());
	}

	size_t printTo(Print & printer) const override {
		return printer.print(c_str());
	}
};
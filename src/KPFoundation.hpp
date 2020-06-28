#pragma once
#include <Arduino.h>
#undef min
#undef max

#include <KPConfiguration.hpp>
#include <algorithm>

enum class Verbosity : int {
	none,
	error,
	info,
	debug
};

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

#define secsToMillis(x) ((x)*1000)
#define millisToSecs(x) ((x) / 1000)

class Error;
extern "C" char * sbrk(int i);
extern int free_ram();
extern void printFreeRam();
extern void raise(Error e, long ms = 3000);
extern void raise(const char *, long ms = 3000);
extern int strcmpi(const char *, const char *);

class KPController;

class KPComponent {
public:
	const char * name;
	KPController * controller;

	KPComponent(const char * name, KPController * controller = nullptr)
		: name(name), controller(controller) {}

	virtual bool enabled() {
		return true;
	}

	virtual void setup(){};
	virtual void update(){};
};

//=============================================================================
// Print
//=============================================================================
template <typename T>
void print(const T & value) {
	if (PrintConfig::getPrintVerbose() <= PrintConfig::defaultPrintVerbose) {
		Serial.print(value);
	}
}

template <>
inline void print(const time_t & value) {
	Serial.print((long)value);
}

template <typename T, typename... Types>
void print(const T & first, const Types &... args) {
	print(first);
	Serial.print(PrintConfig::printSeparator);
	print(args...);
}

inline void println() {
	Serial.println();
}

template <typename T>
inline void println(const T & value) {
	print(value);
	Serial.println();
}

template <>
inline void println(const time_t & value) {
	Serial.println((long)value);
}

template <typename T, typename... Types>
void println(const T & first, const Types &... args) {
	print(first);
	Serial.print(PrintConfig::printSeparator);
	println(args...);
}

class KPString {
private:
	const char * ptr = nullptr;

public:
	KPString()				   = default;
	KPString(const KPString &) = default;
	KPString(const char * _ptr)
		: ptr(_ptr) {}

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

class KPClearableString : public String {
public:
	using String::String;

	static KPClearableString & sharedInstance() {
		static KPClearableString * buffptr = nullptr;
		if (!buffptr) {
			static KPClearableString buff(0);
			buff.reserve(KP_STATIC_STRING_BUFFER_SIZE);
			buffptr = &buff;
		}
		return *buffptr;
	}

#ifdef ARDUINO_ARCH_SAMD
	void clear() {
		len = 0;
	}
#endif
};

template <size_t capacity>
class KPStringBuilder : public Printable {
private:
	char buffer[capacity + 1]{0};
	size_t size = 0;

	static inline KPClearableString & sharedBuffer() {
		return KPClearableString::sharedInstance();
	}

	static void clearSharedBuffer() {
		sharedBuffer().clear();
	}

public:
	size_t length() const {
		return size;
	}

	const char * c_str() const {
		return buffer;
	}

	KPStringBuilder() {}

	// Construct from another KPStringBuilder
	template <size_t N>
	KPStringBuilder(const KPStringBuilder<N> & other) {
		// println("Construct from another KPStringBuilder<", N, ">");
		concat(other);
	};

	// Construct from other types
	template <typename T>
	explicit KPStringBuilder(const T & first) {
		concat(first);
	}

	// Variadic template for construction
	template <typename T, typename... Types>
	explicit KPStringBuilder(T first, Types... args) {
		concat(first, args...);
	}

	// Copy assignment to another KPStringBuilder
	template <size_t N>
	KPStringBuilder<capacity> & operator=(const KPStringBuilder<N> & other) {
		// println("Copy assignment to another KPStringBuilder<", N, ">");
		size = 0;
		concat(other);
		return *this;
	};

	// Copy assignment to const char *
	KPStringBuilder & operator=(const char * cstr) {
		// println("Copy assignment to const char *");
		for (size = 0; size < capacity; size++) {
			if (!cstr[size]) break;
			buffer[size] = cstr[size];
		}
		return *this;
	};

	unsigned char compare(const char * cstr) const {
		return strncmp(buffer, cstr, capacity + 1);
	}

	bool operator==(const char * cstr) const {
		return compare(cstr) == 0;
	}

	// Concatenate another KPStringBuilder
	template <size_t N>
	KPStringBuilder & concat(const KPStringBuilder<N> & other) {
		// println("Concat from another KPStringBuilder<", N, "> which has
		// size=", other.length());
		const size_t new_size = std::min(capacity, size + other.length());
		strncpy(buffer + size, other.c_str(), new_size - size);
		size		 = new_size;
		buffer[size] = 0;
		return *this;
	}

	// Concatenate other types.
	// Use the shared String buffer to convert between types then copy the
	// content over
	template <typename T>
	KPStringBuilder & concat(const T & first) {
		// println("Concat from another type");
		clearSharedBuffer();
		sharedBuffer().concat(first);
		strncpy(buffer + size, sharedBuffer().c_str(), capacity - size);
		size		 = std::min(capacity, size + sharedBuffer().length());
		buffer[size] = 0;
		return *this;
	}

	// Variadic template for concatenate
	template <typename T, typename... Types>
	KPStringBuilder & concat(const T & first, const Types &... args) {
		concat(first);
		return concat(args...);
	}

	size_t printTo(Print & printer) const override {
		return printer.println(buffer);
	}

	operator const char *() const {
		return buffer;
	}
};

class Error : public Printable {
public:
	const char * name	 = nullptr;
	const char * message = nullptr;

	Error(const char * name, const char * message)
		: name(name), message(message) {}

	explicit Error(const char * message = nullptr)
		: Error(nullptr, message) {}

	Error(const Error & rhs, const char * message)
		: Error(rhs.name, message) {}

	Error(const Error & rhs)
		: Error(rhs.name, rhs.message) {}

	Error withMessage(const char * message) const {
		return Error(this->name, message);
	}

	size_t printTo(Print & printer) const {
		return printer.print(name ? name : "Error")
			   + printer.print(": ")
			   + printer.print(message);
	}

	bool operator==(const Error & rhs) {
		return strcmp(name, rhs.name);
	}

	bool operator!=(const Error & rhs) {
		return !(*this == rhs);
	}
};

namespace Exception {
	static const Error OutOfRange	   = Error("OutOfRangeError", "");
	static const Error FileNotFound	   = Error("FileNotFoundError", "");
	static const Error InvalidArgument = Error("InvalidArgumentError", "");
}  // namespace Exception

#define var_name(x) Internal::_var_name(#x, x)
namespace Internal {
	template <typename T>
	const char * _var_name(const char * name, const T & var) {
		return name;
	}
};	// namespace Internal

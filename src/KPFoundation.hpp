#pragma once
#include <Arduino.h>
#undef min
#undef max

#include <KPConfig.hpp>
#include <algorithm>

#define secsToMillis(x) (x * 1000)
#define millisToSecs(x) (x / 1000)

class Error;
extern "C" char * sbrk(int i);
extern int free_ram();
extern void printFreeRam();
extern void raise(Error e, long ms = 2000);
extern int strcmpi(const char *, const char *);

class KPController;
class KPComponent {
public:
    const char * name;
    KPController * controller;

    KPComponent(const char * name, KPController * controller = nullptr)
        : name(name), controller(controller) {
    }

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
    if (Config::printVerbose <= Config::defaultPrintVerbose) {
        Serial.print(value);
    }
}

template <typename T, typename... Types>
void print(const T & first, const Types &... args) {
    print(first);
    Serial.print(Config::printSeparator);
    print(args...);
}

template <typename T = void>
void println() {
    Serial.println();
}

template <typename T>
void println(const T & value) {
    print(value);
    Serial.println();
}

template <typename T, typename... Types>
void println(const T & first, const Types &... args) {
    print(first);
    Serial.print(Config::printSeparator);
    println(args...);
}

class KPReusableString : public String {
public:
    using String::String;
#ifdef ARDUINO_ARCH_SAMD
    void clear() {
        len = 0;
    }
#endif
};

template <size_t capacity = 80>
class KPLocalString : public Printable {
public:
    size_t size = 0;
    char c_str_buffer[capacity + 1]{0};

public:
    static KPReusableString & buffer() {
        static KPReusableString * buffptr = nullptr;
        if (!buffptr) {
            static KPReusableString buff(0);
            buff.reserve(capacity);
            buffptr = &buff;
        }
        return *buffptr;
    }

    static void clearBuffer() {
        KPLocalString::buffer().clear();
    }

    KPLocalString() {}

    KPLocalString(const KPLocalString & rhs) {
        strncpy(c_str_buffer, rhs.c_str_buffer, std::min(capacity, rhs.size));
        size = strlen(c_str_buffer);
    }

    template <typename T>
    KPLocalString(T first) {
        concat(first);
    }

    template <typename T, typename... Types>
    KPLocalString(T first, Types... args) {
        concat(first, args...);
    }

    template <size_t N>
    const char * concat(const KPLocalString<N> & s) {
        return this->concat(s.c_str());
    }

    template <typename T>
    const char * concat(const T & first) {
        clearBuffer();
        buffer().concat(first);
        transfer();
        return c_str_buffer;
    }

    template <typename T, typename... Types>
    const char * concat(const T & first, const Types &... args) {
        concat(first);
        return concat(args...);
    }

    void transfer() {
        int length = std::min(buffer().length(), capacity - size);
        strncpy(c_str_buffer + size, buffer().c_str(), length);
		size += length;
        c_str_buffer[size] = 0;
    }

    const char * c_str() const {
        return c_str_buffer;
    }

    size_t printTo(Print & p) const {
        return p.print(c_str_buffer);
    }

    bool operator==(const char * rhs) const {
        println(c_str_buffer);
        return strcmp(c_str_buffer, rhs) == 0;
    }

    bool operator==(const KPLocalString & rhs) const {
        return strcmp(c_str_buffer, rhs.c_str_buffer) == 0;
    }
};

class Error {
public:
    const char * name;
    const char * message;

    Error(const char * name, const char * message): name(name), message(message){
    }

    Error(const char * name, const KPLocalString<> & message)
        : Error(name, message.c_str()) {}

    explicit Error(const char * message = nullptr)
        : Error(nullptr, message) {}

    Error(const Error & rhs, const char * message)
        : Error(rhs.name, message) {}

    Error(const Error & rhs)
        : Error(rhs.name, rhs.message) {}

    Error withMessage(const char * message) const {
        return Error(this->name, message);
    }

    bool operator==(const Error & rhs) {
        return name == rhs.name;
    }

    bool operator!=(const Error & rhs) {
        return !(*this == rhs);
    }
};

namespace Exception {
    static const Error OutOfRange      = Error("OutOfRangeError", "");
    static const Error FileNotFound    = Error("FileNotFoundError", "");
    static const Error InvalidArgument = Error("InvalidArgumentError", "");
}  // namespace Exception

#define var_name(x) Internal::_var_name(#x, x)
namespace Internal {
    template <typename T>
    const char * _var_name(const char * name, const T & var) {
        return name;
    }
};  // namespace Internal

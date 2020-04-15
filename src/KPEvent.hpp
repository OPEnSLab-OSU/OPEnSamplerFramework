#pragma once
#include <KPFoundation.hpp>
#include <Vector>
#include <functional>
#include <unordered_map>

// NOTE WORK IN PROGRESS

template <typename T>
class KPEvent {
public:
	typedef T type;
	std::vector<std::function<void(T *)>> listeners;
	const char * name = nullptr;

	template <typename K>
	void dispatch(T * arg = nullptr) {
		for (auto listener : listeners) {
			listener((K *)arg);
		}
	}
};

class KPEventPublisher {
public:
	std::unordered_map<const char *, KPEvent<void>> events;
	KPEventPublisher() {
		auto value = events["asdf"];
		if (std::is_same<decltype(value)::type, void>::value) {
		}
	}

	template <typename T>
	void addEventListener(const char * name, std::function<void(T *)> callback) {
	}
};

// template <typename T>
// class KPEventListener {
// public:
// 	typedef std::function<void(T *)> Callback;
// 	Callback _callback;
// 	KPEventListener(Callback callback)
// 		: _callback{callback} {
// 	}
// };

// class KPEventPublisher {
// public:
// 	typedef std::function<void(void *)> Listener;
// 	typedef std::vector<Listener> listeners;
// 	std::unordered_map<const char *, listeners> eventTable;

// public:
// 	template <typename T>
// 	void addEventListener(const char * event, std::function<void(T * arg)> listener) {
// 		eventTable[event].push_back(static_cast<std::function<void(void *)>>(listener));
// 	}

// protected:
// 	void notify(const char * event, void * args) {
// 		for (auto listener : eventTable[event]) {
// 			listener(args);
// 		}
// 	}
// };
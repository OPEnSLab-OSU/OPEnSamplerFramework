
// References:
// https://embeddedartistry.com/blog/2017/02/01/improving-your-callback-game/
// https://en.cppreference.com/w/cpp/language/parameter_pack
#pragma once
#include <KPFoundation.hpp>
#include <functional>
#include <unordered_map>

template <typename T>
class KPSubject {
protected:
	using ObserverType = T;
	std::unordered_map<int, ObserverType *> observers;

public:
	int addObserver(ObserverType * o) {
		auto token = rand();
		while (!observers.insert({token, o}).second) {
			token = rand();
		}

		return token;
	}

	auto addObserver(ObserverType & o) -> decltype(addObserver(&o)) {
		return addObserver(&o);
	}

	void removeObserver(long token) {
		observers.erase(token);
	}

	template <typename F, typename... Types>
	void updateObservers(F method, Types &&... args) {
		for (auto & k : observers) {
			println("Updating ", k.second->ObserverName());
			(k.second->*method)(std::forward<Types>(args)...);
		}
	}
};
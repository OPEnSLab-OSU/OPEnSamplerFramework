
// References:
// https://embeddedartistry.com/blog/2017/02/01/improving-your-callback-game/
// https://en.cppreference.com/w/cpp/language/parameter_pack
#pragma once
#include <KPFoundation.hpp>
#include <functional>
#include <unordered_map>

/**
 * This class is represents the "subject" of the observer pattern.
 *
 * @tparam T Observer Type
 */
template <typename T>
class KPSubject {
protected:
	using ObserverType = T;
	std::unordered_map<int, ObserverType *> observers;

public:
	/**
	 * Add the observer to the internal data structure
	 *
	 * @param o Pointer to the observer instance
	 * @return int Randomly generated token (use to remove the observer later)
	 */
	int addObserver(ObserverType * o) {
		auto token = rand();
		while (!observers.insert({token, o}).second) {
			token = rand();
		}

		return token;
	}

	/**
	 * Convient method for adding reference to the observer instance
	 *
	 * @param o see addObserver(ObserverType * o)
	 * @return see addObserver(ObserverType * o)
	 */
	auto addObserver(ObserverType & o) -> decltype(addObserver(&o)) {
		return addObserver(&o);
	}

	/**
	 * Remove observer given a token
	 *
	 * @param token Token generated when calling addObserver methods
	 */
	void removeObserver(int token) {
		observers.erase(token);
	}

	/**
	 * Pass variable number of arguments to observers' class member function
	 *
	 * @tparam F Auto deduced type of observer's member function
	 * @tparam Types Auto deduced types of arguments
	 * @param method Pointer to member function of the observer
	 * @param args Arguments to pass through
	 */
	template <typename F, typename... Types>
	void updateObservers(F method, Types &&... args) {
		for (auto & k : observers) {
			println("Updating ", k.second->ObserverName());
			(k.second->*method)(std::forward<Types>(args)...);
		}
	}
};
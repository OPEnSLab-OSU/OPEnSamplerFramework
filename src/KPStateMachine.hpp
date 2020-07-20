#pragma once
#include <KPSubject.hpp>
#include <KPStateMachineObserver.hpp>
#include <unordered_map>

class KPState;
class KPStateMachine : public KPComponent, public KPSubject<KPStateMachineObserver> {
private:
	using Middleware = std::function<void(int)>;
	std::unordered_map<const char *, KPState *> mapNameToState;
	std::unordered_map<const char *, Middleware> mapNameToMiddleware;
	KPState * currentState = nullptr;

public:
	using KPComponent::KPComponent;

	template <typename T>
	void registerState(T && state, const char * name, Middleware middleware = nullptr) {
		if (mapNameToState.count(name)) {
			halt(TRACE, name, " already exist");
		}

		if (name == nullptr) {
			halt(TRACE, "State must have a name");
		}

		T * copy			 = new T{std::forward<T>(state)};
		copy->name			 = name;
		mapNameToState[name] = copy;
		if (middleware) {
			mapNameToMiddleware[name] = middleware;
		} else {
			mapNameToMiddleware[name] = [name](int code) {
				halt(TRACE, "Unhandled state transition: ", name);
			};
		}
	}

	template <typename T>
	void registerState(T && state, const char * name, const char * next_name) {
		registerState(std::forward<T>(state), name,
					  [this, next_name](int code) { transitionTo(next_name); });
	}

	template <typename T>
	T * getState(const char * name) {
		auto c = mapNameToState[name];
		if (c) {
			return static_cast<T *>(c);
		} else {
			return nullptr;
		}
	}

	KPState * getCurrentState() const {
		return currentState;
	}

	void next(int code = 0);
	void restart();
	void transitionTo(const char * name);

protected:
	void setup() override;
	void update() override;
};
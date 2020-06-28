#pragma once
#include <KPFoundation.hpp>
#include <KPSubject.hpp>
#include <KPStateMachineObserver.hpp>
#include <unordered_map>
#include <vector>

class KPState;
class KPStateMachine : public KPComponent,
					   public KPSubject<KPStateMachineObserver> {
private:
	std::unordered_map<const char *, KPState *> statesByName;
	std::unordered_map<KPState *, int> statesIndexMap;
	KPState * currentState = nullptr;

public:
	using KPComponent::KPComponent;

	static KPStateMachine & sharedInstance() {
		static KPStateMachine machine("default-state-machine");
		return machine;
	}

	template <typename T>
	void registerState(const T & state, const char * name, int reserve_size = 0) {
		if (statesByName.count(name)) {
			println("State machine: ", name, " already exist");
			return;
		}

		T * copy			 = new T(state);
		copy->name			 = name;
		statesByName[name]	 = copy;
		statesIndexMap[copy] = statesIndexMap.size();
		if (reserve_size) {
			copy->reserve(reserve_size);
		}
	}

	template <typename T>
	void registerState(const T & state) {
		if (state.name == nullptr) {
			raise(Exception::InvalidArgument.withMessage("State must have a name"));
		}

		registerState(state, state.name);
	}

	template <typename T>
	T * getState(const char * name) {
		auto c = statesByName[name];
		if (c) {
			return static_cast<T *>(c);
		} else {
			return nullptr;
		}
	}

	KPState * getCurrentState() const {
		return currentState;
	}

	int getCurrentStateIndex() const {
		return currentState ? statesIndexMap.at(currentState) : -1;
	}

	void transitionTo(const char * name);
	void setup() override;
	void update() override;
};
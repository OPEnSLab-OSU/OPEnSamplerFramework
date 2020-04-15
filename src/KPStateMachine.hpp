#pragma once

#include <KPFoundation.hpp>
#include <unordered_map>
#include <vector>

class KPStateMachine;
class KPStateMachineListener {
public:
	virtual void stateTransitioned(const KPStateMachine & sm) = 0;
};

class KPState;
class KPStateMachine : public KPComponent {
private:
	std::unordered_map<const char *, KPState *> statesByName;
	std::unordered_map<KPState *, int> statesIndexMap;
	KPState * currentState = nullptr;

	std::vector<KPStateMachineListener *> listeners;

public:
	using KPComponent::KPComponent;

	static KPStateMachine & instance() {
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

	void updateListeners() {
		for (auto listener : listeners) {
			listener->stateTransitioned(*this);
		}
	}

	void addListener(KPStateMachineListener * listener) {
		listeners.push_back(listener);
	}

	void transitionTo(const char * name);
	void setup() override;
	void update() override;
};
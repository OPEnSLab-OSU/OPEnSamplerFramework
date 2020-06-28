#pragma once
#include <KPObserver.hpp>

class KPState;
class KPStateMachineObserver : public KPObserver {
public:
	static const char * ObserverName() {
		return "<Unnamed> State Machine Observer";
	}

	virtual void stateDidBegin(const KPState * newState) = 0;
};
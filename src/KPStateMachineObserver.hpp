#pragma once
#include <KPObserver.hpp>

class KPState;

/**
 * By inheriting this class, the subclass can listen to several notifications
 * called from KPStateMachine
 * 
 */
class KPStateMachineObserver : public KPObserver {
public:
	const char * ObserverName() const {
		return KPStateMachineObserverName();
	}

	virtual const char * KPStateMachineObserverName() const {
		return "<Unnamed> State Machine Observer";
	}

	virtual void stateDidBegin(const KPState * newState) = 0;
};
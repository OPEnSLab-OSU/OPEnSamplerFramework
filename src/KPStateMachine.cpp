#include <KPState.hpp>
#include <KPStateMachine.hpp>

void KPStateMachine::setup() {
	// do nothing
}

void KPStateMachine::update() {
	if (!currentState) {
		return;
	}

	for (KPStateSchedule & s: currentState->schedules) {
		if (!s.activated && currentState->timeSinceLastTransition() >= s.time) {
			s.activated = true;
			s.callback();
		}
	}
	
    currentState->update(*this);
}

void KPStateMachine::transitionTo(const char * name) {
    // Leave the current state
    if (currentState) {
        currentState->leave(*this);
		currentState->stop();
    }

    // Move to new state
    auto next = statesByName[name];
    if (next) {
        currentState = next;
		currentState->begin(); 
		println("Transitioned to ", next->getName());
        currentState->enter(*this);
    }
}
#include <KPState.hpp>
#include <KPStateMachine.hpp>

void KPStateMachine::setup() {
	// do nothing
}

void KPStateMachine::update() {
	if (!currentState) {
		return;
	}

	for (KPStateSchedule & s : currentState->schedules) {
		if (!s.activated && s.condition) {
			if (!s.condition()) {
				continue;
			}

			s.activated = true;
			s.callback();
		} else if (!s.activated && currentState->timeSinceLastTransition() >= s.time) {
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
	}

	// Move to new state
	auto next = statesByName[name];
	if (next) {
		// println("Transitioned to ", next->getName());
		currentState = next;
		updateListeners();
		currentState->begin();
		currentState->enter(*this);
	}
}
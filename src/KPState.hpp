#pragma once
#include <KPFoundation.hpp>
#include <KPStateMachine.hpp>
#include <functional>
#include <vector>

struct KPStateSchedule {
private:
	bool activated = false;
	friend class KPStateMachine;

public:
	unsigned long time = 0;
	std::function<bool()> condition;
	std::function<void()> callback;

	KPStateSchedule(long time, std::function<void()> callback)
		: time(time), callback(callback) {
	}

	KPStateSchedule(std::function<bool()> condition, std::function<void()> callback)
		: condition(condition), callback(callback) {
	}
};

class KPState {
	friend class KPStateMachine;

public:
	const char * name		= nullptr;
	unsigned long startTime = 0;
	size_t currentSchedule	= 0;
	std::vector<KPStateSchedule> schedules;

	void begin() {
		startTime		= millis();
		currentSchedule = 0;
	}

public:
	void reserve(size_t size) {
		schedules.reserve(size);
	}

	const char * getName() const {
		return name;
	}

	// enter and leave methods are required by subclasses
	virtual void enter(KPStateMachine & sm) = 0;

	// (optional) leave is called when leaving the state
	virtual void leave(KPStateMachine & sm) {
	}

	// (optional) update provides runtime update while the state is true
	virtual void update(KPStateMachine & sm) {
	}

	//
	// virtual void setValuesFromJson(const JsonVariant & data) {
	// }

	unsigned long timeSinceLastTransition() const {
		return millis() - startTime;
	}

	void setTimeCondition(unsigned long time, std::function<void()> callback) {
		PrintConfig::setPrintVerbose(Verbosity::debug);
		if (currentSchedule == schedules.size()) {
			schedules.push_back(KPStateSchedule(time, callback));
			println("Adding new schedule");
		} else {
			schedules[currentSchedule] = KPStateSchedule(time, callback);
			println("Updating schedule");
		}

		println("Time arugment:", time);
		println("Schedule Time:", schedules[currentSchedule].time);

		currentSchedule++;
		PrintConfig::setDefaultVerbose();
	}

	void setCondition(std::function<bool()> condition, std::function<void()> callback) {
		if (currentSchedule == schedules.size()) {
			schedules.push_back(KPStateSchedule(condition, callback));
			// println("Adding new schedule");
		} else {
			schedules[currentSchedule] = KPStateSchedule(condition, callback);
			// println("Updating schedule");
		}

		currentSchedule++;
	}
};
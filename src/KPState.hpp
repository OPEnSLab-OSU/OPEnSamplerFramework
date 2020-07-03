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

protected:
	const char * name		= nullptr;
	unsigned long startTime = 0;
	size_t currentSchedule	= 0;
	std::vector<KPStateSchedule> schedules;

	void begin() {
		startTime		= millis();
		currentSchedule = 0;
	}

	void reserve(size_t size) {
		schedules.reserve(size);
	}

public:
	const char * getName() const {
		return name;
	}

	/** ────────────────────────────────────────────────────────────────────────────
	 *  @brief REQUIRED Subclass must override this method and specify the behavior 
	 * when entering this state
	 *
	 *  @param machine
	 *  ──────────────────────────────────────────────────────────────────────────── */
	virtual void enter(KPStateMachine & machine) = 0;

	/** ────────────────────────────────────────────────────────────────────────────
	 *  @brief When override, this method is called just before the transition to new
	 *  state
	 *
	 *  @param machine State machine owning this state
	 *  ──────────────────────────────────────────────────────────────────────────── */
	virtual void leave(KPStateMachine & machine) {}

	/** ────────────────────────────────────────────────────────────────────────────
	 *  @brief Override to receive runtime lifecycle loop while in this state
	 *
	 *  @param machine State machine owning this state
	 *  ──────────────────────────────────────────────────────────────────────────── */
	virtual void update(KPStateMachine & machine) {}

	// TODO: Maybe a good idea
	// virtual void setValuesFromJson(const JsonVariant & data) {}

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
		PrintConfig::setPrintVerbose(Verbosity::debug);
		if (currentSchedule == schedules.size()) {
			schedules.push_back(KPStateSchedule(condition, callback));
			println("Adding new schedule");
		} else {
			schedules[currentSchedule] = KPStateSchedule(condition, callback);
			println("Updating schedule");
		}

		currentSchedule++;
		PrintConfig::setDefaultVerbose();
	}
};
#pragma once
#include <KPFoundation.hpp>
#include <functional>
#include <vector>

struct KPStateSchedule {
private:
	bool activated = false;
	friend class KPStateMachine;

public:
	std::function<bool()> condition;
	std::function<void()> callback;
	KPStateSchedule(std::function<bool()> condition, std::function<void()> callback)
		: condition(condition),
		  callback(callback) {}
};

class KPStateMachine;
class KPState {
	friend class KPStateMachine;

protected:
	const char * name		 = nullptr;
	unsigned long startTime	 = 0;
	bool didEnter			 = false;
	size_t numberOfSchedules = 0;
	std::vector<KPStateSchedule> schedules;

	void begin() {
		startTime		  = millis();
		numberOfSchedules = 0;
		didEnter		  = false;
	}

	void reserve(size_t size) {
		schedules.reserve(size);
	}

public:
	/**
	 * Get the Name object
	 *
	 * @return const char* pointer to the name assigned to this state
	 */
	const char * getName() const {
		return name;
	}

	/**
	 * [Required] Subclass must override this method and specify the behavior
	 * when entering this state
	 *
	 * @param machine
	 */
	virtual void enter(KPStateMachine & machine) = 0;

	/**
	 * When override, this method is called just before the transition to new
	 * state
	 *
	 * @param machine State machine owning this state
	 */
	virtual void leave(KPStateMachine & machine) {}

	/**
	 * Override to receive runtime lifecycle loop while in this state
	 *
	 * @param machine State machine owning this state
	 */
	virtual void update(KPStateMachine & machine) {}

	// TODO: Maybe a good idea
	// virtual void setValuesFromJson(const JsonVariant & data) {}

	/**
	 * Return time since the last call to enter lifecycle method
	 *
	 * @return unsigned long
	 */
	unsigned long timeSinceLastTransition() const {
		return millis() - startTime;
	}

	/**
	 * Convenient method for time-based state condition
	 *
	 * @param seconds time until the callback is executed
	 * @param callback callback to execute when the time expires
	 */
	void setTimeCondition(unsigned long seconds, std::function<void()> callback) {
		auto millis = secsToMillis(seconds);
		setCondition([this, millis]() { return timeSinceLastTransition() >= millis; }, callback);
	}

	/**
	 * Set the state condition.
	 *
	 * @param condition callable that returns true on abitary condition
	 * @param callback callable to be executed when condition returns true
	 */
	void setCondition(std::function<bool()> condition, std::function<void()> callback) {
		if (numberOfSchedules == schedules.size()) {
			schedules.push_back(KPStateSchedule(condition, callback));
		} else {
			schedules[numberOfSchedules] = KPStateSchedule(condition, callback);
		}

		numberOfSchedules++;
	}
};
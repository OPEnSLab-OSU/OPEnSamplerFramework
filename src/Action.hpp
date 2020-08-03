#pragma once
#include <KPFoundation.hpp>
#include <functional>
#include <vector>

class TimedAction {
protected:
	friend class ActionScheduler;
	bool removable = false;
	long start	   = 0;

	void begin() {
		removable = false;
		start	  = millis();
	}

public:
	long interval		  = 0;
	signed char repeatFor = 0;
	const char * name;

	std::function<void()> callback;
	TimedAction() = default;
	TimedAction(const char * name) : name(name) {}

	long timeElapsed() const {
		return millis() - start;
	}

	bool isReady() const {
		return timeElapsed() >= interval;
	}
};

class ActionScheduler : public KPComponent {
public:
	std::vector<TimedAction> actions;

	ActionScheduler(const char * name) : KPComponent(name) {}

	template <typename T>
	void add(T && action) {
		actions.push_back(std::move(action));
		actions.back().begin();
	}

	void markForRemoval(const char * name) {
		for (auto & action : actions) {
			if (strcmp(action.name, name) == 0) {
				action.removable = true;
			}
		}
	}

	void removeCompletedActions() {
		auto predicate = std::remove_if(actions.begin(), actions.end(),
										[](const TimedAction & a) { return a.removable; });

		actions.erase(predicate, actions.end());
	}

	void update() override {
		removeCompletedActions();

		for (auto & action : actions) {
			if (action.isReady()) {
				action.callback();
			} else {
				continue;
			}

			if (action.repeatFor == 0) {
				action.removable = true;
			}

			if (!action.removable) {
				action.begin();
				action.repeatFor = std::max(action.repeatFor - 1, -1);
			}
		}
	}

	static ActionScheduler & sharedInstance() {
		static ActionScheduler shared("Shared ActionScheduler");
		return shared;
	}
};

template <typename T>
inline void run(T && action) {
	ActionScheduler::sharedInstance().add(std::forward<T>(action));
}

inline void run(long delay, std::function<void()> callback) {
	TimedAction action;
	action.interval = delay;
	action.callback = callback;
	ActionScheduler::sharedInstance().add(action);
}

inline void runForever(long delay, const char * name, std::function<void()> callback) {
	TimedAction action;
	action.name		 = name;
	action.interval	 = delay;
	action.callback	 = callback;
	action.repeatFor = -1;
	ActionScheduler::sharedInstance().add(action);
}

inline void runForever(TimedAction action) {
	action.repeatFor = -1;
	ActionScheduler::sharedInstance().add(std::move(action));
}

inline void cancel(const char * name) {
	ActionScheduler::sharedInstance().markForRemoval(name);
}

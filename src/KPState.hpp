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
    long time = 0;
    std::function<void()> callback;

    KPStateSchedule(long time, std::function<void()> callback)
        : time(time), callback(callback) {}
};

class KPState {
    friend class KPStateMachine;

private:
    const char * name      = nullptr;
    long startTime         = 0;
    size_t currentSchedule = 0;
    std::vector<KPStateSchedule> schedules;

    void begin() {
        startTime = millis();
    }

    void stop() {
        currentSchedule = 0;
    }

public:
    const char * getName() const {
        return name;
    }

    // enter and leave methods are required by subclasses
    virtual void enter(KPStateMachine & sm) = 0;

    // update is optional
    virtual void leave(KPStateMachine & sm) {
    }

    virtual void update(KPStateMachine & sm) {
    }

    long timeSinceLastTransition() const {
        return millis() - startTime;
    }

    void wait(long time, std::function<void()> callback) {
        long prevTime = currentSchedule ? schedules[currentSchedule - 1].time : 0;
        if (currentSchedule == schedules.size()) {
            schedules.push_back(KPStateSchedule(prevTime + time, callback));
            println("Adding new schedule");
        } else {
            schedules[currentSchedule] = KPStateSchedule(prevTime + time, callback);
            println("Updating schedule");
        }

        currentSchedule++;
    }
};
#pragma once
#include "OPTask.hpp"
/*
    Class for running multiple asyncronous tasks in parallel
*/
class OPTaskSequence : public LinkedList<OPTask> {
    public: 

    OPTaskSequence() : LinkedList() {
        
    }

    OPTaskSequence & repeat(int times) {
        if (tail == nullptr) {
            Serial.println("Repeating nullptr");
            while(true);
        }

        OPTask & task = tail->data;
        task.repeat = times;
        return * this;
    }

    OPTaskSequence & then(OPTask const & other) {
        OPTask copy(other);
        append(copy);
        return * this;
    }
    
    void loop() {
        if (isEmpty()) {
            return;
        }

        OPTask & task = head->data;
        if (task.activated) {
            task.loop();
        } else {
            task.run();
        }

        if (task.isEmpty()) {
            remove(0);
        }
    }
};
#pragma once
#include "OPTaskSequence.hpp"
#include "OPComponent.hpp"

class OPTaskScheduler : public LinkedList<OPTaskSequence>, public OPComponent {
public:
    OPTaskScheduler(String name = "Scheduler") 
    : 
    LinkedList(), OPComponent(name) {
        
    }
    
    void loop() override {
        auto current = head;
        while (current) {
            OPTaskSequence & seq = head->data;
            seq.loop();

            if (seq.isEmpty()) {
                remove(current);
            }

            current = current->next;
        }
    }
};
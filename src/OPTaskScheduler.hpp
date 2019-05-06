#pragma once
#include "OPTask.hpp"
#include "OPComponent.hpp"

class OPTaskScheduler : public LinkedList<OPTask>, public OPComponent {
public:
    OPTaskScheduler(String name = "Scheduler") 
    : LinkedList(), OPComponent(name) { }
    
    void loop() override {
        auto current = head;
        while (current) {
            OPTask & task = current->data;
            if (task.isEmpty() || task.repeat == 0) {
                remove(current);
            } else {
                task.loop();
            }

            current = current->next;
        }
    }
};
#include "OPActionSequence.hpp"
#include "LinkedList.hpp"

/*
    Class for running multiple asyncronous tasks in parallel
*/
class OPActionSequenceScheduler : public OPComponent {
    private:
        LinkedList<OPActionSequence> sequences;
    public: 

    using OPComponent::OPComponent;

    void schedule(OPActionSequence const & sequence) {
        sequences.append(sequence);
    }

    void scheduleForever(OPActionSequence const & sequence) {
        
    }
    
    void setup() override {

    }

    void loop() override {
        if (sequences.isEmpty()) {
            return;
        }

        // Serial.println("Scheduler Loop");
        for (int i = sequences.size - 1; i >= 0; i--) {
            OPActionSequence & sequence = sequences.get(i);

            if (sequence.activated == false) {
                sequence.run();
            } else {
                sequence.loop();
            }

            if (sequence.isEmpty()) {
                sequences.remove(i);
            }
        }

        auto current = sequences.head;
        while (current) {
            OPActionSequence & seq = current->data;
            if (seq.activated) {
                seq.loop();
            } else {
                seq.run();
            }

            if (seq.isEmpty()) {
                sequences.remove(current);
            }
            
            current = current->next;
        }
    }
};
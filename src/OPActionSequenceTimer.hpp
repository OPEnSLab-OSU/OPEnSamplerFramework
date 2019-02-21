#include "OPActionSequence.hpp"
#include "LinkedList.hpp"


/*
    Class for running multiple asyncronous tasks in parallel
*/
class OPActionSequenceTimer : public OPComponent {
    private:
        LinkedList<OPActionSequence *> sequences;
    public: 
    void setup() override {

    }

    void loop() override {
        if (sequences.isEmpty()) {
            return;
        }

        for (int i = sequences.size - 1; i >= 0; i--) {
            OPActionSequence * sequence = sequences.get(i);
        }
    }
}
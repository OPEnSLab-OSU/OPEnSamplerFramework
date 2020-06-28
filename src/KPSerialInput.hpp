#pragma once
#include <KPFoundation.hpp>
#include <KPSubject.hpp>
#include <KPSerialInputObserver.hpp>
#include <vector>

class KPSerialInput : public KPSubject<KPSerialInputObserver> {
private:
	KPClearableString input;

public:
	void update() {
		while (Serial.available() > 0) {
			char inputChar = Serial.read();
			if (inputChar == '\n') {
				println();
				updateObservers(&KPSerialInputObserver::commandReceived, input);
				input.clear();
				print("> ");
				return;
			}

			// Ignore System characters
			if (inputChar >= 32) {
				input += inputChar;
				print(inputChar);
			}
		}
	}

	static KPSerialInput & instance() {
		static KPSerialInput serial;
		return serial;
	}
};
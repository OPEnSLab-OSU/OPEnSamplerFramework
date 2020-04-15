#pragma once
#include <KPFoundation.hpp>
// #include <KPEvent.hpp>
#include <vector>

class KPSerialInputListener {
public:
	virtual void commandReceived(const String & line) = 0;
};

class KPSerialInput {
private:
	KPClearableString input;
	std::vector<KPSerialInputListener *> listeners;

public:
	void addListener(KPSerialInputListener * delegate) {
		listeners.push_back(delegate);
	}

	void update() {
		while (Serial.available() > 0) {
			char inputChar = Serial.read();
			if (inputChar == '\n') {
				println();
				for (auto d : listeners) {
					d->commandReceived(input);
				}

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

#include <KPController.hpp>
#include <KPFileLoader.hpp>
#include <KPSerialInput.hpp>
#include <KPServer.hpp>
#include <KPStateMachine.hpp>
#include <KPStateMachineObserver.hpp>
#include <Action.hpp>

namespace {
	int read() {
		static int a = 5;
		if (a == -1) {
			a = 5;
		}

		println("Read");
		return --a;
	}
}  // namespace

void callback() {
	if (read() == 0) {
		cancel("check");
		run(1000, []() { runForever(200, "check", callback); });
	}
}

// replace read() with your actual read function
// Call this once to kick off the process:
// runForever(200, "check", callback); <--

void setup() {
	Serial.begin(115200);
	while (!Serial) {}
	runForever(200, "check", callback);
}

void loop() {
	ActionScheduler::sharedInstance().update();
}
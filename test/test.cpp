#include <unity.h>

#include <KPController.hpp>
#include <KPFileLoader.hpp>
#include <KPSerialInput.hpp>
#include <KPServer.hpp>
#include <KPStateMachine.hpp>
#include <Action.hpp>

void test_kpstring_concat() {
	auto name = "panda";
	KPStringBuilder<100> a("Once upon a time, there is a nice ", name, " wizard");
	TEST_ASSERT(a == "Once upon a time, there is a nice panda wizard");
}

void TestStringConstructors() {}

// #define TEST(func)                                                                                 \
// 	{ func, #func }

// typedef std::pair<void (*)(), const char *> TestEntry;
// std::initializer_list<TestEntry> tests = {TEST(test_kpstring_concat), TEST()};
int read() {
	static int a = 5;
	if (a == -1) {
		a = 5;
	}

	return a--;
}

void setup() {
	delay(5000);
	UNITY_BEGIN();
	TimedAction action;
	action.name		= "check";
	action.interval = 1000;
	action.callback = [action]() {
		if (read()) {
			// cancel and delay for 50ms
			cancel("check");
			run(5000, [action]() { runForever(action); });
		}
	};

	runForever(action);
	UNITY_END();
}

void loop() {}
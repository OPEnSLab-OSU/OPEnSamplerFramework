#include <unity.h>

#include <KPController.hpp>
#include <KPFileLoader.hpp>
#include <KPSerialInput.hpp>
#include <KPServer.hpp>
#include <KPStateMachine.hpp>

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

void setup() {
	delay(5000);
	UNITY_BEGIN();
	RUN_TEST(test_kpstring_concat);
	UNITY_END();
}

void loop() {}
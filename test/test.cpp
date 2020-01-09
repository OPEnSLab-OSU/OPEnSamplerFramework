class KPTestApplication;

#include <unity.h>

#include <KPAction.hpp>
#include <KPController.hpp>

class KPTestApplication : public KPController {
public:
    KPActionScheduler<10> scheduler{"scheduler"};

    void setup() override {
        addComponent(scheduler);
    }

    void update() override {
    }
};

KPTestApplication app;

void TestKPLocalString() {
    char name[] = "Kawin";
    KPLocalString<> s("Hi there ", name, ", how are you doing today?");
    TEST_ASSERT_TRUE(s == "Hi there Kawin, how are you doing today?");
}

void TestStateMachine() {
}

void TestKPAction() {
    KPActionChain<5> actionChain;
    actionChain
        .delay(1000, []() {
            println("First");
        })
        .delay(1000, []() {
            println("Second");
        })
        .delay(2000, []() {
            println("Third");
        })
        .then([&]() {
            KPActionChain<3> newActionChain;
            newActionChain.delay(1000, []() {
                println("Done");
            });
            run(newActionChain, app.scheduler);
        });

    run(actionChain, app.scheduler);
}

void setup() {
    delay(3000);
    UNITY_BEGIN();
    RUN_TEST(TestKPLocalString);
    RUN_TEST(TestStateMachine);
}

void loop() {
    UNITY_END();
}
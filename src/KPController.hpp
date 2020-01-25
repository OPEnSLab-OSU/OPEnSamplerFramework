#pragma once
#include <Array>
#include <KPFoundation.hpp>
#include <map>

class KPController {
private:
	std::map<const char *, KPComponent *> componentsByName;
	bool performComponentSetup = true;

public:
	virtual void setup() {}
	virtual void update() {
		// if (performComponentSetup) {
		//     performComponentSetup = false;
		//     for (auto & p : componentsByName) p.second->setup();
		// }

		for (auto & p : componentsByName) {
			p.second->update();
		}
	}

	void addComponent(KPComponent & c) {
		addComponent(&c);
		c.setup();
	}

	void addComponent(KPComponent * c) {
		componentsByName[c->name] = c;
		c->controller			  = this;
	}

	template <typename T>
	T * getComponent(const char * name) {
		auto c = componentsByName[name];
		if (c && c->enabled()) {
			return static_cast<T *>(c);
		} else {
			return nullptr;
		}
	}
};
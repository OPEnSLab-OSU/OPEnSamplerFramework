#pragma once
#include <KPFoundation.hpp>
#include <map>

class KPController {
private:
	std::map<const char *, KPComponent *> mapNameToComponent;

public:
	virtual void setup() = 0;
	virtual void update() {
		for (auto & p : mapNameToComponent) {
			p.second->update();
		}
	}

	void addComponent(KPComponent * c) {
		if (mapNameToComponent.find(c->name) != mapNameToComponent.end()) {
			halt(TRACE, c->name, " already exists");
		}

		mapNameToComponent[c->name] = c;
		c->controller				= this;
		c->setup();
		println(c->name, " setup");
	}

	void addComponent(KPComponent & c) {
		addComponent(&c);
	}

	template <typename T>
	T * getComponent(const char * name) {
		auto c = mapNameToComponent[name];
		if (c && c->enabled()) {
			return static_cast<T *>(c);
		}

		return nullptr;
	}
};
#pragma once
#include <KPFoundation.hpp>
#include <KPObserver.hpp>

class KPSerialInputObserver : public KPObserver {
public:
	static const char * ObserverName() {
		return "<Unnamed> Serial Input Observer";
	}
	virtual void commandReceived(const String & line) = 0;
};
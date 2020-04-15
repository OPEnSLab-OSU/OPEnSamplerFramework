#include "KPFoundation.hpp"

Verbosity PrintConfig::printVerbose		 = PrintConfig::defaultPrintVerbose;
const char * PrintConfig::printSeparator = "";

int free_ram() {
	char stack_dummy = 0;
	return &stack_dummy - sbrk(0);
}

void printFreeRam() {
	print("Free Memory: ");
	println(free_ram());
}

void raise(Error e, long ms) {
	while (true) {
		println(e);
		delay(ms);
	}
}

void raise(const char * error, long ms) {
	raise(Error(error), ms);
}

int strcmpi(const char * left, const char * right) {
	for (int i = 0;; i++) {
		int l = left[i], r = right[i];
		if (l == 0 && r == 0) {
			return 0;
		}

		if (toupper(l) != toupper(r)) {
			return l - r;
		}
	}
}

#include "KPFoundation.hpp"

Verbosity Config::printVerbose = Config::defaultPrintVerbose;
const char * Config::printSeparator = "";

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
		Serial.printf("%s: %s", e.name ? e.name : "Error", e.message);
        delay(ms);
    }
}

int strcmpi(const char * left, const char * right) {
	for (int i = 0;;i++) {
		int l = left[i], r = right[i];
		if (l == 0 && r == 0) {
			return 0;
		} 

		if (toupper(l) != toupper(r)) {
			return l - r;
		}
	} 
}

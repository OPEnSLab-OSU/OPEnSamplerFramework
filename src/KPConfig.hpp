#pragma once

enum class Verbosity: int {
	none, error, info, debug
};

struct Config {
private:
	Config() {
		
	}
public:
	static const Verbosity defaultPrintVerbose = Verbosity::debug;
	static Verbosity printVerbose;
	static const char * printSeparator;
};
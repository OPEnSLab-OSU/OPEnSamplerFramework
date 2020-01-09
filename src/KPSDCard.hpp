#pragma once
#include <KPFoundation.hpp>
#include <KPDataStoreInterface.hpp>
#include <SD.h>

class KPSDCard: public KPDataStoreInterface {
private:
	int pin;
public:
	KPSDCard(const char * name, int pin): KPDataStoreInterface(name), pin(pin) {}

	void setup() override {
		SD.begin(pin);
	}

	int loadContentOfFile(const char * filepath, char * buffer, size_t bufferSize) override {
		if (bufferSize <= 0 || buffer == nullptr || filepath == nullptr) {
			println("Invalid arguments", filepath, buffer, bufferSize);
			return 0;
		}

		println("Loading content of file: ", filepath);

		// Keep track of the position in the file
		static size_t position = 0;
		static const char * prevFilepath = nullptr;
		if (prevFilepath == nullptr || strcmp(filepath, prevFilepath) != 0) {
			position = 0;
		}

		prevFilepath = filepath;

		File file = SD.open(filepath, FILE_READ);
		if (!file) {
			println("File not found: ", filepath);
			position = 0;
			return 0;
		} else {
			println("File found: ", file.name());
		}

		// Calculate how many bytes are remaining to be read
		size_t bytesRemaining = file.size() - position;
		println("Bytes Remaining: ", bytesRemaining);
		if (bytesRemaining <= 0) {
			position = 0;
			println("EOF");
			file.close();
			return 0;
		}

		// Copy 
		int size = constrain(bytesRemaining, 0, bufferSize - 1);
		buffer[size] = 0;
		file.seek(position);
		file.read(buffer, size);
		file.close();

		position += size;		
		return size;
	}

	int saveContentToFile(const char * filepath, char * buffer, size_t bufferSize) override {
		return 0;
	}
};


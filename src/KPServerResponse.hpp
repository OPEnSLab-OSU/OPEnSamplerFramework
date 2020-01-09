
#pragma once
#include <WiFi101.h>

#include <KPDataStoreInterface.hpp>
#include <KPFoundation.hpp>
#include <map>
#include <ArduinoJson.h>

struct Response {
private:
    int status         = 200;
    bool headerPending = true;
    std::map<const char *, const char *> headers{
        {"Content-Type", "text/html"},
        {"Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS"},
        {"Access-Control-Allow-Origin", "*"}};

    std::function<void()> _notFound;

    void sendHeader() {
        client.printf("HTTP/1.1 %d %s\r\n", status, statusText(status));
        for (auto & p : headers) {
            client.printf("%s: %s\r\n", p.first, p.second);
        }

        client.print("\r\n");
        headerPending = false;
    }

    const char * statusText(int code) const {
        switch (code) {
        case 200:
            return "OK";
        default:
            return "Not Found";
        }
    }

public:
    WiFiClient & client;
	const size_t TCP_LIMIT;
    Response(WiFiClient & client, const size_t tcp_limit)
        : client(client), TCP_LIMIT(tcp_limit) {
        _notFound = [&]() {
            client.println("HTTP/1.1 404 Not Found");
            client.println("Connection: close");
            client.print("\r\n");
        };
    }

    void setHeader(const char * key, const char * value) {
		for (auto p: headers) {
			if (strcmpi(p.first, key) == 0) {
				headers[p.first] = value; // update
				return;
			}
		}
		
        headers[key] = value;
    }

    template <typename T>
    void send(const T & data) {
        if (headerPending) {
            sendHeader();
        }

        client.print(data);
    }

	template<size_t N>
	void send(const char (&data)[N]) {
		if (N > TCP_LIMIT) {
			Config::printVerbose = Verbosity::info;
			println("Warning: data exeeds TCP limit. All or some of it may be lost."); 
			println("Try to reduce the amount of data sent at once to be below ", TCP_LIMIT, " bytes");
			Config::printVerbose = Verbosity::debug;
		}

		if (headerPending) {
			sendHeader();
		} 

		client.print(data);
	}
 
	template<size_t size>
	void sendJson(const JsonDocument & doc) {
		char buffer[size]{0};
		serializeJson(doc, buffer, size);
		send(buffer);
	}

    void end() {
        if (headerPending) {
            sendHeader();
        }

		client.print("\r\n\r\n");
        client.stop();
    }

    void onNotFound(std::function<void()> callback) {
        _notFound = callback;
    }

    void notFound() {
        _notFound();
    }
};

//     void serveContentFromFile(const char * filepath, KPSDCard & card) {
//         const int size = 4096;
//         char buffer[size]{0};
//         while (card.loadContentsOfFileInParts(filepath, buffer, size)) {
//             send(buffer);
// 			println("sending...");
//         }
//     }
// };
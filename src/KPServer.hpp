#pragma once
#include <WiFi101.h>

#include <KPFoundation.hpp>
#include <KPServerRequest.hpp>
#include <KPServerResponse.hpp>
#include <vector>
#include <functional>

#define HTTP_GET  "GET"
#define HTTP_POST "POST"

using EndPointCallback = std::function<void(Request &, Response &)>;

struct RequestHandler {
	const char * path;
	const char * method;
	EndPointCallback callback;

	RequestHandler() {}

	RequestHandler(const char * path, const char * method, EndPointCallback callback)
		: path(path),
		  method(method),
		  callback(callback) {}
};

class KPServer : public KPComponent {
public:
	const char * ssid;
	const char * pass;

	uint8_t status = WL_IDLE_STATUS;
	bool isRunning = false;

	std::vector<RequestHandler> handlers;
	WiFiServer server;

	const size_t TCP_LIMIT = 1400;

	KPServer(const char * name, const char * ssid, const char * pass, const size_t tcp_limit = 1400)
		: KPComponent(name),
		  ssid(ssid),
		  pass(pass),
		  server(80),
		  TCP_LIMIT(tcp_limit) {}

	void setup() override {
		WiFi.setPins(8, 7, 4, 2);
		WiFi.lowPowerMode();
	}

	void begin() {
		while ((status = WiFi.beginAP(ssid, pass)) != WL_AP_LISTENING) {
			println("WiFi AP Mode Failed to Initialize");
			println("Try again in ");
			for (char i = 3; i > 0; i++) {
				println(i);
				delay(1000);
			}
		}

		isRunning = true;
	}

	void update() override {
		if (!isRunning) {
			return;
		}

		// WiFi status has changed
		if (status != WiFi.status()) {
			if ((status = WiFi.status()) == WL_AP_CONNECTED) {
				byte remoteMac[6];
				println("Device connected to AP");
				print("MAC Address: ");
				printMacAddress(WiFi.APClientMacAddress(remoteMac));
				server.begin();
			} else {
				println("Device disconnected from AP");
			}
		}

		// Short circuit if client is not connected
		if (status != WL_AP_CONNECTED) {
			return;
		}

		WiFiClient client = server.available();
		if (client && client.connected() && client.available()) {
			constexpr const int size   = 4095;
			char httpRequest[size + 1] = {0};
			auto count				   = 0;

			while (client.available()) {
				httpRequest[count++] = client.read();
			}

			if (strstr(httpRequest, "Content-Length:")) {
				delay(10);
				while (client.available()) {
					httpRequest[count++] = client.read();
				}
			}

			// Construct a request object and handle accordingly
			client.flush();
			Request request(httpRequest, client);
			handleRequest(request);
		}
	}

	void on(const char * path, const char * method, EndPointCallback callback) {
		handlers.push_back(RequestHandler(path, method, callback));
	}

	void get(const char * path, EndPointCallback callback) {
		on(path, HTTP_GET, callback);
	}

	void post(const char * path, EndPointCallback callback) {
		on(path, HTTP_POST, callback);
	}

	void serveStaticFile(const char * path, const char * filepath, KPDataStoreInterface & store,
						 const char * contentType) {
		on(path, HTTP_GET, [=, &store](Request & req, Response & res) {
			constexpr int size = 1400;
			char buffer[size]{0};
			res.setHeader("Content-Type", contentType);
			while (store.loadContentOfFile(filepath, buffer, size)) {
				res.send(buffer);
			}
			res.end();
		});
	}

	//===========================================================
	// [+_+] Search for the coresponding request handler and call the callback function
	//===========================================================
	void handleRequest(Request & req) {
		WiFiClient & client = req.client;
		Response res(client, TCP_LIMIT);

		// Preflight
		if (strcmp(req.method, "OPTIONS") == 0) {
			// client.println("Access-Control-Allow-Origin: *");
			return;
		}

		for (auto & handler : handlers) {
			if (strcmp(handler.path, req.path) == 0 && strcmp(handler.method, req.method) == 0) {
				// println("Routing to", handler.path);
				// printFreeRam();
				handler.callback(req, res);
				return;
			}
		}

		res.notFound();
	}

	void printWiFiStatus() {
		// SSID:
		println("SSID: ", WiFi.SSID());

		// IP address:
		IPAddress ip = WiFi.localIP();
		println("IP Address: ", ip);

		// Signal strength:
		long rssi = WiFi.RSSI();
		println("Signal strength (RSSI): ", rssi, "dBM");

		// Address:
		println("Web Browser: http://", ip);
	}

	void printMacAddress(byte mac[]) {
		for (int i = 5; i >= 0; i--) {
			if (mac[i] < 16) {
				print("0");
			}
			Serial.print(mac[i], HEX);
			if (i > 0) {
				print(":");
			}
		}
		println();
	}
};
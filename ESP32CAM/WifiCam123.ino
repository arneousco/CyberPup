#include "WifiCam.hpp"
#include <WiFi.h>
#include <ESPmDNS.h>

static const char* WIFI_SSID = "LAPTOP-K5PP";
static const char* WIFI_PASS = "kartikay";

esp32cam::Resolution initialResolution;

WebServer server(80);
WiFiClient client = server.client();

// void handlePostRequest() {
//   if (server.hasArg("plain") == false) { //Check if body received
//     server.send(200, "text/plain", "Body not received");
//     return;
//   }

//   String message = server.arg("plain");
//   // Do something with the message
//   // For example, store it in a variable
//   server.send(200, "text/plain", "Message received: " + message);
// }

// void addRequestHandlers() {
//   server.on("/post", HTTP_POST, handlePostRequest);
// }

void setup() {
  Serial.begin(115200);
  Serial.println();
  delay(2000);

  // Connect to WiFi
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi connection failed");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("WiFi connected");

  // Setup mDNS
  if (!MDNS.begin("my-esp32cam")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // Initialize camera
  {
    using namespace esp32cam;

    initialResolution = Resolution::find(1024, 768);

    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(initialResolution);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    if (!ok) {
      Serial.println("camera initialize failure");
      while (1) {
        delay(1000);
      }
    }
    Serial.println("camera initialize success");
  }

  // Print camera URL
  Serial.print("Camera streaming at: http://");
  Serial.print("my-esp32cam.local");
  Serial.println("/stream");

  // Setup web server
  addRequestHandlers();
  server.begin();
}

void loop() {
  server.handleClient();
}
/*
 * Smart Home System - ESP32 Firebase Realtime Streaming
 * Uses SSE streaming (no polling) to monitor /devices/LED
 * Controls GPIO 2 based on LED value
 */

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <esp_wifi.h>
#include <FirebaseClient.h>

// --- Configuration ---
#define WIFI_SSID     "Galaxy A24 D5BA"
#define WIFI_PASSWORD "12345678safa"
#define DATABASE_URL  "smarthome-6e1fa-default-rtdb.europe-west1.firebasedatabase.app"
#define API_KEY       "AIzaSyCx3PDoX4KB3rcEn8Dj94fiOguzHKRoai0"

// --- Hardware ---
#define LED_PIN 2

// --- Objects ---
WiFiClientSecure ssl_client, stream_ssl_client;

using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client), streamClient(stream_ssl_client);

UserAuth user_auth(API_KEY, "esp32@smarthome.local", "SmartHome2026!", 3000);
FirebaseApp app;
RealtimeDatabase Database;

// --- State ---
int lastLedState = -1;

// --- Forward Declarations ---
void processData(AsyncResult &aResult);

// --- Auth Debug ---
void auth_debug_print(AsyncResult &aResult) {
  if (aResult.isEvent())
    Firebase.printf("[Auth] %s, code: %d\n", aResult.eventLog().message().c_str(), aResult.eventLog().code());
  if (aResult.isError())
    Firebase.printf("[Auth] Error: %s, code: %d\n", aResult.error().message().c_str(), aResult.error().code());
}

// --- WiFi ---
void connectWiFi() {
  Serial.println("\n========================================");
  Serial.println("   Smart Home System - Booting...       ");
  Serial.println("========================================\n");
  Serial.print("[WiFi] Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.disconnect(true, true);
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  
  // Force 802.11bgn (2.4GHz only) for dual-band routers
  esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT20);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts % 20 == 0) {
      Serial.printf(" (%d sec)\n", attempts / 2);
      // Retry connection
      WiFi.disconnect();
      delay(500);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
    if (attempts > 120) {
      Serial.println("\n[WiFi] Failed after 60s. Restarting...");
      ESP.restart();
    }
  }
  Serial.println();
  Serial.println("[WiFi] Connected!");
  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("[WiFi] RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

// --- Process LED Value ---
void processLedValue(int value) {
  if (value != 0 && value != 1) return;
  if (value != lastLedState) {
    digitalWrite(LED_PIN, value ? HIGH : LOW);
    Serial.printf("[LED] %s - GPIO 2 %s\n", value ? "ON" : "OFF", value ? "HIGH" : "LOW");
    lastLedState = value;
  } else {
    Serial.println("[LED] No change");
  }
}

// --- Stream Callback ---
void processData(AsyncResult &aResult) {
  if (!aResult.isResult()) return;

  if (aResult.isEvent())
    Firebase.printf("[Event] %s, code: %d\n", aResult.eventLog().message().c_str(), aResult.eventLog().code());

  if (aResult.isError())
    Firebase.printf("[Error] %s, code: %d\n", aResult.error().message().c_str(), aResult.error().code());

  if (aResult.available()) {
    RealtimeDatabaseResult &stream = aResult.to<RealtimeDatabaseResult>();
    if (stream.isStream()) {
      String path = stream.dataPath();
      String data = stream.to<String>();

      Serial.println("----------------------------------------");
      Firebase.printf("[Stream] event: %s\n", stream.event().c_str());
      Firebase.printf("[Stream] path : %s\n", path.c_str());
      Firebase.printf("[Stream] data : %s\n", data.c_str());

      if (path == "/LED" || path == "LED") {
        processLedValue(data.toInt());
      } else if (path == "/") {
        int idx = data.indexOf("\"LED\":");
        if (idx >= 0) {
          int vs = idx + 6;
          while (vs < (int)data.length() && data[vs] == ' ') vs++;
          if (vs < (int)data.length() && data[vs] >= '0' && data[vs] <= '9')
            processLedValue(data[vs] - '0');
        }
      }
      Serial.println("----------------------------------------");
    }
    Firebase.printf("[Heap] %d bytes free\n", ESP.getFreeHeap());
  }
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  connectWiFi();

  ssl_client.setInsecure();
  stream_ssl_client.setInsecure();

  Serial.println("[Firebase] Initializing...");
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  initializeApp(aClient, app, getAuth(user_auth), auth_debug_print, "authTask");

  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);

  streamClient.setSSEFilters("get,put,patch,keep-alive,cancel,auth_revoked");

  Serial.println("[Firebase] Starting stream on /devices ...");
  Database.get(streamClient, "/devices", processData, true, "streamTask");

  Serial.println("\n========================================");
  Serial.println("   System ready - Listening...          ");
  Serial.println("========================================\n");
}

// --- Loop ---
void loop() {
  app.loop();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Disconnected - reconnecting...");
    connectWiFi();
  }
  delay(10);
}

// File: http_esp32_client_dht11.ino
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT11.h>   // gunakan library DHT11.h (bukan Adafruit DHT)

// === Konfigurasi DHT11 ===
#define DHTPIN 4        // pin data sensor DHT11 ke GPIO 4
DHT11 dht11(DHTPIN);

// === WiFi ===
#define WIFI_SSID "ITTS "
#define WIFI_PASS "ittsok"

// === Alamat server Flask ===
// Contoh endpoint: http://192.168.120.100:8080/ingest
const char* HTTP_URL = "http://192.168.120.121:8080/ingest";

unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL_MS = 2500;  // jeda 2,5 detik agar DHT11 stabil

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("[WiFi] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n[WiFi] Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(100);
  wifiConnect();
  Serial.println("[SETUP] DHT11 initializing...");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) wifiConnect();

  unsigned long now = millis();
  if (now - lastSend >= SEND_INTERVAL_MS) {
    lastSend = now;

    // --- Baca DHT11 ---
    int temperature = 0;
    int humidity = 0;
    int chk = dht11.readTemperatureHumidity(temperature, humidity);
    if (chk != 0) {  // 0 = OK
      Serial.println("[DHT11] Gagal membaca sensor");
      return;
    }

    // --- Baca nilai analog (contoh pin 34) ---
    int analogValue = analogRead(34) % 100;

   
    String payload = String("{") +
                     "\"device_id\":\"esp32-http-001\"," +
                     "\"seq\":" + String(now / 1000) + "," +
                     "\"temp_c\":" + String(temperature) + "," +
                     "\"hum_rh\":" + String(humidity) + "," +
                     "\"analog\":" + String(analogValue) +
                     "}";

    // --- Kirim ke server Flask ---
    HTTPClient http;
    http.begin(HTTP_URL);
    http.addHeader("Content-Type", "application/json");

    int code = http.POST(payload);
    String resp = http.getString();
    http.end();

    Serial.printf("[HTTP] POST %d, resp: %s | T=%d°C, H=%d%%\n",
                  code, resp.c_str(), temperature, humidity);
  }
}

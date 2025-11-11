/// File: mqtt_esp32_publisher_dht11_simple.ino
#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <DHT11.h>   // gunakan library DHT11.h

// === Konfigurasi DHT11 ===
#define DHTPIN 4        // pin data sensor DHT11 ke GPIO 4
DHT11 dht11(DHTPIN);

// === WiFi & MQTT ===
#define WIFI_SSID "ITTS"
#define WIFI_PASS "ittsok"

// IP broker Mosquitto (contoh: laptop 192.168.1.100)
const char* MQTT_HOST = "192.168.120.121";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_TOPIC = "IOTS/LAB/telemetry";

WiFiClient espClient;
PubSubClient mqtt(espClient);

unsigned long lastPub = 0;
const unsigned long PUB_INTERVAL_MS = 2500;  // jeda antar publish
String device_id = "esp32-dev-001";

// --- Sinkronisasi waktu (NTP) ---
void syncTime() {
  configTime(7 * 3600, 0, "pool.ntp.org", "time.google.com"); // WIB
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println("[NTP] Time synced");
  } else {
    Serial.println("[NTP] Failed, continue without exact time");
  }
}

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

void mqttReconnect() {
  while (!mqtt.connected()) {
    Serial.print("[MQTT] Connecting...");
    String willPayload = String("{\"device_id\":\"") + device_id + "\",\"status\":\"offline\"}";
    if (mqtt.connect(device_id.c_str(), nullptr, nullptr,
                     MQTT_TOPIC, 1, false, willPayload.c_str())) {
      Serial.println("connected");
      String online = String("{\"device_id\":\"") + device_id + "\",\"status\":\"online\"}";
      mqtt.publish(MQTT_TOPIC, online.c_str(), false);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" retry in 2s");
      delay(2000);
    }
  }
}

String isoTimestamp() {
  time_t now = time(nullptr);
  struct tm tmNow;
  gmtime_r(&now, &tmNow);
  char buf[32];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tmNow);
  return String(buf);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  wifiConnect();
  syncTime();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqttReconnect();

  pinMode(34, INPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) wifiConnect();
  if (!mqtt.connected()) mqttReconnect();
  mqtt.loop();

  unsigned long now = millis();
  if (now - lastPub >= PUB_INTERVAL_MS) {
    lastPub = now;

    // --- Baca data sensor DHT11 ---
    int temperature = 0;
    int humidity = 0;
    int chk = dht11.readTemperatureHumidity(temperature, humidity);

    if (chk != 0) {  // 0 = OK
      Serial.println("[DHT11] Error membaca data sensor");
      return;
    }

    int adc_raw = analogRead(34) % 4096;

    String payload = String("{") +
      "\"device_id\":\"" + device_id + "\"," +
      "\"ts\":\"" + isoTimestamp() + "\"," +
      "\"temp_c\":" + String(temperature) + "," +
      "\"hum_rh\":" + String(humidity) + "," +
      "\"adc_raw\":" + String(adc_raw) +
    "}";

    bool ok = mqtt.publish(MQTT_TOPIC, payload.c_str(), false);
    Serial.printf("[MQTT] Publish len=%u ok=%d | T=%d°C H=%d%%\n",
                  payload.length(), ok, temperature, humidity);
  }
}

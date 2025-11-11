# Esp32_Protokol# 🌐 ESP32 IoT Telemetry Project

## 🧩 Deskripsi Proyek
Proyek ini merupakan implementasi **IoT berbasis ESP32** yang berfungsi untuk mengirim dan menerima data sensor melalui dua protokol utama:

- **HTTP (REST API)** → untuk mengirim data ke server Flask.  
- **MQTT (Broker Mosquitto)** → untuk komunikasi publish/subscribe secara real-time.

Proyek ini digunakan dalam konteks **praktikum IoT**, **pengujian sensor (DHT11)**, serta **simulasi komunikasi antarperangkat (ESP32 ↔ Laptop)**.
## Arsitektur Folder server
    praktikumiot/
    ├── http_server/
    │   ├── server.py
    │   └── .env
    ├── mqtt_broker/
    │   └── docker-compose.yml
    ├── esp32/
    │   ├── http_esp32_client_dht11.ino
    │   ├── mqtt_esp32_publisher.ino
    │   ├── mqtt_esp32_subscriber.ino
    │   └── config.h
    └── README.md

## aktifkan server untuk server http
    cd praktikumiot/http_server
    export HTTP_HOST=0.0.0.0
    export HTTP_PORT=8080
    export CSV_PATH=ingest_log.csv
    export TZ_OFFSET_MIN=420
    python3 server.py

## Jalankan MQTT Broker (Docker)
    cd praktikumiot/mqtt_broker
    docker run -d \
      --name mosquitto \
      -p 1883:1883 -p 9001:9001 \
      -v $(pwd)/config:/mosquitto/config \
      -v $(pwd)/data:/mosquitto/data \
      -v $(pwd)/log:/mosquitto/log \
      eclipse-mosquitto:latest

---

## ⚙️ Arsitektur Sistem

```mermaid
flowchart LR
    subgraph Device
        ESP32["ESP32 + DHT11\n(Client)"]
    end

    subgraph Server
        Flask["Flask Server (HTTP)"]
        Mosquitto["Mosquitto Broker (MQTT)"]
    end

    ESP32 -- "POST /ingest" --> Flask
    ESP32 -- "Publish telemetry" --> Mosquitto
    Laptop["Laptop Subscriber"] -- "Subscribe topic" --> Mosquitto



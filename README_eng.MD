Oke, Kak — kemungkinan error-nya muncul karena **blok kode** atau **diagram Mermaid** dalam README tadi belum diformat sesuai standar GitHub Markdown.
Berikut versi **aman untuk langsung di-copy ke GitHub** tanpa error, sudah saya bersihkan dari karakter yang bisa menyebabkan parsing issue 👇

---

```markdown
# ESP32 IoT Protocol Examples

## Overview
This repository provides implementation examples using the ESP32 microcontroller with common IoT communication protocols.  
It demonstrates how to send telemetry and sensor data from devices to a server using:

- **HTTP (REST API)** — send data to a Flask-based server.  
- **MQTT (Publish/Subscribe)** — real-time data transfer using an MQTT broker (e.g., Mosquitto).

These examples are suitable for IoT lab sessions, IoT coursework, and prototype development.

---

## Project Structure


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



- **http_server/** — Flask HTTP server for receiving data.  
- **mqtt_broker/** — Docker configuration for Mosquitto MQTT broker.  
- **esp32/** — Arduino sketches for ESP32 (HTTP and MQTT examples).  
- **config.h** — WiFi and broker/server configuration file.

---

## Getting Started

### 1. Run the HTTP Server
```bash
cd praktikumiot/http_server
export HTTP_HOST=0.0.0.0
export HTTP_PORT=8080
export CSV_PATH=ingest_log.csv
export TZ_OFFSET_MIN=420
python3 server.py
````

### 2. Start the MQTT Broker

```bash
cd praktikumiot/mqtt_broker
docker run -d \
  --name mosquitto \
  -p 1883:1883 -p 9001:9001 \
  -v $(pwd)/config:/mosquitto/config \
  -v $(pwd)/data:/mosquitto/data \
  -v $(pwd)/log:/mosquitto/log \
  eclipse-mosquitto:latest
```

### 3. Flash ESP32 Device

* Upload `http_esp32_client_dht11.ino` to your ESP32 board.
* Or upload `mqtt_esp32_publisher.ino` / `mqtt_esp32_subscriber.ino`.
* Edit `config.h` for your WiFi and server/broker settings.

---

## System Architecture

```
[ESP32 + DHT11] ---> (HTTP POST) ---> [Flask Server]
       |
       +---> (MQTT Publish) ---> [Mosquitto Broker] ---> [Subscriber Laptop]
```

---

## Why This Project?

* Demonstrates real-world IoT protocols on ESP32.
* Perfect for IoT learning labs and coursework.
* Covers both REST (HTTP) and Publish/Subscribe (MQTT) communication models.
* Easy to expand with more sensors or dashboard integration.

---

## Future Improvements

* Add SSL/TLS for secure communication.
* Support more sensors and data logging.
* Integrate cloud IoT platforms (AWS, Azure, GCP).
* Build dashboards for real-time monitoring.

---

## Contributing

Pull requests and feature suggestions are welcome!
Please ensure your code:

* Includes clear comments and documentation.
* Notes new dependencies in the README.
* Is properly tested and formatted.

---

## License

This project is licensed under the **MIT License** (you may replace with your preferred license).

---

## Author

Maintained by **Aolia Ikhwanudin **
Institut Teknologi Tangerang Selatan
For collaboration or questions, please reach out via GitHub Issues.

---

*Thank you for visiting this project! I hope it helps you explore and build IoT systems using ESP32.*

```

---


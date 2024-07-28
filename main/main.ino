#include <WiFi.h>
#include <PubSubClient.h>

// WiFi settings
const char* ssid = "YOUR_SSID_HERE";
const char* password = "YOUR_PASSWORD_HERE";

// MQTT settings
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int PulseSensorYellowPin = 36;
const int LM35Pin = 34;
const int LED13 = 2;

int Signal;
int Threshold = 2000;
unsigned long lastBeatTime = 0;
unsigned long IBI = 600;
int BPM;
boolean pulseDetected = false;
unsigned long lastPrintTime = 0;
const int printInterval = 1000;

bool mqttConnected = false;

void setup() {
  pinMode(LED13, OUTPUT);
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  connectMQTT();
}

void loop() {
  if (!mqttConnected) {
    connectMQTT();
  } else {
    client.loop();
    processHealthData();
  }
  delay(10);
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("health/status", "MQTT connected");
      mqttConnected = true;
      
      Serial.println("-----------------------------");
      Serial.println("MQTT Connection Established!");
      Serial.println("Starting health monitoring...");
      Serial.println("-----------------------------");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void processHealthData() {
  unsigned long currentTime = millis();
  Signal = analogRead(PulseSensorYellowPin);

  if (Signal > Threshold && !pulseDetected) {
    pulseDetected = true;
    digitalWrite(LED13, HIGH);

    IBI = currentTime - lastBeatTime;
    lastBeatTime = currentTime;

    if (IBI > 0) {
      int calculatedBPM = 60000 / IBI;
      if (calculatedBPM >= 60 && calculatedBPM <= 200) {
        BPM = calculatedBPM;
      }
    }
  }

  if (Signal < Threshold && pulseDetected) {
    pulseDetected = false;
    digitalWrite(LED13, LOW);
  }

  if (currentTime - lastPrintTime >= printInterval) {
    float temperature = readTemperature();

    if (temperature >= 25 && temperature <= 50) {
      char msg[50];
      snprintf(msg, 50, "BPM: %d, Temperature: %.2f°C", BPM, temperature);
      Serial.println(msg);
      client.publish("health/bpm", String(BPM).c_str());
      client.publish("health/temperature", String(temperature).c_str());
    }

    lastPrintTime = currentTime;
  }
}

float readTemperature() {
  int analogValue = analogRead(LM35Pin);
  float voltage = analogValue * (3.3 / 4095.0);
  float temperature = voltage * 100;
  return temperature;
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);
}

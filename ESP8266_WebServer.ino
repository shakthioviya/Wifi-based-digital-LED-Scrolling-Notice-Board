/* ESP8266 (NodeMCU) - Subscribe to Adafruit IO feed and display on I2C LCD
   Uses: PubSubClient for MQTT and LiquidCrystal_I2C for the LCD.
   Install via Arduino Library Manager: PubSubClient, LiquidCrystal_I2C
   Wiring: I2C LCD SDA -> D2 (GPIO4), SCL -> D1 (GPIO5). If different, adapt Wire.begin()
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --------- USER CONFIG ----------
const char* WIFI_SSID = "sairam";
const char* WIFI_PASS = "sairam2705";

const char* ADAFRUIT_USERNAME = "shakthioviya"; // e.g. "oviyap"
const char* ADAFRUIT_KEY = "aio_hs89KLMnopQ12rsTUVwxYZ34ABcdEf56";       // AIO key (secret)
const char* FEEDNAME = "noticeboard";                    // the Adafruit feed name

// MQTT broker for Adafruit IO
const char* mqtt_server = "io.adafruit.com";
const uint16_t mqtt_port = 1883;

// Set LCD I2C address (0x27 or 0x3F commonly). Change if needed.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Serial forwarding: if true, the ESP also forwards messages to Serial1 (TX-only) or Serial
// If you want to send to an Arduino Nano, connect NodeMCU TX -> level shifter -> Nano RX
const bool FORWARD_TO_SERIAL = true; // set false if not needed
// -------------------------------

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

String subscribeTopic;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
    if (millis() - start > 20000UL) { // 20s timeout
      Serial.println("\nWiFi connection failed. Resetting...");
      ESP.restart();
    }
  }
  Serial.println();
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // payload is NOT null-terminated
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("MQTT msg on [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  // Display on LCD
  displayOnLCD(message);

  // Forward to Serial (Nano) if needed
  if (FORWARD_TO_SERIAL) {
    // Use Serial (USB) or Serial1 (TX-only). On NodeMCU, Serial1 is TX-only (GPIO2).
    // Here we send to Serial (this will appear in USB monitor AND on TX pin).
    Serial.println(message); // careful: connecting TX to Nano RX will forward this line
  }
}

void connectMQTT() {
  // client id must be unique
  String clientId = "ESP-" + String(ESP.getChipId(), HEX);
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);

  Serial.print("Connecting to Adafruit IO MQTT... ");
  while (!mqttClient.connected()) {
    // Adafruit IO requires username = AIO username, password = AIO key
    if (mqttClient.connect(clientId.c_str(), ADAFRUIT_USERNAME, ADAFRUIT_KEY)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 3s");
      delay(3000);
    }
  }
  // Subscribe to the feed topic
  subscribeTopic = String(ADAFRUIT_USERNAME) + "/feeds/" + FEEDNAME;
  mqttClient.subscribe(subscribeTopic.c_str());
  Serial.print("Subscribed to: ");
  Serial.println(subscribeTopic);
}

void displayOnLCD(const String &msg) {
  lcd.clear();
  // If LCD is 16x2, we break message across two lines
  if (msg.length() <= 16) {
    lcd.setCursor(0, 0);
    lcd.print(msg);
  } else {
    lcd.setCursor(0, 0);
    lcd.print(msg.substring(0, 16));
    lcd.setCursor(0, 1);
    if (msg.length() > 16) lcd.print(msg.substring(16, min(32, (int)msg.length())));
  }
}

void setup() {
  Serial.begin(115200);      // Primary serial for debugging; also used to forward if desired.
  delay(10);
  Wire.begin();              // Use default I2C pins on NodeMCU (D2=D4? depends on board); works usually.
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Waiting msg...");

  connectWiFi();
  connectMQTT();
}

void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();
}

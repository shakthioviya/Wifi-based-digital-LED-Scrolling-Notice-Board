/* Arduino Nano: Matrix + LCD + Buzzer Serial Receiver
   - Requires: MD_Parola, MD_MAX72xx, SPI, LiquidCrystal_I2C
   - Connect MAX7219: DATA -> D11, CLK -> D13, CS -> D10
   - Connect I2C LCD: SDA -> A4, SCL -> A5 (address commonly 0x27)
   - Buzzer -> D9 (active buzzer)
*/

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8     // change to number of cascaded 8x8 modules you have
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

MD_Parola display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// I2C LCD - adjust address if needed
LiquidCrystal_I2C lcd(0x27, 20, 4); // change to 16,2 or 20,4 if needed

const int buzzerPin = 9;

String incoming = "";
bool newMessage = false;

void setup() {
  Serial.begin(115200); // Must match ESP8266 serial baud in its sketch
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Initialize display
  display.begin();
  display.setIntensity(5);  // 0-15
  display.displayClear();
  display.setPause(10);
  display.displayReset();

  // Initialize LCD
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Notice Board Ready");

  delay(1000);
  lcd.clear();
  Serial.println("Nano ready");
}

void loop() {
  // Read serial lines (terminated by '\n')
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      incoming.trim();
      if (incoming.length() > 0) {
        handleMessage(incoming);
      }
      incoming = "";
    } else {
      incoming += c;
      // safety: cap length
      if (incoming.length() > 400) incoming = incoming.substring(incoming.length()-400);
    }
  }

  // update display engine
  if (display.displayAnimate()) {
    // nothing to do, Parola handles animation
  }
}

void handleMessage(String msg) {
  Serial.print("New message: ");
  Serial.println(msg);

  // beep buzzer
  tone(buzzerPin, 2000, 120); // freq 2kHz, duration 120ms

  // show on LCD (wrap into first two lines)
  lcd.clear();
  lcd.setCursor(0,0);
  if (msg.length() <= 20) {
    lcd.print(msg);
  } else {
    lcd.print(msg.substring(0,20));
    lcd.setCursor(0,1);
    if (msg.length() > 20) lcd.print(msg.substring(20, min(40, msg.length())));
  }

  // Show on MAX7219 matrix using Parola
  // Configure Parola: scroll speed and pause
  display.displayClear();
  display.setTextAlignment(PA_LEFT);
  display.displayText(msg.c_str(), PA_CENTER, 80, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  // parameters: (text, alignment, speed(ms), pause(ms), effect_in, effect_out)
}



#include <Wire.h>
#include <ArduinoJson.h> // V6!
#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>
#include "images.h"

/*
 * Don't rate this code! It's a project for fun :)
 */


/*
 * !!! IMPORTANT This project was developer to work with SSH1106 - 1.3 0.96 128x64 OLED DISPLAY
 * Pay atention if you use a diferent display !!
 */
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C // Change conform your OLED Display

#if ESP8266
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h> 
#define OLED_RESET     D4
#endif

#if ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#define OLED_RESET     4
#endif

#define LOGO_HEIGHT   64
#define LOGO_WIDTH  128

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Change conform your OLED Display <<<<<<<<<<<<<

const char *ssid = ""; // Change this to your WiFi SSID  <<<<<<<<<<<<<
const char *password = ""; // Change this to your WiFi password <<<<<<<<<<<<<
const String ducoUser = ""; // Change this to your Duino-Coin username <<<<<<<<<<<<<

const String ducoReportJsonUrl = "https://server.duinocoin.com/v2/users/" + ducoUser + "?limit=1";

const int run_in_ms = 60000; // 1 Minute to Start Cicle Again. Is recomended use values upper 5000

float lastAverageHash = 0.0;
float lastTotalHash = 0.0;


#define FRAME_DELAY (42)
#define FRAME_WIDTH (64)
#define FRAME_HEIGHT (64)
#define FRAME_COUNT (sizeof(pigAnimation) / sizeof(pigAnimation[0])) // All animatins as the same size


void DrawPig(void) { // Draw a cash pig animation in display
  int i;
  for (i = 1; i < 28; ++i)
  {
  display.clearDisplay();
  display.drawBitmap(32, 0, pigAnimation[i], FRAME_WIDTH, FRAME_HEIGHT, 1);
  display.display();
  delay(FRAME_DELAY);
  }
}

void DrawEmojiS(void) { // Draw a emoji animation in display
  int i;
  for (i = 1; i < 28; ++i)
  {
  display.clearDisplay();
  display.drawBitmap(32, 0, emojiSusto[i], FRAME_WIDTH, FRAME_HEIGHT, 1);
  display.display();
  delay(FRAME_DELAY);
  }
}


void DrawEmojiPiscadaOculos(void) {  // Draw a emoji animation in display
  int i;
  for (i = 1; i < 28; ++i)
  {
  display.clearDisplay();
  display.drawBitmap(32, 0, emojiPiscadaOculos[i], FRAME_WIDTH, FRAME_HEIGHT, 1);
  display.display();
  delay(FRAME_DELAY);
  }
}

void setup() {
    Serial.begin(115200);
    setupWifi();

    delay(250); // wait for the OLED to power up
    display.begin(SCREEN_ADDRESS, true); // Address 0x3C default
    //display.setContrast (0); // If you need change the constrast set here 
  
    display.display();
    display.clearDisplay();
}

void loop() {
    if (runEvery(run_in_ms)) {
        float totalHashrate = 0.0;
        float avgHashrate = 0.0;
        int total_miner = 0;

        String input = httpGetString(ducoReportJsonUrl);
        DynamicJsonDocument doc (8000);
        DeserializationError error = deserializeJson(doc, input);

        if (error) {
            Serial.print("deserializeJson() failed: "); // As possible duco server deny your IP with you send a lot requests, be careful
            Serial.println(error.c_str());
            return;
        }

        JsonObject result = doc["result"];

        JsonObject result_balance = result["balance"];
        double result_balance_balance = result_balance["balance"];
        const char *result_balance_created = result_balance["created"];
        const char *result_balance_username = result_balance["username"];
        const char *result_balance_verified = result_balance["verified"];

        for (JsonObject result_miner : result["miners"].as<JsonArray>()) {
            float result_miner_hashrate = result_miner["hashrate"];
            totalHashrate = totalHashrate + result_miner_hashrate;
            total_miner++;
        }
        avgHashrate = totalHashrate / long(total_miner);
        long run_span = run_in_ms / 1000;

         /*
        * Loop >>> 
        * 1º - Animation Pig
        * 2º - Show Miners Number
        * 3º - Animation Emoji Blinking
        * 4º - Hash Rate Number
        * 5º - Animation Emoji Scare
        * 6º - Balance Number
        */

        DrawPig(); 

        display.clearDisplay();
        display.setCursor(4, 0);
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(2);
        display.println("Miners:");
        display.setTextSize(3);
        display.println(String(total_miner));
        display.display();
        delay(3000);
        display.clearDisplay();

		    DrawEmojiPiscadaOculos();

        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(2);
        display.println("Hash Rate:");
        display.setTextSize(2);
        display.println(String(totalHashrate) + " kH/s");
        display.display();
        delay(3000);

        DrawEmojiS();

        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(2);
        display.println("Balance:");
        display.setTextSize(3);
        display.println(String(result_balance_balance));
        display.display();
        delay(3000);

    }
}

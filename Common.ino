
/*
 * Don't rate this code! It's a project for fun :)
 */

void setupWifi() {

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
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

String httpGetString(String URL) {

    String payload = "";
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    if (http.begin(client, URL)) {
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            payload = http.getString();
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    return payload;
}

boolean runEvery(unsigned long interval) {
    static unsigned long previousMillis = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        return true;
    }
    return false;
}

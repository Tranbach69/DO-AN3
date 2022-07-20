#include <Arduino.h>

#include "WiFi.h"
#include <Wire.h>
#include "ESPAsyncWebServer.h"
#include <AsyncTCP.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "ThingSpeak.h"
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
const char* ssid = "MI8";
const char* password = "fidelity222";
const char* serverName = "https://bachtran69.herokuapp.com/api/patient/1";
WiFiClient  client;
//thingspeak
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "1EI0XXXXT9JCHTP5";
uint64_t lastTime = 0;
uint64_t timerDelay = 10000;
//end thingspeak
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

uint8_t lcdColumns = 16;
uint8_t lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
uint8_t ID = 0;

// void vtaskLcd(void * parameter);
// void vtaskThingSpeak(void * parameter);
bool sta1 = 0;
bool sta2 = 0;
#define DHTPIN 18
#define DHTTYPE    DHT22
#define LED_BIN 32
#define BUTTON_PIN1 34
#define BUTTON_PIN2 35
DHT dht(DHTPIN, DHTTYPE);

const uint8_t oneWireBus = 5;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

AsyncWebServer server(80);

//read Dht22
String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}
String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}//end read dht22

//read ds18b20
String readBodyTemperature() {
  sensors.requestTemperatures();
  float Body = sensors.getTempCByIndex(0);
  if (isnan(Body)) {
    Serial.println("Failed to read from ds18b20 sensor!");
    return "--";
  }
  else {
    Serial.println(Body);
    return String(Body);
  }
}//end read ds18b20
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .sensor-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP32 Web Server</h2>
  <div>
    <div>
      <p>
        <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
        <span class="sensor-labels">Temperature</span> 
        <span id="temperature">%TEMPERATURE%</span>
        <sup class="units">&deg;C</sup>
      </p>
    </div>
    <div>
      <p>
        <i class="fas fa-tint" style="color:#00add6;"></i> 
        <span class="sensor-labels">Humidity</span>
        <span id="humidity">%HUMIDITY%</span>
        <sup class="units">&percnt;</sup>
      </p>
    </div>
  </div>
  %BUTTONPLACEHOLDER%
</body>
<script >


function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ; 


</script>
</html>)rawliteral";

String outputState(int output) {
  if (digitalRead(output)) {
    return "checked";
  }
  else {
    return "";
  }
}

// Replaces placeholder with sensor values
String processor(const String& var) {
  //Serial.println(var);
  if (var == "TEMPERATURE") {
    return readDHTTemperature();
  }
  else if (var == "HUMIDITY") {
    return readDHTHumidity();
  }
  // else if (var == "BODYTEMPERATURE") {
  //   return readBodyTemperature();
  // }
  else if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    buttons += "<h4>Air-Conditioner Remote Control</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"32\" " + outputState(32) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();

}

void setup() {

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  Serial.begin(115200);
  sensors.begin();
  dht.begin();
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(LED_BIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("xin moi chon ID");

  // xTaskCreate(
  //   vtaskThingSpeak, /* Task function. */
  //   "vTaskThingSpeak", /* name of task. */
  //   2048, /* Stack size of task */
  //   NULL, /* parameter of the task */
  //   4, /* priority of the task */
  //   NULL); /* Task handle to keep track of created task */
  // xTaskCreate(
  //   vtaskLcd, /* Task function. */
  //   "vTaskLcd", /* name of task. */
  //   2048, /* Stack size of task */
  //   NULL, /* parameter of the task */
  //   3, /* priority of the task */
  //   NULL); /* Task handle to keep track of created task */
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  // server.on("/bodytemperature", HTTP_GET, [](AsyncWebServerRequest * request) {
  //   request->send_P(200, "text/plain", readBodyTemperature().c_str());
  // });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("Điều hòa: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop() {
  if (dht.readTemperature() >= 34.00)
  {
    //digitalWrite(34,HIGH);
    digitalWrite(LED_BIN, HIGH); //HIGH is set to about 5V PIN8

  }

    if ((millis() - lastTime) > timerDelay) {
      ThingSpeak.setField(1, dht.readTemperature());
      ThingSpeak.setField(2, dht.readHumidity());
      int x = ThingSpeak.writeFields(myChannelNumber, "1EI0XXXXT9JCHTP5");
      if (x == 200) {
        Serial.println("Channel update successful.");
      }
      else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
      lastTime = millis();
    }
  
    bool button_state1 = digitalRead(BUTTON_PIN1);
    bool button_state2 = digitalRead(BUTTON_PIN2);
    if (button_state1 == HIGH ) {
      delay(20);
      if (button_state1 == HIGH && sta1 == 0) {
        sta1 = 1;
        ID++;
        Serial.println(ID);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ID cua ban la:" + String(ID));
      }
    }
    else {
      sta1 = 0;
    }
    if (button_state2 == HIGH ) {
      delay(20);
      if (button_state2 == HIGH && sta2 == 0) {
        sta2 = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("bat dau do");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("than nhiet :");
        lcd.setCursor(1, 4);
        sensors.requestTemperatures();
        float Body = sensors.getTempCByIndex(0);
        String a=String(Body);
        lcd.print("36.66");
        
        ID=0;

        
          WiFiClient client;
          HTTPClient http;

          http.begin(serverName);

          http.addHeader("Content-Type", "application/json");
          int httpResponseCode = http.PATCH("{\"BodyTemperature\":\" 36.66\"}");    
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
            
          // Free resources
          http.end();
      
    }
    else {
      sta2 = 0;
    }
}
}

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>



#define FIREBASE_HOST "smodified host"
#define FIREBASE_AUTH "modified auth"
#define WIFI_SSID "Daneet"
#define WIFI_PASSWORD "youare23444"

#define DHTPIN 13     // Digital pin (D7) connected to the DHT sensor
#define DHTTYPE DHT11


DHT dht(DHTPIN, DHTTYPE);

int light;
int fan;
int lightPin = 5;
int fanPin = 4;
int pirPin = 12;
bool motionDetected = 0;
bool value;
unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  pinMode(lightPin, OUTPUT); //specifying D1 on the NodeMCU as output pin e.g. light
  pinMode(fanPin, OUTPUT); //specifying D2 as output pin, for a different component e.g. fan  
  pinMode(pirPin, INPUT); //specifying D6 as input pin, for the PIR sensor
 
  
  Serial.begin(9600);
  delay(500);
    // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  dht.begin();
}

void readDHT(){
  float temp = dht.readTemperature();
  Serial.println(temp);  
  Firebase.setFloat("alert/temperature", temp);
}

void loop() {
  light = Firebase.getInt("components/light");
  if (Firebase.failed()){
    Serial.print("light setting failed");
    Serial.println(Firebase.error());
    return;
  }
  
  Serial.println(light);  
  Serial.println(light==1);
  fan = Firebase.getInt("components/fan");
  if (Firebase.failed()){
    Serial.print("fan setting failed");
    Serial.println(Firebase.error());
    return;
  }

  Serial.println(light==1);
  if (light == 1){  
    digitalWrite (5, LOW); 
    } else {
    digitalWrite(5, HIGH);       
    
  }
  if (fan == 1){
    digitalWrite(4, LOW);
  } else {
    digitalWrite(4, HIGH);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
    readDHT();
    previousMillis = currentMillis;
  }

  motionDetected = digitalRead(pirPin);
  if (motionDetected){
    Firebase.setString("alert/pir_alert", "ACTIVE");
  } else{
    Firebase.setString("alert/pir_alert", "INACTIVE");
  }
}



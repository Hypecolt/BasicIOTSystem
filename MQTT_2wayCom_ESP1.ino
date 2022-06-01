/*
 * ESP8266 (Adafruit HUZZAH) Mosquitto MQTT Publish Example
 * Thomas Varnish (https://github.com/tvarnish), (https://www.instructables.com/member/Tango172)
 * Made as part of my MQTT Instructable - "How to use MQTT with the Raspberry Pi and ESP8266"
 */
#include <WiFi.h> // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <ir_Transcold.h>

const int ledPin = 26; // This code uses the built-in led for visual feedback that the button has been pressed
const uint16_t kIrSendPin = 25;

// WiFi
// Make sure to update this for your own WiFi network!
const char* ssid = //your Wi-Fi name
const char* wifi_password = //your Wi-Fi password

// MQTT
// Make sure to update this for your own MQTT Broker!
const char* mqtt_server = //the IP of the device that hosts the MQTT server
const char* mqtt_topic_receive = "ESP1";
const char* mqtt_topic_send = "main";
const char* mqtt_username = "main1";
const char* mqtt_password = "main1";
// The client id identifies the ESP8266 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* clientID = "RoomLights";

long lastMsg = 0;

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(wifiClient); // 1883 is the listener port for the Broker
IRsend irsend(kIrSendPin);
IRTranscoldAc ac(kIrSendPin);

void setup_wifi(){
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, wifi_password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length){
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp = "";

  for(int i = 0; i < length; i++){
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if(String(topic) == mqtt_topic_receive){
    Serial.println("Changing output to ");
     if(messageTemp == "on_LGHT"){
      Serial.println("on_LGHT");
      roomLightsON();
    }
    else if(messageTemp == "off_LGHT"){
      Serial.println("off LIGHT");
      roomLightsOFF();
    }
    else if(messageTemp == "on_AC"){
      Serial.println("on AC");
      AC_ON();
    }
    else if(messageTemp == "off_AC"){
      Serial.println("off AC");
      AC_OFF();
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(mqtt_topic_receive);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void roomLightsON(){
  int i;

  for(i = 0 ; i < 10 ; i++){
    irsend.sendNEC(0xFFB04F);
    delay(10);
  }
}

void roomLightsOFF(){
  int i;

  for(i = 0 ; i < 10 ; i++){
    irsend.sendNEC(0xFFF807);
    delay(10);
  }
}

void AC_ON(){
  int i;

  for(i = 0 ; i < 10 ; i++){
    irsend.sendTranscold(0xEB6554);
    delay(10);
  }
}

void AC_OFF(){
  int i;
  
  for(i = 0 ; i < 10 ; i++){
    irsend.sendTranscold(0xEB7554);
    delay(10);
  }
}

void setup(){
  Serial.begin(115200);

  irsend.begin();
  ac.begin();

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    //client.publish(mqtt_topic_send, "hello");
  }
}

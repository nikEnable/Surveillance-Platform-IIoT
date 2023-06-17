#include <Arduino.h>
#include <WiFiNINA.h>
#include "config.h"
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <stdio.h>
#include <string.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#define DHTPIN A0 // Analog Pin sensor is connected to
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define DHTTYPE DHT11 // type of sensor

DHT dht(DHTPIN, DHTTYPE);

// defines variables
long duration; // variable for the duration of sound wave travel
int Distance; // variable for the distance measurement
 

void connectWiFi()
{
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(SSID);
    // Connect to WPA/WPA2 network:
    int status = WiFi.begin(SSID, PASS);
    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
}

void reconnectMQTTClient()
{
    while (!client.connected())
    {
      Serial.print("Attempting MQTT connection...");

        if (client.connect(ClientName.c_str()))
        {
            Serial.println("connected");     
            client.subscribe(Client_pub_topic.c_str());
        }
        else
        {
            Serial.print("Retying in 5 seconds - failed, rc=");
            Serial.println(client.state());            
            delay(5000);
        }
    }
}

void createMQTTClient()
{
    client.setServer(Broker.c_str(), TCP_port);
    //client.setCallback(clientCallback);
    reconnectMQTTClient();
}

void dht_hum() {
  dht.read();
  float humidity = dht.readHumidity();
  Serial.print("Current humidity = ");
  Serial.print(humidity);
  Serial.println("%");
  delay(5000);
}

void dht_temp() {
  dht.read();
  float temperature = dht.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println("C");
  delay(5000);
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT 
  delay(1000);//Wait before accessing Sensor
  while (!Serial)
  {
    ;
  }
  connectWiFi();
  createMQTTClient();
}

void loop() {
  reconnectMQTTClient();
  client.loop();
  string telemtry = "Hello Worold";
  client.publish(Client_pub_topic.c_str(), telemtry.c_str());
  Serial.println("Hello mqtt");
  delay(2000);
}





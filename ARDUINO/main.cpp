#include <Arduino.h>
#include <WiFiNINA.h>
#include "config.h"
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <stdio.h>
#include <string.h>
#include <Servo.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#define DHTPIN  A0// Analog Pin sensor is connected to
#define echoPin 7 // attach pin D7 Arduino to pin Echo of HC-SR04
#define trigPin 6 //attach pin D6 Arduino to pin Trig of HC-SR04
#define DHTTYPE DHT11 // type of sensor
#define LED_BUILTIN 9 // Digital Pin 

DHT dht(DHTPIN, DHTTYPE);
Servo servo;
int angle = 0;

// defines variables
long duration; // variable for the duration of sound wave travel
int Distance; // variable for the distance measurement
String temperatureString;
String humidityString;

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


void clientCallback(char *topic, uint8_t *payload, unsigned int length)
{
    char buff[length + 1];
    for (int i = 0; i < length; i++)
    {
        buff[i] = (char)payload[i];
    }
    buff[length] = '\0';

    Serial.print("Message received:");
    Serial.println(buff);
    String message = buff;
    Serial.println(message);

    if (message == "1")
    {
      digitalWrite(LED_BUILTIN, HIGH);
      // scan from 145 to 180 degrees with 1 sec/1 degree 
      servo.read(); 
      Serial.println(servo.read());                               
      for(angle = 0; angle < 90; angle+=5)  
      {                                  
        servo.write(angle);                                  
      }
      delay(45000);
      servo.write(45);
      delay(10000);
      for(angle = 45; angle > 0; angle-=9)  
      {                                  
        servo.write(angle);                                  
      }

      digitalWrite(LED_BUILTIN, LOW);
    }
}

void reconnectMQTTClient()
{
    while (!client.connected())
    {
      Serial.print("Attempting MQTT connection...");

        if (client.connect(ClientName.c_str()))
        {
            Serial.println("connected");     
            client.subscribe(Client_sub_topic.c_str());
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
    client.setCallback(clientCallback);
    reconnectMQTTClient();
}

void dht_hum() 
{
  dht.read();
  float humidity = dht.readHumidity();
  humidityString = humidity - 74.80;
   if (isnan(humidity)){
    //randHum = random(50, 70);
    //humidityString = randHum;
    //client.publish(Client_pub_topic_hum.c_str(), humidityString.c_str());
    Serial.println("Hum isnan: ");
    //Serial.println(humidityString);
   } else {
    client.publish(Client_pub_topic_hum.c_str(), humidityString.c_str());
    Serial.print("Current humidity = ");
    Serial.print(humidityString);
    Serial.println("%");
   }
}

void dht_temp() 
{
  dht.read();
  float temperature = dht.readTemperature();
  temperatureString = temperature + 12;
   if (isnan(temperature)){
    //randTemp = random(50, 70);
    //temperatureString = randHum;
    //client.publish(Client_pub_topic_temp.c_str(), temperatureString.c_str());
    Serial.println("Temp isnan: ");
    //Serial.println(humidityString);
   } else {
    client.publish(Client_pub_topic_temp.c_str(), temperatureString.c_str());
    Serial.print("Current Temp = ");
    Serial.print(temperatureString);
    Serial.println("%");
   }
}


void ultra_sonic()
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  Distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(Distance);
  Serial.println("cm ");
  delay(2000);
}

void servo_mg90s()
{
  // scan from 145 to 180 degrees with 1 sec/1 degree                                  
  for(angle = 145; angle < 180; angle++)  
  {                                  
    servo.write(angle);
    delay(1000);                                  
  }
  delay(30000); // Delay 30 sec
  servo.write(0); // rotation to 0 degress
}


void setup() 
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  servo.attach(8);
  servo.write(angle); 
  pinMode(LED_BUILTIN, OUTPUT);
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
  dht_temp();
  dht_hum();
  ultra_sonic();
  if (Distance < 10)
  {
    String value = "1";
    client.publish(Client_pub_topic_flame.c_str(), value.c_str());
    Serial.println("Hello");
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    servo_mg90s();
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (LOW is the voltage level)
  }
  delay(10000);
  //string telemtry = "Hello Worold";
  //client.publish(Client_pub_topic.c_str(), telemtry.c_str());
  //Serial.println("Hello mqtt");
}

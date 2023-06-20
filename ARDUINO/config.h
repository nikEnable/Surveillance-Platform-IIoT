#pragma once
#include<string>
using namespace std;

// WiFi Credentials
char SSID[] = "WIND_2.4G_BD23DC"; 
char PASS[] = "gKpfzqHtCH";  

// MQTT settings
const string ID = "nikEnable";
const string Broker = "test.mosquitto.org";
const string ClientName = ID + "IIoT_client";
const int TCP_port = 1883;
const string Client_pub_topic_flame =  "iiotnode/flamedetection";
const string Client_pub_topic_temp =  "iiotnode/DHT11/temp";
const string Client_pub_topic_hum =  "iiotnode/DHT11/hum";
const string Client_sub_topic =  "iiotnode/condition";


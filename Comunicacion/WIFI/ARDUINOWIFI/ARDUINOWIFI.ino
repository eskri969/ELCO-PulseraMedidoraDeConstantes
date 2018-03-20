int val=0;
#include <WiFi.h>
#include <SPI.h>
const char* ssid = "Wifi Eurielec";
const char* password =  "c0ck3t3and0";
WiFiServer wifiServer(80);
boolean connected=false;
//ESP32 ip 10.13.16.150

IPAddress ip(10,13,16,150);            // IP address of the server
IPAddress gateway(110,13,16,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

/*
void setup() 
{
  //initialize serial communications at a 9600 baud rate
  Serial.begin(9600);
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());
 
}

void loop() {
 
  WiFiClient client = wifi.available();
 
  if (client) {
 
    while (client.connected()) {
 
      while (client.available()>0) {
        client.write("hello world");
        Serial.println("Escribiendo: hello world");
      }
 
      delay(10);
    }
 
    client.stop();
    Serial.println("Client disconnected");
 
  }
}*/


int status = WL_IDLE_STATUS;
IPAddress server(10,13,16,120);  // Google

// Initialize the client library
WiFiClient client;

void setup() {
  //initialize serial communications at a 9600 baud rate
  Serial.begin(9600);
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());
    // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    connected=true;
    Serial.println("connected");
    client.println("CONSEGUIDO");
    client.println();
  }
}

void loop() {  
  if(connected){
    Serial.println("intento");
    String welcome="I just connected to you my IP is";
    welcome+=WiFi.localIP();
    //String var="2";
    //client.write(var);
    client.write("nada mas");
  }
  /*
  char var= client.read();
  if(var!=0){
      Serial.println(var);
  }*/

  delay(300);
}


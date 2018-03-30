int val=0;
#include <WiFi.h>
#include <SPI.h>

const char* ssid = "Tenda_205850";
const char* password =  "54000209";
boolean connected=false;
int sensor=15;

int status = WL_IDLE_STATUS;
IPAddress server(192,168,0,103);  // Server IP

// Initialize the client library
WiFiClient client;

IPAddress ip(192,168,0,150);            // IP address of the sESP32
IPAddress gateway(192,168,0,103);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

String input;
int ack=0;
String pulso;
String oxigeno;
String temperatura;
String datos;
String tiempo;

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
  if (client.connect(server, 3000)) {
    connected=true;
    Serial.println("connected");
  }
  else{
    Serial.println("not able to reach server");
  }
}

void loop() {  
  if(connected){
     Serial.println();
     if(client.available()) {
       Serial.println("<< hay mensaje que leer>>");
       input = client.readStringUntil('\n');
       if(!input || input==""){
          Serial.println("<<mensaje vacio>>");
       }
       else{
         Serial.print("recibo -> ");
         Serial.println(input);      
       }
     }
     else{
       Serial.println("<<no hay mensaje que leer>>");
     }
     if(input=="peticion"){
      pulso="get pulso";
      oxigeno="get pulso";
      temperatura="get temperatura";
      tiempo=millis();
      datos=",,,"+tiempo+",,,"+pulso+",,,"+oxigeno+",,,"+temperatura+",,,";

      client.println(datos);
      Serial.print("escribo -> ");
      Serial.println(datos);  
      while(ack<6){
        if(client.available()){
          input = client.readStringUntil('\n');
          if(input=="ACK"){
             ack=6;
             Serial.print("recibo -> ");
             Serial.println(input);  
          }
        }
        ack++;
        if(ack==5){
          client.println(datos);
          Serial.print("escribo -> ");
          Serial.println(datos);
          ack=0;
        }
        delay(1000);
      }
      ack=0;
     }
  }
  else{
    Serial.println("no connection :(");
  }

  delay(5000);
}


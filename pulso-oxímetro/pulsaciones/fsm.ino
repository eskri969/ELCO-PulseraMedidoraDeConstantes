#include <WiFi.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"


enum states { IDLE_ST , SEND_ST, MEASURE_POX, PROCESS_POX, MEASURE_TEMP };


const char* ssid = "";
const char* password =  "";
boolean connected=false;

int status = WL_IDLE_STATUS;
IPAddress server(192,168,1,20);  // Server IP

// Initialize the client library
WiFiClient client;

IPAddress ip(192,168,1,150);            // IP address of the sESP32
IPAddress gateway(192,168,1,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

MAX30105 pox;
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte hr_cont, spo2_cont;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;



#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif
int32_t bufferLength = 100; //data length

int pox_samples;
int current_state;


int HRAvg, spo2_avg, HR_final, spo2_final;
int hr2;



void setup_WIFI(){
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
   }else{
      Serial.println("not able to reach server");
   }
}

void setup_pox(){
    // Initialize sensor
    if (!pox.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
      Serial.println("MAX30105 was not found. Please check wiring/power. ");
      while (1);
    }

    pox_samples = 0;
    
    byte ledBrightness = 0x2F; //Options: 0=Off to 255=50mA
    byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    byte sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 411; //Options: 69, 118, 215, 411
    int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  
    pox.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings


     for (byte i = 0; i < 100; i++)
    {
      redBuffer[i] = bufferLength;
      irBuffer[i] = bufferLength;
    }

    HRAvg = 1;
    spo2_avg = 1;
    HR_final = spo2_final = 0;
    hr_cont = spo2_cont = 0;

}

void setup() {
  Serial.begin(9600);
  setup_WIFI();
  setup_pox();
  current_state =IDLE_ST;
}



#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
void shift_pox_array(uint16_t *pox_arr, uint16_t new_sample)
#else
void shift_pox_array(uint32_t *pox_arr, uint32_t new_sample)
#endif
{
   byte i;
   for (i = 0; i < 100; i++) {
      *(pox_arr + i ) = *(pox_arr + i + 1);
   }
   *(pox_arr + i) = new_sample;

}


int fsm(int current_state){
  int next_state;
  switch(current_state){
      case IDLE_ST:
             
          if(connected && client.available() && client.readStringUntil('\n') == "peticion" ){
            String data=",,,1,,,"+ String(HR_final)+",,,"+ String(spo2_final)+",,,36.5,,,";
              client.println(data);
            //  Serial.print("escribo -> ");
            //  Serial.println(data);  
             next_state = SEND_ST;
          }else if(pox.available()){
             next_state = MEASURE_POX;
          }else{
            next_state = current_state;
          }
          
          break;
      case SEND_ST:

         /* if(connected && client.available() && client.readStringUntil('\n') == "peticion" ){
              String data=",,,1,,,2,,,3,,,4,,,";
              client.println(data);
              Serial.print("escribo -> ");
              Serial.println(data);  
          }*/
          
          if(pox.available()){
             next_state = MEASURE_POX;
          }else{
            next_state = current_state;
          }
          break;
      case MEASURE_POX:
          if(pox.available()){
              shift_pox_array(irBuffer, pox.getIR());
              shift_pox_array(redBuffer,  pox.getRed());
              pox.nextSample(); 
              pox_samples++;
           
              
             if (checkForBeat(irBuffer[100]) == true)
             {
                Serial.println("FGDGHTHGT");
                //We sensed a beat!
                long delta = millis() - lastBeat;
                lastBeat = millis();

                beatsPerMinute = 60 / (delta / 1000.0);
                maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

                 Serial.println(beatsPerMinute);
                if (beatsPerMinute < 200 && beatsPerMinute > 30)
                {
                  hr_cont++;
                  HRAvg = HRAvg + beatsPerMinute;
                  
                  if(hr_cont >= RATE_SIZE){ 
                    HR_final = HRAvg/RATE_SIZE;
                    Serial.print(", HR: ");
                    Serial.println(HR_final);
                    Serial.print("SPO2: ");
                    Serial.println(spo2_final);
                    HRAvg = 0;
                    hr_cont = 0;
                  }  
                }

                if(spo2 > 90 && spo2 <100){
                   spo2_cont = spo2_cont + 1;
                   spo2_avg =spo2_avg + spo2;
                   if(spo2_cont >= RATE_SIZE){
                        spo2_final = spo2_avg/RATE_SIZE;
                        
                        Serial.print(", HR: ");
                        Serial.println(HR_final);
                        Serial.print("SPO2: ");
                        Serial.println(spo2_final);
                        spo2_avg = 0;
                        spo2_cont = 0;
                   }
                }

                
              }        
             //Serial.print(", SPO2: ");
             // Serial.println( 100.0*irBuffer[100]/(irBuffer[100]+redBuffer[100]) );
          }
           next_state = MEASURE_TEMP;
           /*
          if(pox_samples > 25){
             Serial.println("HOLA ");
             next_state = PROCESS_POX;
          }else{
             next_state = MEASURE_TEMP;
          }*/
          break;
      case PROCESS_POX:
            //maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
            pox_samples = 0;
           /* Serial.print("HR: ");
            Serial.println(heartRate);
            Serial.print("SPO2: ");
            Serial.println(spo2);*/
            break;
      case MEASURE_TEMP:

          next_state = IDLE_ST;
      
          break;

   }

   return next_state;
  
}



void print_info(){
   
   Serial.print("Estado: ");
   Serial.print(current_state);
   Serial.print(" ,WIFI:");
   Serial.println(connected);
   Serial.println("--------------------------------------");

}



void loop() {
  while(1){
    // Serial.println("--------------------------------------");
     pox.check();
     current_state = fsm(current_state);
    // print_info();
     //print_arr();
     delay(1);
  }
 
}





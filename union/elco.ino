#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);


#define D8 15
#define D3 0
#define DELAY 20
#define TEMP_MUESTRAS 50

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

#define LOGO_FULL_GLCD_HEIGHT 32
#define LOGO_FULL_GLCD_WIDTH  128


byte hr_cont, spo2_cont;

static const unsigned char PROGMEM logo128_logo[] =
{
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000011, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00011111, B11111111, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000100, B01111111, B11111000, B00000000, B00000000, B00000000, B00000000, B00100011, B11111111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000101, B11111111, B11111000, B01111100, B00111111, B11111000, B00000000, B00101111, B11111111, B10000011, B11111111, B10000011, B11111111, B10000011, B11111111, B11000000,
  B00000101, B11111100, B01111000, B10001110, B01000111, B11111100, B00000000, B00101111, B11000000, B00000100, B01111111, B11000100, B01111111, B11000100, B01111111, B11100000,
  B00000111, B11111100, B01111000, B10111110, B01011111, B11111110, B00000000, B00111111, B11000000, B00000101, B11111111, B11100101, B11111111, B11100101, B11111111, B11000000,
  B00000111, B11111100, B01111000, B10111110, B01011111, B10001110, B00000000, B00111111, B11000000, B00000101, B11111000, B11100101, B11110001, B11100101, B11110000, B00000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111000, B11100111, B11110001, B11100111, B11110000, B00000000,
  B00000111, B11111111, B11110000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111111, B11100111, B11111111, B11100111, B11111111, B11000000,
  B00000111, B11111100, B01111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111111, B11100111, B11111111, B11000111, B11111111, B11100000,
  B00000111, B11111100, B01111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111111, B11100111, B11111111, B10000111, B11111111, B11000000,
  B00000111, B11111100, B01111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111000, B11100111, B11111111, B11000111, B11110000, B00000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B11111110, B00000000, B00111111, B11111111, B10000111, B11111000, B11100111, B11110111, B11100111, B11110000, B00000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B11111110, B00000000, B00111111, B11111111, B11000111, B11111000, B11100111, B11110001, B11100111, B11111111, B11000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B11111110, B00000000, B00111111, B11111111, B11000111, B11111000, B11100111, B11110001, B11100111, B11111111, B11100000,
  B00000111, B11111111, B11110000, B11111110, B00111111, B11111100, B00100000, B00011111, B11111111, B11000111, B11111000, B11100111, B11110001, B11100111, B11111111, B11100000,
  B00000011, B11111111, B11100000, B01111100, B00011111, B11111000, B00100000, B00001111, B11111111, B10000011, B11100000, B01000011, B11100000, B01000011, B11111111, B11000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B01110000, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B01010001, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11011001, B01000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B10001001, B01100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000111, B11111111, B11111111, B11111111, B11111111, B11111111, B10001011, B00111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11100000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00001110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000100, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
};



static const unsigned char PROGMEM logo16_pulse_0[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00001110, B01110000,
  B00001111, B11110000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_pulse_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00011100, B00111000,
  B00111110, B01111100,
  B00111111, B11111100,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_pulse_2[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B01111100, B00111110,
  B11111110, B01111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B01111111, B11111110,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_temp_1[] =
{
  B00000000, B00000000,
  B00000011, B11000000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_temp_2[] =
{
  B00000000, B00000000,
  B00000011, B11000000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_o2_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000001, B10000000,
  B00000011, B11000000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00111111, B11111100,
  B01111111, B11111110,
  B01111111, B11111110,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_o2_2[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00001110, B00000000,
  B00110001, B10000000,
  B01100000, B11000000,
  B01100000, B11001100,
  B01100000, B11010010,
  B01100000, B11000100,
  B00110001, B10001000,
  B00001110, B00011110,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_conect_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00001111,
  B00000000, B00001111,
  B00000000, B00001111,
  B00000011, B11001111,
  B00000011, B11001111,
  B00000011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_conect_2[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000001, B00000100,
  B00000011, B10001110,
  B00000111, B11011111,
  B00000011, B11111110,
  B00000001, B11111100,
  B00000000, B11111000,
  B00000001, B11111100,
  B00000011, B11111110,
  B00000111, B11011111,
  B00000011, B10001110,
  B00000001, B00000100,
  B00000000, B00000000,
};


/* POX GLOBAL VARIABLES */


MAX30105 pox;
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.

long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;

uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
int32_t bufferLength = 100; //data length

int pox_samples;

int HRAvg, spo2_avg, HR_final, spo2_final;
int hr2;


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float temp[TEMP_MUESTRAS];
int pulseResult = 0;
float tempResult = 0;
int spo2Result = 0;

bool animacion = true;
bool supermenu = true;


// WIFI VARIABLES

WiFiClient client;
 char* ssid;      // SSID
 char* password;      // Password
 char* host;  // IP serveur - Server IP
 int port;

void setup_wifi(){
    ssid = "EA4RCT";      // SSID
    password = "laradiomola";      // Password
    host = "192.168.1.105";  // IP serveur - Server IP
    port = 3000;  
  
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
    byte sampleAverage = 2; //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    byte sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 411; //Options: 69, 118, 215, 411
    int adcRange = 4096; //Options: 2048, 4096, 8192, 163840
  
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


void setup_display(){
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Clear the buffer.
  display.clearDisplay();
  display.drawBitmap(0, 0,  logo128_logo, 128, 32, WHITE);
  display.display();
  delay(2000);
}

int menu_number = 0;

int current_state = 0;

int inside_menu = 0;

bool connect_wifi(){
  WiFi.begin(ssid, password);
  delay(5000);
  return (WiFi.status() == WL_CONNECTED);  
}


bool connect_server(){
  client.connect(host, port);
  delay(5000);
  return(client.available());  
}


void setup() {
  // BUTTON INTERRUPTIONS
  pinMode(D8, INPUT_PULLUP);
  //digitalWrite(D8, HIGH);
  attachInterrupt(digitalPinToInterrupt(D3), menu_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(D8), select_button, RISING);
  Serial.begin(9600);
  setup_wifi();
  setup_display();
  setup_pox();
  mlx.begin();//set up temp sensor
}


void menu_button()
{
  if (inside_menu == 0) {
    //Serial.print("\nBoton de menu presionado");
    if (supermenu) {
      if (menu_number == 0) {
        Serial.print("\nEntra de 0 a 1");
        menu_number = 1;
        current_state = 0;
      }
      else {
        Serial.print("\nEntra de 1 a 0");
        menu_number = 0;
        current_state = 0;
      }
    }
    else {
      menu_number = menu_number + 1;
      current_state = 0;
    }
  }
}

void select_button()
{

  Serial.print("\nBoton de seleccion presionado");
  if((menu_number == 0) && (current_state == 1) && (inside_menu == 1)){
    inside_menu = 0;
  }
  else{
    inside_menu = 1;
  }
}

void setDisplayMenu() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
}

void setDisplayMeasure() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

void setDisplayResult() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
}


void midePulso() {
 bool end_measure = false;
 int c_anim = 0;
 while(!end_measure){
    c_anim++;
    pox.check();
    if(pox.available()){
          shift_pox_array(irBuffer, pox.getIR());
          shift_pox_array(redBuffer,  pox.getRed());
          pox.nextSample(); 
          pox_samples++; 

          if (checkForBeat(irBuffer[100]) == true){
             //We sensed a beat!
              long delta = millis() - lastBeat;
              lastBeat = millis();

              beatsPerMinute = 60 / (delta / 1000.0);
               Serial.print(", fuera: ");
              Serial.println(beatsPerMinute);
             if (beatsPerMinute < 200 && beatsPerMinute > 30)
              {
                hr_cont++;
                HRAvg = HRAvg + beatsPerMinute;
                
                if(hr_cont >= RATE_SIZE){ 
                  pulseResult = HRAvg/RATE_SIZE;
                  Serial.print(", HR: ");
                  Serial.println(pulseResult);
                  HRAvg = 0;
                  hr_cont = 0;
                  end_measure = true;
                  reset_pox_buffer(irBuffer);
                  reset_pox_buffer(redBuffer);
                }  
              }
          }

    }
    if (((c_anim % 50) == 0) and (animacion == true)) {
      setDisplayMeasure();
      display.println("Midiendo\npulso . .");
      display.drawBitmap(110, 15,  logo16_pulse_1, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      //delay(500);
      animacion = !animacion;
    }
    else if (((c_anim % 50) == 0) and (animacion == false)) {
      setDisplayMeasure();
      display.println("Midiendo\npulso . . .");
      display.drawBitmap(110, 15,  logo16_pulse_2, 16, 16, WHITE);
      display.display();
      //delay(500);
      display.clearDisplay();
      animacion = !animacion;
    }
    delay(1);
  }
  //animacion = true;
  setDisplayResult();
  //spo2Result = 92;
  display.print(pulseResult);
  display.setTextSize(1);
  display.println("lpm");
  display.drawBitmap(110, 15,  logo16_pulse_2, 16, 16, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
}







void mideTemp() {
  tempResult = 0;
  for (int i = 0; i <= (TEMP_MUESTRAS - 1); i++) {

    if (((i % 5) == 0) and (animacion == true)) {
      setDisplayMeasure();
      display.println("Midiendo\ntemperatura . .");
      display.drawBitmap(110, 15,  logo16_temp_1, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      animacion = !animacion;
    }
    else if ((((i % 5) == 0) or (i == (TEMP_MUESTRAS - 1)) ) and (animacion == false)) {
      setDisplayMeasure();
      display.println("Midiendo\ntemperatura . . .");
      display.drawBitmap(110, 15,  logo16_temp_2, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      animacion = !animacion;
    }

    temp[i] = mlx.readObjectTempC();
    //Serial.print("Temp Array = "); Serial.println(temp[i]);

    tempResult = tempResult + temp[i];

    if (i == (TEMP_MUESTRAS - 1)) {
      tempResult = (tempResult / TEMP_MUESTRAS);
      //Serial.print("Temp Result = "); Serial.println(tempResult);
      //Serial.println();
    }

    delay(100);
  }
  animacion = true;

  setDisplayResult();
  display.print(tempResult);
  display.print((char)247); //degree symbol
  display.println("C");
  display.drawBitmap(110, 15,  logo16_temp_2, 16, 16, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

}



void mideSPO2() {
 int c_anim = 0;
 bool end_measure = false;
 while(!end_measure){
    c_anim++;
    pox.check();
    if(pox.available()){
          shift_pox_array(irBuffer, pox.getIR());
          shift_pox_array(redBuffer,  pox.getRed());
          pox.nextSample(); 
          pox_samples++; 
          maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
          Serial.print(", fuera: ");
          Serial.print(spo2);
          Serial.print(", : ");
           Serial.println(spo2_cont);
         if(spo2 > 90 && spo2 <100){
             Serial.print(", fuera: ");
             Serial.print(spo2);
             Serial.print(", : ");
             Serial.println(spo2_cont);
             spo2_cont = spo2_cont + 1;
             spo2_avg =spo2_avg + spo2;
             if(spo2_cont >= RATE_SIZE){
                  spo2Result = spo2_avg/RATE_SIZE;        
                  Serial.print(", HR: ");
                  Serial.println(HR_final);
                  Serial.print("SPO2: ");
                  Serial.println(spo2Result);
                  spo2_avg = 0;
                  spo2_cont = 0;
                  pox_samples = 0;
                  end_measure = true;
                  reset_pox_buffer(irBuffer);
                  reset_pox_buffer(redBuffer);
             }
          }
    }
    if (((c_anim % 50) == 0) and (animacion == true)) {
      setDisplayMeasure();
      display.println("Midiendo oxigeno\nen sangre . .");
      display.drawBitmap(110, 15,  logo16_o2_1, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      //delay(500);
      animacion = !animacion;
    }
    else if (((c_anim % 50) == 0) and (animacion == false)) {
      setDisplayMeasure();
      display.println("Midiendo oxigeno\nen sangre . . .");
      display.drawBitmap(110, 15,  logo16_o2_2, 16, 16, WHITE);
      display.display();
      //delay(500);
      display.clearDisplay();
      animacion = !animacion;
    }
    delay(1);
  }
  animacion = true;
  setDisplayResult();
  //spo2Result = 92;
  display.print(spo2Result);
  display.setTextSize(1);
  display.println("%");
  display.drawBitmap(110, 15,  logo16_o2_2, 16, 16, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
}


void showResults(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Pulso: ");
  display.print(pulseResult);
  display.println(" lpm");
  display.print("Temperatura: ");
  display.print(tempResult);
  display.print((char)247); //degree symbol
  display.println("C");
  display.print("O2 en sangre: ");
  display.print(spo2Result);
  display.println("%");
  display.display();
  delay(5000);
  display.clearDisplay();
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


void reset_pox_buffer(uint32_t *pox_arr)
{
   byte i;
   for (i = 0; i <= 100; i++) {
      *(pox_arr + i ) = 1;
   }
}



void loop() {

  if (menu_number == 0) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        supermenu = false;
          //while(inside_menu){
        if(WiFi.status() != WL_CONNECTED ){
            Serial.println("Entra en WIFI");
            connect_wifi();
            Serial.println("WIFI connection");
        }
        if (!client.available()){
            Serial.println("Entra en Server");
            connect_server();
              Serial.println("Server connection");
        }
        if(WiFi.status() == WL_CONNECTED &&  client.available()){
            Serial.println("connected");
        
          midePulso();
          mideTemp();
          mideSPO2();
          client.print(String(pulseResult) + ":" + String(tempResult) + ":" + String(spo2Result) + "\r\n");
          showResults();
          
          current_state = 0;
          supermenu = true;
          //inside_menu = 0;
        }
       }
      
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Auto");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 1) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        menu_number = menu_number + 1;
        current_state = 0;
        supermenu = false;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Manual");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 2) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        midePulso();
        current_state = 0;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Pulso");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 3) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        mideTemp();
        current_state = 0;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Temperat.");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 4) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        mideSPO2();
        current_state = 0;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("SPO2");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 5) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        menu_number = 1;
        current_state = 0;
        supermenu = true;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Exit");
      //display.drawBitmap(110, 0,  logo16_conect_2, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
    }
  }


  else if (menu_number > 5) {
    menu_number = 2;
  }
  else {
    setDisplayMenu();
    display.println("Raro raro");
    display.display();
    delay(200);
    display.clearDisplay();
  }
}







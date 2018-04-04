#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"

MAX30105 pox;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

int plot_HR = 0; //CAMBIAR PARA VER PULSOS O OBTENER VALOR DE BPM


void setup()
{
  Serial.begin(115200);

  // Initialize sensor
  if (!pox.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  
  byte ledBrightness = 0x2F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  pox.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

  if(plot_HR){
     const byte avgAmount = 64;
    long baseValue = 0;
    for (byte x = 0 ; x < avgAmount ; x++)
    {
      baseValue += pox.getIR(); //Read the IR value
    }
    baseValue /= avgAmount;
  
    //Pre-populate the plotter so that the Y scale is close to IR values
    for (int x = 0 ; x < 500 ; x++)
      Serial.println(baseValue);
  }
}

void loop()
{
  long irValue = pox.getIR();
  

  if(!plot_HR){
    if (checkForBeat(irValue) == true)
    {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();
    
        beatsPerMinute = 60 / (delta / 1000.0);
    
        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
          rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
          rateSpot %= RATE_SIZE; //Wrap variable
    
          //Take average of readings
          beatAvg = 0;
          for (byte x = 0 ; x < RATE_SIZE ; x++)
            beatAvg += rates[x];
          beatAvg /= RATE_SIZE;
        }
      }
      
      Serial.print(", BPM=");
      Serial.print(beatAvg);
    
      if (irValue < 50000) Serial.print(" No finger?");
  }else{
     Serial.print(irValue);
    
  }
  Serial.println();
}

#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

#include "heartRate.h"

MAX30105 pox;

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif
int32_t bufferLength; //data length


int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid



const byte RATE_SIZE = 16; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void setup()
{
  Serial.begin(115200); // initialize serial communication at 115200 bits per second:


  // Initialize sensor
  if (!pox.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }


  byte ledBrightness = 0x2A; //Options: 0=Off to 255=50mA
  byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  pox.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

    
}


int samples_per_second = 12;

void loop()
{
  bufferLength = 50; //buffer length of 100 stores 4 seconds of samples running at 25sps
  
  //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (pox.available() == false) //do we have new data?
      pox.check(); //Check the sensor for new data

    redBuffer[i] = pox.getRed();
    irBuffer[i] = pox.getIR();
    pox.nextSample(); //We're finished with this sample so move to next sample
    
  }

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = samples_per_second; i < bufferLength; i++)
    {
      redBuffer[i - samples_per_second] = redBuffer[i];
      irBuffer[i - samples_per_second] = irBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = bufferLength - samples_per_second; i < bufferLength; i++)
    {
      while (pox.available() == false) //do we have new data?
        pox.check(); //Check the sensor for new data

      redBuffer[i] = pox.getRed();
      irBuffer[i] = pox.getIR();
      
    
     if (checkForBeat(pox.getIR()) == true)
     {
      Serial.println("holi");
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
      
       pox.nextSample(); //We're finished with this sample so move to next sample
      
      Serial.print(F(", HR="));
      Serial.print(heartRate, DEC);
  
      Serial.print(F(", SPO2="));
      Serial.println(spo2, DEC);
      

    /*
      //send samples and calculation result to terminal program through UART
      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      Serial.print(F(", ir="));
      Serial.print(irBuffer[i], DEC);

     
      Serial.print(F(", HRvalid="));
      Serial.print(validHeartRate, DEC);

      Serial.print(F(", SPO2="));
      Serial.print(spo2, DEC);

      Serial.print(F(", SPO2Valid="));
      Serial.println(validSPO2, DEC);
      */
    }

    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  }
}

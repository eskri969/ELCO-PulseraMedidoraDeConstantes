#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "heartRate.h"



// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only only one Task is accessing this resource at any time.
SemaphoreHandle_t xSerialSemaphore;

// define two Tasks for DigitalRead & AnalogRead
void wifi_controller( void *pvParameters );
void pox_controller( void *pvParameters );





MAX30105 pox;

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[50]; //infrared LED sensor data
uint16_t redBuffer[50];  //red LED sensor data
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








// the setup function runs once when you press reset or power the board
void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
  // because it is sharing a resource, such as the Serial port.
  // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
  if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }
/*
  // Now set up two Tasks to run independently.
 xTaskCreate(
    wifi_controller
    ,  (const portCHAR *)"DigitalRead"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 1 being the highest, and 4 being the lowest.
    ,  NULL );

  xTaskCreate(
    pox_controller
    ,  (const portCHAR *) "pox_controller"
    ,  128  // Stack size
    ,  NULL
    ,  3  // Priority
    ,  NULL );
*/
  xTaskCreate(
    read_pox
    ,  (const portCHAR *) "pox_controller"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // Now the Task scheduler, which takes over control of scheduling individual Tasks, is automatically started.



  // Initialize sensor
  if (!pox.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }


  byte ledBrightness = 0x2A; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 50; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  pox.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

}


/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/



void read_pox( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

  //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
   /* if (pox.available()>0)//do we have new data?
    {
      pox.check(); //Check the sensor for new data

      redBuffer[i] = pox.getRed();
      irBuffer[i] = pox.getIR();
      pox.nextSample(); //We're finished with this sample so move to next sample
      
    }else{*/
      redBuffer[i] = 0;
      irBuffer[i] = 0;
   // }
    
     Serial.println( irBuffer[i] );
  }

  unsigned long time1 = millis();
  for (;;) // A Task shall never return or exit.
  {
    pox.check(); //Check the sensor for new data
    if (pox.available()>0)//do we have new data?
    {
       
  
       //shift the array
       for (byte i = 1; i < bufferLength; i++)
       {
          redBuffer[i - 1] = redBuffer[i];
          irBuffer[i - 1] = irBuffer[i];
       }

       redBuffer[bufferLength] = pox.getRed();
       irBuffer[bufferLength] = pox.getIR();


       Serial.println(pox.getRed());

      pox.nextSample(); //We're finished with this sample so move to next sample
      maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    } 
    //Serial.println(millis() - time1);
    time1 =  millis();
    
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
  }
  
  
}


void wifi_controller( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  /*
    DigitalReadSerial
    Reads a digital input on pin 2, prints the result to the serial monitor

    This example code is in the public domain.
  */

  // digital pin 2 has a pushbutton attached to it. Give it a name:
  uint8_t pushButton = 2;

  // make the pushbutton's pin an input:
  

  for (;;) // A Task shall never return or exit.
  {


    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
  /*  if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
      // We were able to obtain or "Take" the semaphore and can now access the shared resource.
      // We want to have the Serial Port for us alone, as it takes some time to print,
      // so we don't want it getting stolen during the middle of a conversion.
      // print out the state of the button:
      //Serial.println("TAREA 1 AL CONTROL....");

      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }*/

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void pox_controller( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

  bufferLength =50; //buffer length of 100 stores 4 seconds of samples running at 25sps

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
  //maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    Serial.println("ghyhjhyj");
   
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 12; i < bufferLength; i++)
    {
      redBuffer[i - 12] = redBuffer[i];
      irBuffer[i - 12] = irBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 35; i < bufferLength; i++)
    {
      while (pox.available() == false) //do we have new data?
        pox.check(); //Check the sensor for new data

      redBuffer[i] = pox.getRed();
      irBuffer[i] = pox.getIR();
      
    
     if (checkForBeat(irBuffer[i]) == true)
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
      
       pox.nextSample(); //We're finished with this sample so move to next sample
      
      Serial.print(F(", HR="));
      Serial.print(beatAvg, DEC);
  
      Serial.print(F(", SPO2="));
      Serial.println(spo2, DEC);
     

    }
   // maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability

  }
    
}




void loop()
{
  // Empty. Things are done in Tasks.
}


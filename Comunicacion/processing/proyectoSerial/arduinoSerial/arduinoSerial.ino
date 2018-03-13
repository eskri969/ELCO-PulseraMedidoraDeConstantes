//#include <SoftwareSerial.h>
int incoming;
//SoftwareSerial mySerial(10, 11)

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);      // initialize serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( Serial.available() > 0) 
    {  // If data is available,
    //val = myPort.readStringUntil('\n');      // read it and store it in val
    //println(val); //print it out in the console
      incoming=Serial.read();
    } 
  //Serial.print("I received: ");
  //Serial.println(incoming, DEC);
  
  delay(1000);
}  

int val;
Serial myPort;  // Create object from Serial class

void setup() 
{
  //initialize serial communications at a 9600 baud rate
  Serial.begin(9600);
  delay(1000); // give me time to bring up serial monitor
  printf("\n ESP32 Touch Test\n");
}


void loop()
{
  //send 'Hello, world!' over the serial port
  //Serial.println("Hello, world!");
  //wait 100 milliseconds so we don't drive ourselves crazy
   if ( myPort.available() > 0) 
  {  // If data is available,
    val=myPort.read;         // read it and store it in val
    pintln("in");
  } 
  pintln(val);
  Serial.println(touchRead(4)+'\n');  // get value using T0
  delay(200);

  
}


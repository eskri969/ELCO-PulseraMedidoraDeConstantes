import processing.serial.*;
Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port
String myString = "Please press x!";
int pidedatos =0;

void setup()
{
  // I know that the first port in the serial list on my mac
  // is Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[4]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);
  println(myString);
}

void draw()
{
    if ( myPort.available() > 0) 
    {  // If data is available,
    val = myPort.readStringUntil('\n');      // read it and store it in val
    //println(val); //print it out in the console
    } 
 // printArray(Serial.list());
 if(pidedatos == 1){
    //myString = "Thank you!";
    println(val+'\n'+myString+'\n'); //print it out in the console
    pidedatos=0;  
 }
  delay(100);
}

void keyPressed(){
  if(key == 'x'){
    pidedatos=1;
  }
  else{
    //myString = "I said X!";
  }
}
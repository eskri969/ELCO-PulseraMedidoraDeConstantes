/*  
import processing.net.*; 

Client myClient; 
int dataIn; 
 
void setup() { 
  size(200, 200); 
  // Connect to the local machine at port 80.
  // This example will not run if you haven't
  // previously started a server on this port.
  myClient = new Client(this, "10.13.16.1", 80); 
} 

void draw() { 
  if (myClient.available() > 0) { 
    dataIn = myClient.read(); 
  } 
  background(dataIn); 
} 
*/

import processing.net.*;
Server myServer;
int val = 0;
Client myClient; 

void setup() {
  size(200, 200);
  // Starts a myServer on port 80
  myServer = new Server(this, 80);
  myClient = new Client(this, "10.13.16.1", 80); 
}

void draw() {
  val=myClient.readChar();
  println(val);
  //val = (val + 1) % 255;
  //background(val);
  //myServer.write(val);
}
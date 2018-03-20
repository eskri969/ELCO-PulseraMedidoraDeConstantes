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
Client c;
int val = 0;


void setup() {
  size(200, 200);
  // Starts a myServer on port 80
  myServer = new Server(this, 80);
}

void draw() {
  c=myServer.available();
  if (c != null) {
    String input = c.readString();
    println(input);
  }
  myServer.write("hellooo");
  /*
  if(val==0)
   myServer.write("hellooo");
  */
}
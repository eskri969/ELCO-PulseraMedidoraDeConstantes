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
import java.io.FileWriter;
import java.io.*;
Client c;
int val = 0;
String output;
String input;
int tout=5000;
BufferedWriter bw;
//PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter("outfilename", true))); // true = append

void setup() {
  size(200, 200);
  // Starts a myServer on port 80
  myServer = new Server(this, 3000);
  
  try{
    //chemin = dataPath;
// positions.txt== your file;
          File file =new File("/home/eskri96/Documentos/clase/Elco/proyectogit/Comunicacion/WIFI/PROCESSINGWIFI/fichero.txt"); 
          if(!file.exists()){
            file.createNewFile();
          }
 
          FileWriter fw = new FileWriter(file,true);//a true hace append
          BufferedWriter bw = new BufferedWriter(fw);
          PrintWriter pw = new PrintWriter(bw);
 
          pw.write("NUEVA ENTRADA DE DATOS\n");
 
          pw.close();
 
 
       }catch(IOException ioe){
           System.out.println("Exception ");
           ioe.printStackTrace();
      }
   

}

void draw() {
  println();
  c=myServer.available();
  if (c != null) {
    input=c.readString();      
    if(input==null || input==""){
       println("<<mensaje nulo>>");
    }
    else{
      escribir(input);
      println("El mensaje es: "+input);
      myServer.write("ACK\n");
      println("escribo ACK");
    }
  }
  else{
    println("<<no hay mensaje>>");
  }
  myServer.write("peticion\n");
  delay(5000);
}

void escribir(String datos){
  try{
    //chemin = dataPath;
// positions.txt== your file;
          File file =new File("/home/eskri96/Documentos/clase/Elco/proyectogit/Comunicacion/WIFI/PROCESSINGWIFI/fichero.txt"); 
 
          FileWriter fw = new FileWriter(file,true);
          BufferedWriter bw = new BufferedWriter(fw);
          PrintWriter pw = new PrintWriter(bw);
 
          pw.write(datos+"\n");
 
          pw.close();
 
       }catch(IOException ioe){
           System.out.println("Exception ");
           ioe.printStackTrace();
      }
}
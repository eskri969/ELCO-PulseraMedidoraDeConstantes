
import processing.serial.*;    
import grafica.*;
import g4p_controls.*;
import processing.net.*;
import java.io.FileWriter;
import java.io.*;

// CAMBIAR A LA RUTA DEL DIRECTORIO RAIZ DEL PROYECTO( SIN AÑADIR / AL FINAL)
String path = "C:/Users/barra_000/Downloads/Processing_User_Interface";

//Variables representacion
boolean flagpintar=true;

//Variable guardar path para archivo
String result="";
int state;
boolean boton=false;
boolean pintado;

//Vista (1...4)
int vista;

//Variables para tiempo
int m;
int m0;

//Variables string para recibir de arduino
String [] lines;
String rep;


//Declaramos imagenes
PImage img1;
PImage img2;
PImage img3;
PImage img4;
PImage img5;
PImage img6;
PImage img7;

//Declaramos botones
GButton btn1;
GButton btn2;
GButton btn3;
GButton btn4;
GButton btn5;
GButton btn6;
GButton btn7;
GButton btn8;
GButton btn9;
GButton btn10;

// Server
Server myServer;

// Plot from grafica library
GPlot plot1;
GPlot plot2;
GPlot plot3;
GPlot plot4;
GPlot plot5;
GPlot plot6;
GPlot plot7;
GPlot plot8;
GPlot plot9;

Client c;
int val = 0;
String output;
String input;
int tout=5000;
BufferedWriter bw;
//PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter("outfilename", true))); // true = append

//Array para datos que vienen del get*4

//**********************SETUP********************************
void setup() {
  // Initialize Window
  size(800, 600); 
  
  // Starts a myServer on port 3000
  myServer = new Server(this, 3000);
  
  try{
    //chemin = dataPath;
// positions.txt== your file;
          File file =new File(path +  "/randomfile.txt"); 
          if(!file.exists()){
            file.createNewFile();
          }
 
          FileWriter fw = new FileWriter(file,true);//a true hace append
          BufferedWriter bw = new BufferedWriter(fw);
          PrintWriter pw = new PrintWriter(bw);
 
       //   pw.write("NUEVA ENTRADA DE DATOS\n");
 
          pw.close();
 
 
       }catch(IOException ioe){
           System.out.println("Exception ");
           ioe.printStackTrace();
      }
 
  // Load images
  //img1=loadImage("C:/Users/Usuario/Desktop/Ingenieria Biomedica/4º curso/2º cuatrimestre/ELCO/PROCESSINGWIFI_EDIT/Imágenes/Logo_TFB.png");
  img2=loadImage(path + "/Imágenes/Logo_TFB2.png");
  img3=loadImage(path + "/Imágenes/Photon.png");
  img4=loadImage(path + "/Imágenes/upm.png"); //<>//
  img5=loadImage(path + "/Imágenes/logo.png");
  img6=loadImage(path + "/Imágenes/enfermo.png");
  img7=loadImage(path + "/Imágenes/feliz.png");

  // Initialize Plot1
  plot1 = new GPlot(this);
  plot1.setPos(75,25);
  plot1.setDim(200,150);
  plot1.setYLim(0,220);
  plot1.getTitle().setText("Pulso");
  plot1.getXAxis().getAxisLabel().setText(" Tiempo (s)");
  plot1.getYAxis().getAxisLabel().setText("BPM");
  plot1.setBoxBgColor(color(#FFFFFF));
  //plot1.activateZooming(1.5);
 
  // Initialize Plot2
  plot2 = new GPlot(this);
  plot2.setPos(425,25);
  plot2.setDim(200,150);
  plot2.setYLim(30,50);
  plot2.getTitle().setText("Temperatura (ºC)");
  plot2.getXAxis().getAxisLabel().setText(" Tiempo (s)");
  plot2.getYAxis().getAxisLabel().setText("Temperatura");
  plot2.setBoxBgColor(color(#FFFFFF));
  //plot2.activateZooming(1.5);
  
  // Initialize Plot3
  plot3 = new GPlot(this);
  plot3.setPos(75,325);
  plot3.setDim(200,150);
  plot3.setYLim(80,100);
  plot3.getTitle().setText("Saturación O2");
  plot3.getXAxis().getAxisLabel().setText(" Tiempo (s)");
  plot3.getYAxis().getAxisLabel().setText("PO2 (%)");
  plot3.setBoxBgColor(color(#FFFFFF));
  //plot3.activateZooming(1.5);

  // Initialize Plot4
  plot4 = new GPlot(this);
  plot4.setPos(100,50);
  plot4.setDim(500,350);
  plot4.setYLim(80,110);
  plot4.getTitle().setText("Saturación O2");
  plot4.getXAxis().getAxisLabel().setText(" Tiempo (s)");
  plot4.getYAxis().getAxisLabel().setText("PO2 (%)");
  plot4.setBoxBgColor(color(#FFFFFF));
 //plot4.activateZooming(1.5);
  
  // Initialize Plot5
  plot5 = new GPlot(this);
  plot5.setPos(100,50);
  plot5.setDim(500,350);
  plot5.setYLim(0,220);
  plot5.getTitle().setText("Pulso");
  plot5.getXAxis().getAxisLabel().setText(" Tiempo (s)");
  plot5.getYAxis().getAxisLabel().setText("BPM");
  plot5.setBoxBgColor(color(#FFFFFF));
  //plot5.activateZooming(1.5);

  // Initialize Plot6
  plot6 = new GPlot(this);
  plot6.setPos(100,50);
  plot6.setDim(500,350);
  plot6.setYLim(30,50);
  plot6.getTitle().setText("Temperatura (ºC)");
  plot6.getXAxis().getAxisLabel().setText(" Tiempo (s)");
  plot6.getYAxis().getAxisLabel().setText("Temperatura");
  plot6.setBoxBgColor(color(#FFFFFF));
  //plot6.activateZooming(1.5);
  
  // Initialize Plot7
  plot7 = new GPlot(this);
  plot7.setPos(75,25);
  plot7.setDim(200,150);
  plot7.setYLim(0,220);
  plot7.getTitle().setText("Pulso");
  plot7.getXAxis().getAxisLabel().setText(" Tiempo (s)");
  plot7.getYAxis().getAxisLabel().setText("BPM");
  plot7.setBoxBgColor(color(#FFFFFF));

  // Initialize Plot8
  plot8 = new GPlot(this);
  plot8.setPos(425,25);
  plot8.setDim(200,150);
  plot8.setYLim(30,50);
  plot8.getTitle().setText("Temperatura (ºC)");
  plot8.getXAxis().getAxisLabel().setText(" Tiempo (s)");
  plot8.getYAxis().getAxisLabel().setText("Temperatura");
  plot8.setBoxBgColor(color(#FFFFFF));
  //plot2.activateZooming(1.5);
  
  // Initialize Plot3
  plot9 = new GPlot(this);
  plot9.setPos(75,325);
  plot9.setDim(200,150);
  plot9.setYLim(80,100);
  plot9.getTitle().setText("Saturación O2");
  plot9.getXAxis().getAxisLabel().setText(" Tiempo (s)");
  plot9.getYAxis().getAxisLabel().setText("PO2 (%)");
  plot9.setBoxBgColor(color(#FFFFFF));
  
  // Initialize Buttons
  btn1 = new GButton(this, 70, 250, 300, 60, "Ver todos los parámetros"); 
  btn2 = new GButton(this, 70, 310, 300, 60, "O2");
  btn3 = new GButton(this, 70, 370, 300, 60, "Pulso"); 
  btn4 = new GButton(this, 70, 430, 300, 60, "Temperatura");
  btn5 = new GButton(this, 425, 325, 300, 100, "Volver a pantalla inicial");
  btn6 = new GButton(this, 250, 500, 300, 100, "Volver a pantalla inicial");
  btn7 = new GButton(this, 570, 500, 100, 100, "Salvar gráfico");
  btn8 = new GButton(this, 70, 490, 300, 60, "Representar archivo existente");
  btn9 = new GButton(this, 250, 410, 300, 90, "REPRESENTAR");
  btn10 = new GButton(this, 425, 425, 300, 100, "Representar otro archivo");
}

//****************** DRAW ******************************
void draw() {
  println();
  println("Estás en la vista "+str(vista));
  if(vista==0){
    vista0();
  }else if(vista==1){
    vista1();
  }else if(vista==2){
    vista2();
  }else if(vista==3){
    vista3();
  }else if(vista==4){
    vista4();
  }else if(vista==5){
    vista5();}
   else if(vista==6){
    vista6();};
  
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
      flagpintar=true;

    }
  }
  else{
    println("<<no hay mensaje>>");
      
    myServer.write("peticion\n");
    delay(500);
  
}  
}

//*********************** ESCRIBIR ********************
void escribir(String datos){
  try{
    //chemin = dataPath;
// positions.txt== your file;
          File file =new File(path + "/randomfile.txt"); 
 
          FileWriter fw = new FileWriter(file,true);
          BufferedWriter bw = new BufferedWriter(fw);
          PrintWriter pw = new PrintWriter(bw);
 
          pw.write("\n"+datos);
 
          pw.close();
 
       }catch(IOException ioe){
           System.out.println("Exception ");
           ioe.printStackTrace();
      }
}

//****************** PINTAR *********************
void pintar(String dat){
      
      //rellenamos array con entrada de datos
      float[] datos = new float[2];
      datos = float(split(dat,':'));
      
      //Pintamos lo relacionado con el primer gráfico
      if(flagpintar==true)
      plot1.addPoint(m, datos[0],"(" + str(m) + "," + str(datos[0]) + ")");
      
      plot1.beginDraw();
      plot1.drawBackground();
      plot1.drawBox();
      plot1.drawXAxis();
      plot1.drawYAxis();
      plot1.drawTitle();
      plot1.drawGridLines(GPlot.BOTH);
      plot1.drawLines();
      plot1.endDraw(); 
      
      //Pintamos lo relacionado con el segundo gráfico
      if(flagpintar==true)
      plot2.addPoint(m, datos[1],"(" + str(m) + "," + str(datos[1]) + ")");
      
      plot2.beginDraw();
      plot2.drawBackground();
      plot2.drawBox();
      plot2.drawXAxis();
      plot2.drawYAxis();
      plot2.drawTitle();
      plot2.drawGridLines(GPlot.BOTH);
      plot2.drawLines();
      plot2.endDraw();
      
      //Pintamos lo relacionado con el tercer gráfico
      if(flagpintar==true)
      plot3.addPoint(m, datos[2],"(" + str(m) + "," + str(datos[2]) + ")");
      
      plot3.beginDraw();
      plot3.drawBackground();
      plot3.drawBox();
      plot3.drawXAxis();
      plot3.drawYAxis();
      plot3.drawTitle();
      plot3.drawGridLines(GPlot.BOTH);
      plot3.drawLines();
      plot3.endDraw();
      
      flagpintar=false;
}

// PINTAR 1
void pintar1(String dat){
      
      //Creamos array y lo rellenamos con entrada de datos
      float[] datos = new float[2];
      datos = float(split(dat,':'));
      
      //Pintamos lo relacionado con el primer gráfico
      if(flagpintar==true)
      plot4.addPoint(m, datos[2],"(" + str(m) + "," + str(datos[2]) + ")");
      
      plot4.beginDraw();
      plot4.drawBackground();
      plot4.drawBox();
      plot4.drawXAxis();
      plot4.drawYAxis();
      plot4.drawTitle();
      plot4.drawGridLines(GPlot.BOTH);
      plot4.drawLines();
      plot4.endDraw();
      
      flagpintar=false;
}

 // PINTAR 2
void pintar2(String dat){

      float[] datos = new float[2]; 
      datos = float(split(dat,':'));   
      
      //Pintamos lo relacionado con el segundo gráfico
      if(flagpintar==true)
      plot5.addPoint(m, datos[0],"(" + str(m) + "," + str(datos[0]) + ")");
      
      plot5.beginDraw();
      plot5.drawBackground();
      plot5.drawBox();
      plot5.drawXAxis();
      plot5.drawYAxis();
      plot5.drawTitle();
      plot5.drawGridLines(GPlot.BOTH);
      plot5.drawLines();
      plot5.endDraw();
      
      flagpintar=false;
}

// PINTAR 3
void pintar3(String dat){
      
      float[] datos = new float[2];
      datos = float(split(dat,':'));
      
      //Pintamos lo relacionado con el tercer gráfico
      if(flagpintar==true)
      plot6.addPoint(m, datos[1],"(" + str(m) + "," + str(datos[1]) + ")");
      
      plot6.beginDraw();
      plot6.drawBackground();
      plot6.drawBox();
      plot6.drawXAxis();
      plot6.drawYAxis();
      plot6.drawTitle();
      plot6.drawGridLines(GPlot.BOTH);
      plot6.drawLines();
      plot6.endDraw();
      
      flagpintar=false;
}

void pintar6(){
   //Cargamos archivo y leemos última linea (en version prueba leemos linea a linea incrementando. CAMBIAR
   lines = loadStrings(path + "/Representar/"+result);
   //lines = loadStrings("C:/Users/Usuario/Desktop/randomfile.txt");
   
   float[] datos = new float[2];
  
   //añadimos los puntos a los plots
if(pintado==false){
  for(int i=0;i<lines.length;i++){
     datos = float(split(lines[i],':'));
     
     plot7.addPoint(i, datos[0],"(" + str(i) + "," + str(datos[0]) + ")");
     plot8.addPoint(i, datos[1],"(" + str(i) + "," + str(datos[1]) + ")");
     plot9.addPoint(i, datos[2],"(" + str(i) + "," + str(datos[2]) + ")");
  }
} 
   //Pintamos plot7, 8 y 9.
   plot7.beginDraw();
   plot7.drawBackground();
   plot7.drawBox();
   plot7.drawXAxis();
   plot7.drawYAxis();
   plot7.drawTitle();
   plot7.drawGridLines(GPlot.BOTH);
   plot7.drawLines();
   plot7.endDraw();
   
   plot8.beginDraw();
   plot8.drawBackground();
   plot8.drawBox();
   plot8.drawXAxis();
   plot8.drawYAxis();
   plot8.drawTitle();
   plot8.drawGridLines(GPlot.BOTH);
   plot8.drawLines();
   plot8.endDraw();
   
   plot9.beginDraw();
   plot9.drawBackground();
   plot9.drawBox();
   plot9.drawXAxis();
   plot9.drawYAxis();
   plot9.drawTitle();
   plot9.drawGridLines(GPlot.BOTH);
   plot9.drawLines();
   plot9.endDraw();
  
   pintado=true;
}

//***************** VISTAS *****************
//VISTA 0
void vista0(){
  //Ocultamos/mostramos botones y fijamos background
  background(173,216,230);
  btn1.setVisible(true);
  btn2.setVisible(true);
  btn3.setVisible(true);
  btn4.setVisible(true);
  btn5.setVisible(false);
  btn6.setVisible(false);
  btn7.setVisible(false);
  btn8.setVisible(true);
  btn9.setVisible(false);
  btn10.setVisible(false);
  
  //Añadimos imagen Biocare
  if(mouseX<745 && mouseX>45 && mouseY>25 && mouseY<200){
      
    //Rectangulo
    fill(255);
    rect(45,25,700,175,7);
    
    //Añadimos titulo
    textSize(26);
    fill(0);
    text("TRABAJO ELCO & BIOFOTÓNICA. CURSO 2017/18",80,65);
    
    //Añadimos nombres
    
    textSize(15);
    fill(0);
    text("Álvaro Escribano",150,100);
    text("Isabel Martín",150,135);
    text("Peña Arroyo",150,170);
    text("Pablo Tezanos",425,100);
    text("Rafael Sendra",425,135);
    text("Pablo Aguilar",425,170);
    
    
  }else{image(img5,45,25,700,175);}
 
  //Añadimos logos proyecto
  //image(img1,400,250,148,148);
  image(img2,420,250,296,148);
  image(img3,420,400,148,148);
  image(img4,570,400,148,148);
  
}

//VISTA 1
void vista1(){ 
  
  //Fondo
  background(173,216,230);
  
  //fijamos/ocultamos botones
  btn1.setVisible(false);
  btn2.setVisible(false);
  btn3.setVisible(false);
  btn4.setVisible(false);
  btn5.setVisible(true);
  btn6.setVisible(false);
  btn7.setVisible(false);    
  btn8.setVisible(false);
  btn9.setVisible(false);
  btn10.setVisible(false);
  
  //Cargamos archivo y leemos última linea (en version prueba leemos linea a linea incrementando. CAMBIAR
  lines = loadStrings(path + "/randomfile.txt");
  rep = lines[lines.length -1];
  float[] datos = new float[2];
  datos = float(split(rep,':'));
  
  //rep = lines[lines.length-1];
  m = (second()+60*minute()+3600*hour()+millis()/1000)-m0;
   
  
  //Añadimos rectángulo
  fill(255);
  rect(425,425,298,150,7);
  
  //Titulo
  textSize(32);
  fill(0);
  text("Resumen",505,460);
  
  //Saturación O2
  textSize(16);
  if(datos[2]<95){fill(200,0,0);}else{fill(0);};
  text("► Saturación de O2:  "+datos[2],475,490);
  if(datos[1]>37.2||datos[1]<36.1){fill(200,0,0);}else{fill(0);};
  text("► Temperatura:         "+datos[1],475,520);
  if(datos[0]<40||datos[0]>120){fill(200,0,0);}else{fill(0);};
  text("► Pulsaciones:           "+datos[0],475,550);
  
  //Dibujamos gráfica e imprimimos traza
  println(rep);
  pintar(rep);
  
}

//VISTA 2
void vista2(){
  
  //Ocultamos/mostramos botones y fijamos background
  background(173,216,230);
  

  
  //fijamos/ocultamos botones
  btn1.setVisible(false);
  btn2.setVisible(false);
  btn3.setVisible(false);
  btn4.setVisible(false);
  btn5.setVisible(false);
  btn6.setVisible(true);
  btn7.setVisible(true);
  btn8.setVisible(false);
  btn9.setVisible(false);
  btn10.setVisible(false);
  
  //Cargamos archivo y leemos última linea (en version prueba leemos linea a linea incrementando. CAMBIAR
  lines = loadStrings(path + "/randomfile.txt");
  rep = lines[lines.length - 1];
  //rep = lines[lines.length-1];
  m = (second()+60*minute()+3600*hour()+millis()/1000)-m0;
    
  
  //Creamos array con datos
  float[] datos = new float[2];
  datos = float(split(rep,':'));
  
  //Fijamos imagen en funcion de valor
  if(datos[2]<95){image(img6,120,500,100,100);}else{image(img7,120,500,100,100);};
  
  //Dibujamos gráfica e imprimimos traza
  println(rep);
  pintar1(rep);
}

//VISTA 3
void vista3(){
  
  //Ocultamos/mostramos botones y fijamos background.
  background(173,216,230);
  
  btn1.setVisible(false);
  btn2.setVisible(false);
  btn3.setVisible(false);
  btn4.setVisible(false);
  btn5.setVisible(false);
  btn6.setVisible(true);
  btn7.setVisible(true);
  btn8.setVisible(false);
  btn9.setVisible(false);
  btn10.setVisible(false);
  
  //Cargamos archivo y leemos última linea (en version prueba leemos linea a linea incrementando. CAMBIAR
  lines = loadStrings(path + "/randomfile.txt");
  rep = lines[lines.length - 1];
  //rep = lines[lines.length-1];
  m = (second()+60*minute()+3600*hour()+millis()/1000)-m0;
   
  
  //Creamos array con datos
  float[] datos = new float[2];
  datos = float(split(rep,':'));
  
  //Fijamos imagen en funcion de valor
  if(datos[0]<40||datos[0]>120){image(img6,120,500,100,100);}else{image(img7,120,500,100,100);};

  
  //Dibujamos gráfica e imprimimos traza
  println(rep);
  pintar2(rep);
}

//VISTA 4
void vista4(){
  //Ocultamos/mostramos botones y fijamos background en funcion de alert
  background(173,216,230);
    
  btn1.setVisible(false);
  btn2.setVisible(false);
  btn3.setVisible(false);
  btn4.setVisible(false);
  btn5.setVisible(false);
  btn6.setVisible(true);
  btn7.setVisible(true);
  btn8.setVisible(false);
  btn9.setVisible(false);
  btn10.setVisible(false);
  
  //Cargamos archivo y leemos última linea (en version prueba leemos linea a linea incrementando. CAMBIAR
  lines = loadStrings(path + "/randomfile.txt");
  rep = lines[lines.length - 1];
  //rep = lines[lines.length-1];
  m = (second()+60*minute()+3600*hour()+millis()/1000)-m0;
   
  
  //Creamos array con datos
  float[] datos = new float[2];
  datos = float(split(rep,':'));
  
  //Fijamos imagen en funcion de valor
  if(datos[1]>37.2||datos[1]<36.1){image(img6,120,500,100,100);}else{image(img7,120,500,100,100);};

  //Dibujamos gráfica e imprimimos traza
  println(rep);
  pintar3(rep);
}

//Vista 5
void vista5(){
  //Ocultamos/mostramos botones y fijamos background
  background(173,216,230);
 
  btn1.setVisible(false);
  btn2.setVisible(false);
  btn3.setVisible(false);
  btn4.setVisible(false);
  btn5.setVisible(false);
  btn6.setVisible(true);
  btn7.setVisible(false);
  btn8.setVisible(false);
  btn9.setVisible(true);
  btn10.setVisible(false);
  
  if(boton==false){btn9.setVisible(false);}else{btn9.setVisible(true);};
  //Imagen biocare
  image(img5,45,25,700,175);
  
  //Fijamos caja de texto de instrucciones
  fill(255);
  rect(45,205,700,150,7);
  rect(45,358,700,50,7);
  
  fill(0);
  textSize(32);
  text("INSTRUCCIONES",275,240);
  
  textSize(16);
  text("1-Escriba la dirección en la que se encuentra el archivo que quiere representar.",65,270);
  text("2-Presione ENTER para que se guarde la dirección escrita.",65,300);
  text("3-Tras guardarse la dirección del archivo aparecerá un botón. Pulselo",65,330);
  
  textSize(20);
  switch (state) {
  case 0:
    fill(0); 
    text (result, 70, 390); 
    break;
 
  case 1:
    fill(2, 255, 2); 
    text ("Thanks", 350, 390); 
    boton=true;
    break;
  }
  
}

//VISTA 6
void vista6(){
  
  //Ocultamos/mostramos botones y fijamos background
  background(173,216,230);
    
  //fijamos/ocultamos botones
  btn1.setVisible(false);
  btn2.setVisible(false);
  btn3.setVisible(false);
  btn4.setVisible(false);
  btn5.setVisible(true);
  btn6.setVisible(false);
  btn7.setVisible(false);
  btn8.setVisible(false);
  btn9.setVisible(false);
  btn10.setVisible(true);
  
  pintar6();
  
}
//************* EVENTOS DE BOTONES *******************
public void handleButtonEvents(GButton button, GEvent event) {  
 
  // If Button 1 is pushed
  if ( button == btn1 ){
    m0=second()+60*minute()+3600*hour()+millis()/1000;
    vista=1;
  }
  
  // If Button 2 is pushed
  if ( button == btn2 ){
    vista=2;
  }
  
  // If Button 3 is pushed
  if ( button == btn3 ){
    vista=3;
  }
  
  // If Button 4 is pushed
  if ( button == btn4 ){
    vista=4;
  }

  // If Button 5 is pushed
  if ( button == btn5 ){
    
    if(vista==6){
      result="";
      plot7.setPoints(new GPointsArray());
      plot8.setPoints(new GPointsArray());
      plot9.setPoints(new GPointsArray());
      pintado=false;
      boton = false;
      state = 0;
    }
    
    vista=0;

  }
  
  // If Button 6 is pushed
  if ( button == btn6 ){
    vista=0;
  }
  
    // If Button 7 is pushed
  if ( button == btn7 ){
    if(vista==2){
      saveFrame(path + "/Screenshots/PO2-######.png");
    }else if(vista==3){
      saveFrame(path + "/Screenshots/BPM-######.png");
    }else if(vista==4){
      saveFrame(path + "/Temperatura-######.png");
    };
  }
  
  // If Button 8 is pushed
  if ( button == btn8 ){
    vista=5;
  }
  
  // If Button 9 is pushed
  if ( button == btn9 ){
    vista=6;
  }
  
  // If Button 10 is pushed
  if ( button == btn10 ){
    
    result="";
    plot7.setPoints(new GPointsArray());
    plot8.setPoints(new GPointsArray());
    plot9.setPoints(new GPointsArray());
    pintado=false;
    boton = false;
    state = 0;
    
    vista=5;
    
  }
}

//********* KEYS *********

void keyPressed() {
 
  if(vista==5){
  if (key==ENTER||key==RETURN) {
    state = 1;
  } else {
    result = result + key;
  }
  }
}

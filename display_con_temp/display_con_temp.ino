#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);


#define D8 15
#define DELAY 20
#define TEMP_MUESTRAS 50

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

  float temp[TEMP_MUESTRAS];
  float tempResult = 0;

  bool animacion = true;


static const unsigned char PROGMEM logo16_glcd_bmp[] =

{ 
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00001110, B01110000,
  B00001111, B11110000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,};

  static const unsigned char PROGMEM logo16_glcd_bmp_2[] =

{ 
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00011100, B00111000,
  B00111110, B01111100,
  B00111111, B11111100,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,};


  static const unsigned char PROGMEM logo16_glcd_bmp_3[] =

{ 
  B00000000, B00000000,
  B00000000, B00000000,
  B00000011, B11000000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,};
  

    static const unsigned char PROGMEM logo16_glcd_bmp_4[] =

{ 
  B00000000, B00000000,
  B00000000, B00000000,
  B00000011, B11000000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,};




int counter = 0;

int current_state = 0;


void setup() {
  
  pinMode(D8, INPUT_PULLUP);
  //digitalWrite(D8, HIGH);
  attachInterrupt(digitalPinToInterrupt(D8), handle_button, RISING);
  Serial.begin(9600);  

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);


  display.display();
  delay(200);

  // Clear the buffer.
  display.clearDisplay();

  mlx.begin();  
}



void loop() {
  //Serial.print(counter);
  if(counter == 0){
    if (current_state == 1){
      delay(10);
    }else{
    current_state = 1;
    display.stopscroll();
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.println("Inicio");
    display.display();
    display.startscrollleft(0x00, 0x0F);
    //delay(200);
    //display.stopscroll();
    display.clearDisplay();
    }
  }
  else if(counter == 1){
    if (current_state == 1){
      delay(10);
    }else{
    current_state = 1;
    display.stopscroll();
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.println("Pulso");
    display.display();
    display.startscrollleft(0x00, 0x0F);
    //delay(2000);
    //display.stopscroll();
    display.clearDisplay();
    }
  }
  else if(counter == 2){
    if (current_state == 1){
      delay(10);
    }else{
    current_state = 1;
    display.stopscroll();
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.println("Temperat.");
    display.display();
    display.startscrollleft(0x00, 0x0F);
    //delay(2000);
    //display.stopscroll();
    display.clearDisplay();
    }
  }
  else if(counter == 3){
    if (current_state == 1){
      delay(10);
    }else{
    current_state = 1;
    display.stopscroll();
    display.clearDisplay();

    for (int i=0; i <= 5; i++){
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Midiendo pulso . .");
      display.drawBitmap(110, 20,  logo16_glcd_bmp, 16, 16, WHITE);
      display.display();
      delay(500);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Midiendo pulso . . .");
      display.drawBitmap(110, 20,  logo16_glcd_bmp_2, 16, 16, WHITE);
      display.display();
      delay(500);
      display.clearDisplay();
   }
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(10,10);
      display.print("75");
      display.setTextSize(1);
      display.println("ppm");
      display.drawBitmap(110, 20,  logo16_glcd_bmp_2, 16, 16, WHITE);
      display.display();
      delay(2000);
    //display.drawBitmap(20, 0,  logo16_glcd_bmp, 16, 16, WHITE);
    //display.display();
    //delay(2000);
    //display.stopscroll();
    display.clearDisplay();
    counter = counter + 1;
    current_state = 0;
    }
  }

  else if(counter == 4){
    if (current_state == 1){
      delay(10);
    }else{
    current_state = 1;
    display.stopscroll();
    display.clearDisplay();

//    for (int i=0; i <= 10; i++){
//      display.setTextSize(1);
//      display.setTextColor(WHITE);
//      display.setCursor(0,0);
//      display.println("Midiendo temp .");
//      display.drawBitmap(110, 15,  logo16_glcd_bmp_3, 16, 16, WHITE);
//      display.display();
//      delay(500);
//      display.clearDisplay();
//      display.setTextSize(1);
//      display.setTextColor(WHITE);
//      display.setCursor(0,0);
//      display.println("Midiendo temp . .");
//      display.drawBitmap(110, 15,  logo16_glcd_bmp_4, 16, 16, WHITE);
//      display.display();
//      delay(500);
//      display.clearDisplay();
//   }
      mideTemp();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(10,10);
      display.print(tempResult);
      display.print((char)247); //degree symbol
      display.println("C");
      display.drawBitmap(110, 15,  logo16_glcd_bmp_4, 16, 16, WHITE);
      display.display();
      delay(5000);
    //display.drawBitmap(20, 0,  logo16_glcd_bmp, 16, 16, WHITE);
    //display.display();
    //delay(2000);
    //display.stopscroll();
    display.clearDisplay();
    counter = counter + 1;
    current_state = 0;
    }
  }


  
  else if(counter > 4){
    counter = 0;
  }
  else{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Raro raro raro");
    display.display();
    delay(200);
    display.clearDisplay();
  }
}

void handle_button()
{
  
  //delay(DELAY);
  Serial.print("\nBoton presionado");
  counter = counter + 1;
  current_state = 0;
  
  //int button_pressed = !digitalRead(D8); // pin low -> pressed
  //return button_pressed;
}


void mideTemp(){
  tempResult = 0;
  for (int i=0; i <= (TEMP_MUESTRAS-1); i++){

    if (((i%5)==0) and (animacion == true)){
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Midiendo temp . .");
      display.drawBitmap(110, 15,  logo16_glcd_bmp_3, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      animacion = !animacion;
    }
     else if ((((i%5)==0) or (i==(TEMP_MUESTRAS-1)) ) and (animacion == false)){
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Midiendo temp . . .");
      display.drawBitmap(110, 15,  logo16_glcd_bmp_4, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      animacion = !animacion;
    }

    
    
    temp[i] = mlx.readObjectTempC();
    //Serial.print("Temp Array = "); Serial.println(temp[i]);

    tempResult = tempResult + temp[i];

    if (i == (TEMP_MUESTRAS-1)){
      tempResult = (tempResult/TEMP_MUESTRAS);
      //Serial.print("Temp Result = "); Serial.println(tempResult);
      //Serial.println();
    }
  
  delay(100); 
  }
  animacion = true;
}



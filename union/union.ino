#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);


#define D8 15
#define D3 0
#define DELAY 20
#define TEMP_MUESTRAS 50

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

#define LOGO_FULL_GLCD_HEIGHT 32
#define LOGO_FULL_GLCD_WIDTH  128


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float temp[TEMP_MUESTRAS];
int pulseResult = 0;
float tempResult = 0;
int oxygenResult = 0;

bool animacion = true;
bool supermenu = true;



static const unsigned char PROGMEM logo128_logo[] =
{
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000011, B11111111, B11110000, B00000000, B00000000, B00000000, B00000000, B00011111, B11111111, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000100, B01111111, B11111000, B00000000, B00000000, B00000000, B00000000, B00100011, B11111111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000101, B11111111, B11111000, B01111100, B00111111, B11111000, B00000000, B00101111, B11111111, B10000011, B11111111, B10000011, B11111111, B10000011, B11111111, B11000000,
  B00000101, B11111100, B01111000, B10001110, B01000111, B11111100, B00000000, B00101111, B11000000, B00000100, B01111111, B11000100, B01111111, B11000100, B01111111, B11100000,
  B00000111, B11111100, B01111000, B10111110, B01011111, B11111110, B00000000, B00111111, B11000000, B00000101, B11111111, B11100101, B11111111, B11100101, B11111111, B11000000,
  B00000111, B11111100, B01111000, B10111110, B01011111, B10001110, B00000000, B00111111, B11000000, B00000101, B11111000, B11100101, B11110001, B11100101, B11110000, B00000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111000, B11100111, B11110001, B11100111, B11110000, B00000000,
  B00000111, B11111111, B11110000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111111, B11100111, B11111111, B11100111, B11111111, B11000000,
  B00000111, B11111100, B01111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111111, B11100111, B11111111, B11000111, B11111111, B11100000,
  B00000111, B11111100, B01111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111111, B11100111, B11111111, B10000111, B11111111, B11000000,
  B00000111, B11111100, B01111000, B11111110, B01111111, B10001110, B00000000, B00111111, B11000000, B00000111, B11111000, B11100111, B11111111, B11000111, B11110000, B00000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B11111110, B00000000, B00111111, B11111111, B10000111, B11111000, B11100111, B11110111, B11100111, B11110000, B00000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B11111110, B00000000, B00111111, B11111111, B11000111, B11111000, B11100111, B11110001, B11100111, B11111111, B11000000,
  B00000111, B11111111, B11111000, B11111110, B01111111, B11111110, B00000000, B00111111, B11111111, B11000111, B11111000, B11100111, B11110001, B11100111, B11111111, B11100000,
  B00000111, B11111111, B11110000, B11111110, B00111111, B11111100, B00100000, B00011111, B11111111, B11000111, B11111000, B11100111, B11110001, B11100111, B11111111, B11100000,
  B00000011, B11111111, B11100000, B01111100, B00011111, B11111000, B00100000, B00001111, B11111111, B10000011, B11100000, B01000011, B11100000, B01000011, B11111111, B11000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B01110000, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B01010001, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11011001, B01000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B10001001, B01100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000111, B11111111, B11111111, B11111111, B11111111, B11111111, B10001011, B00111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11100000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00001110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000100, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
};



static const unsigned char PROGMEM logo16_pulse_0[] =
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
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_pulse_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
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
};


static const unsigned char PROGMEM logo16_pulse_2[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B01111100, B00111110,
  B11111110, B01111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B01111111, B11111110,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_temp_1[] =
{
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
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_temp_2[] =
{
  B00000000, B00000000,
  B00000011, B11000000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000100, B00100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_o2_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000001, B10000000,
  B00000011, B11000000,
  B00000111, B11100000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00111111, B11111100,
  B01111111, B11111110,
  B01111111, B11111110,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_o2_2[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00001110, B00000000,
  B00110001, B10000000,
  B01100000, B11000000,
  B01100000, B11001100,
  B01100000, B11010010,
  B01100000, B11000100,
  B00110001, B10001000,
  B00001110, B00011110,
  B00000000, B00000000,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_conect_1[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00001111,
  B00000000, B00001111,
  B00000000, B00001111,
  B00000011, B11001111,
  B00000011, B11001111,
  B00000011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B11110011, B11001111,
  B00000000, B00000000,
};


static const unsigned char PROGMEM logo16_conect_2[] =
{
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000001, B00000100,
  B00000011, B10001110,
  B00000111, B11011111,
  B00000011, B11111110,
  B00000001, B11111100,
  B00000000, B11111000,
  B00000001, B11111100,
  B00000011, B11111110,
  B00000111, B11011111,
  B00000011, B10001110,
  B00000001, B00000100,
  B00000000, B00000000,
};



int menu_number = 0;

int current_state = 0;

int inside_menu = 0;


void setup() {

  pinMode(D8, INPUT_PULLUP);
  //digitalWrite(D8, HIGH);
  attachInterrupt(digitalPinToInterrupt(D3), menu_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(D8), select_button, RISING);
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);


  //display.display();
  //delay(200);

  // Clear the buffer.
  display.clearDisplay();
  display.drawBitmap(0, 0,  logo128_logo, 128, 32, WHITE);
  display.display();
  delay(2000);

  mlx.begin();
}



void loop() {

  if (menu_number == 0) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        supermenu = false;
        midePulso();
        mideTemp();
        mideO2();
        showResults();
        current_state = 0;
        supermenu = true;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Auto");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 1) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        menu_number = menu_number + 1;
        current_state = 0;
        supermenu = false;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Manual");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 2) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        midePulso();
        current_state = 0;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Pulso");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 3) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        mideTemp();
        current_state = 0;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Temperat.");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 4) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        mideO2();
        current_state = 0;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Sat. de O2");
      display.display();
      display.clearDisplay();
    }
  }

  else if (menu_number == 5) {
    if (current_state == 1) {
      if (inside_menu == 1) {
        menu_number = 1;
        current_state = 0;
        supermenu = true;
        inside_menu = 0;
      }
      //do nothing
    } else {
      current_state = 1;
      setDisplayMenu();
      display.println("Exit");
      //display.drawBitmap(110, 0,  logo16_conect_2, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
    }
  }


  else if (menu_number > 5) {
    menu_number = 2;
  }
  else {
    setDisplayMenu();
    display.println("Raro raro");
    display.display();
    delay(200);
    display.clearDisplay();
  }
}

void menu_button()
{
  if (inside_menu == 0) {
    //Serial.print("\nBoton de menu presionado");
    if (supermenu) {
      if (menu_number == 0) {
        Serial.print("\nEntra de 0 a 1");
        menu_number = 1;
        current_state = 0;
      }
      else {
        Serial.print("\nEntra de 1 a 0");
        menu_number = 0;
        current_state = 0;
      }
    }
    else {
      menu_number = menu_number + 1;
      current_state = 0;
    }
  }
}

void select_button()
{

  Serial.print("\nBoton de seleccion presionado");
  inside_menu = 1;
}

void setDisplayMenu() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
}

void setDisplayMeasure() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

void setDisplayResult() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
}


void midePulso() {
  for (int i = 0; i <= 5; i++) {
    setDisplayMeasure();
    display.println("Midiendo\npulso . .");
    display.drawBitmap(110, 15,  logo16_pulse_1, 16, 16, WHITE);
    display.display();
    delay(500);
    setDisplayMeasure();
    display.println("Midiendo\npulso . . .");
    display.drawBitmap(110, 15,  logo16_pulse_2, 16, 16, WHITE);
    display.display();
    delay(500);
    display.clearDisplay();
  }
  setDisplayResult();
  pulseResult = 75;
  display.print(pulseResult);
  display.setTextSize(1);
  display.println("lpm");
  display.drawBitmap(110, 15,  logo16_pulse_2, 16, 16, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
}







void mideTemp() {
  tempResult = 0;
  for (int i = 0; i <= (TEMP_MUESTRAS - 1); i++) {

    if (((i % 5) == 0) and (animacion == true)) {
      setDisplayMeasure();
      display.println("Midiendo\ntemperatura . .");
      display.drawBitmap(110, 15,  logo16_temp_1, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      animacion = !animacion;
    }
    else if ((((i % 5) == 0) or (i == (TEMP_MUESTRAS - 1)) ) and (animacion == false)) {
      setDisplayMeasure();
      display.println("Midiendo\ntemperatura . . .");
      display.drawBitmap(110, 15,  logo16_temp_2, 16, 16, WHITE);
      display.display();
      display.clearDisplay();
      animacion = !animacion;
    }

    temp[i] = mlx.readObjectTempC();
    //Serial.print("Temp Array = "); Serial.println(temp[i]);

    tempResult = tempResult + temp[i];

    if (i == (TEMP_MUESTRAS - 1)) {
      tempResult = (tempResult / TEMP_MUESTRAS);
      //Serial.print("Temp Result = "); Serial.println(tempResult);
      //Serial.println();
    }

    delay(100);
  }
  animacion = true;

  setDisplayResult();
  display.print(tempResult);
  display.print((char)247); //degree symbol
  display.println("C");
  display.drawBitmap(110, 15,  logo16_temp_2, 16, 16, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

}



void mideO2() {
  for (int i = 0; i <= 5; i++) {
    setDisplayMeasure();
    display.println("Midiendo oxigeno\nen sangre . .");
    display.drawBitmap(110, 15,  logo16_o2_1, 16, 16, WHITE);
    display.display();
    delay(500);
    setDisplayMeasure();
    display.println("Midiendo oxigeno\nen sangre . . .");
    display.drawBitmap(110, 15,  logo16_o2_2, 16, 16, WHITE);
    display.display();
    delay(500);
    display.clearDisplay();
  }
  setDisplayResult();
  oxygenResult = 92;
  display.print(oxygenResult);
  display.setTextSize(1);
  display.println("%");
  display.drawBitmap(110, 15,  logo16_o2_2, 16, 16, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
}


void showResults(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Pulso: ");
  display.print(pulseResult);
  display.println(" lpm");
  display.print("Temperatura: ");
  display.print(tempResult);
  display.print((char)247); //degree symbol
  display.println("C");
  display.print("O2 en sangre: ");
  display.print(oxygenResult);
  display.println("%");
  display.display();
  delay(5000);
  display.clearDisplay();
}



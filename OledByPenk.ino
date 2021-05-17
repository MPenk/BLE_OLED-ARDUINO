#include <SoftwareSerial.h>
#define ARDUINOJSON_ENABLE_STD_STREAM 0
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include "ssd1331.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#define WIDTH      96
#define HEIGHT     64
#define PAGES       8
#define mw 96
#define mh 64
#define OLED_RST    9 
#define OLED_DC     8
#define OLED_CS    10
#define SPI_MOSI   11    /* connect to the DIN pin of OLED */
#define SPI_SCK    13     /* connect to the CLK pin of OLED */
#define show endWrite
#define clear() fillScreen(0)
SoftwareSerial mySerial(2,3);
StaticJsonDocument<100> doc;
char char_array[17];
int char_iterator = 0;
bool readingMess = false;
int lastX;
int lastY;
uint16_t color;
int takenData[2];
Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, OLED_CS, OLED_DC, OLED_RST);

#define LED_BLACK    0

#define LED_RED_VERYLOW   (3 <<  11)
#define LED_RED_LOW     (7 <<  11)
#define LED_RED_MEDIUM    (15 << 11)
#define LED_RED_HIGH    (31 << 11)

#define LED_GREEN_VERYLOW (1 <<  5)   
#define LED_GREEN_LOW     (15 << 5)  
#define LED_GREEN_MEDIUM  (31 << 5)  
#define LED_GREEN_HIGH    (63 << 5)  

#define LED_BLUE_VERYLOW  3
#define LED_BLUE_LOW    7
#define LED_BLUE_MEDIUM   15
#define LED_BLUE_HIGH     31

#define LED_ORANGE_VERYLOW  (LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW    (LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM (LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH   (LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW  (LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW    (LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM (LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH   (LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW  (LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW    (LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM   (LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH   (LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW   (LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM  (LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH    (LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)
void setup()
{
  mySerial.begin(9600);
  Serial.begin(115200);
  Serial.print("OLED Example\n");
   display.begin();
   display.clear();
   color = 63519;
}
void aa(){
  Serial.print("\nTablica:\n");
  for (int i = 0; i <= sizeof(char_array); i++) {
    Serial.print(char_array[i]);
  }
  Serial.print("\n0123456789\n");
}
void parseCharArrayToDo()
{
  DeserializationError error = deserializeJson(doc, char_array);
  if (error) 
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
      
  const char* type = doc["type"]; // "d"
  if(type[0] == 'd')
      {
          int startX = doc["1x"]; // 2000
          int startY = doc["1y"]; // 2000
          int stopX = doc["2x"]; // 2000
          int stopY = doc["2y"]; // 2000
          display.drawLine(startX,startY, stopX, stopY, 0x001F);    
          display.show();
          Serial.println("Narysowano");
      }
    if(type[0] == 'c')
      {
          display.clear();
          Serial.println("Wyczyszczono");
      }
      else
      {
        Serial.println("To nie D");
      }
}
void nextMessage(){

  char_iterator =  -1;
  readingMess = false;
  
}

void takeData(){
  int startInt = char_iterator;
  int comma;
  char value;
  do{
    if (mySerial.available())
    {
      char_iterator++;
      value = mySerial.read();
      //Serial.print(value);
      char_array[char_iterator] = value;
      if(value == ',') comma = char_iterator;
    }
    
  }while(value!='}');
  char buff[3];
  //char buffY[2];
  int plus = 1;
  for(int i = 0; i<comma-startInt-1; i++){
    buff[i] = char_array[startInt+plus];
    Serial.print(""+ String(buff[i]) + "");
    plus++;
  }
  //buff[0] = char_array[startInt+1];
  //buff[1] = char_array[startInt+2];
  Serial.println("\n drugi");
  takenData[0] = atoi(buff);
  for(int i = 0; i<3; i++){
    plus++;
    buff[i] = char_array[startInt+plus];
    Serial.print(""+ String(buff[i]) + "");
    
  }
  //buff[0] = char_array[startInt+4];
  //buff[1] = char_array[startInt+5];
   // if(takenData<10){
    //buff[0] = char_array[startInt+3];
   // buff[1] = char_array[startInt+4];
  // }
  takenData[1] = atoi(buff);
  Serial.println("\n Koniec pobierania");
  //Serial.println("" + String(takenData[0]) + "," + String(takenData[1])+"");
}

void down(){
  takeData();
  lastX = takenData[0];
  lastY = takenData[1];
  Serial.println("" + String(lastX) + "," + String(lastY)+"");
}
void point(){
  takeData();
  int x = takenData[0];
  int y = takenData[1];
  
  display.drawLine(x,y, lastX, lastY, color);    
  display.show();
  Serial.println("" + String(lastX)+"," + String(lastY) + " -> "+ String(x)+"," + String(y)+" / color ->" +String(color)+"");
  
  lastX = x;
  lastY = y;
  
}
void changeColor(){
  takeData();
  Serial.println("" + String(takenData[1]) + "," + String(takenData[0])+"");
  color = ( takenData[0] << 8) | takenData[1];
}
void drawText(){
  String text = "";
  int i=0;
  char value;
  value = ' ';
  Serial.println("");
  while(value!='}'){
    if (mySerial.available())
    {
      
      value = mySerial.read();
      //Serial.print(value);
      text += value;
      Serial.print("" + String(value) +"");
      i++;
    }
  
  }
    text[i-1] = ' ';
    display.setTextWrap(false);
    display.setTextSize(1);
    display.setCursor(lastX,lastY);
    display.setTextColor(color);
    display.print(text);
    display.show();
  
}
void loop()
{
  if (mySerial.available())
  {
      char i = mySerial.read();
      //Serial.print(i);
      if(i=='{')
      {
        readingMess=true;
        //Serial.println("Zaczynam czytać wiadomości");
      }
      if(readingMess==true)
      {
        char_array[char_iterator] = i;
        //Serial.println("- " + String(char_array[char_iterator-1]));
      
      if(i==';'){
        switch (char_array[char_iterator-1]) {
          case 'c':
            display.clear();
            Serial.println("Wyczyszczono");
            break;
          case 'd':
            //Serial.println("Robienie D\n");
            down();
            //Serial.println("\nZroboiono D\n");
            break;
          case 'p':
            point();
            break;
          case 'k':
            changeColor();
            break;
          case 't':
            drawText();
            break;
          default:
            // statements
            break;
        }
        nextMessage();
      }
      char_iterator++;  
    }
  }
}

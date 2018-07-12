#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  8
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int screenNumber = 0;


//This is for the pedometer
#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include "SPI.h"
LIS3DH myIMU; //Default constructor is I2C, addr 0x19.
unsigned long steps = 0;


//This is for the time
#include <RTCZero.h>
RTCZero rtc;
/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 7;
const byte hours = 10;

/* Change these values to set the current initial date */
const byte day = 15;
const byte month = 6;
const byte year = 15;


int xaxis = 0;

void setup()   {                
  SerialUSB.begin(9600);


  //this is for the display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(2000);
  display.clearDisplay();


  //this is for the time
  rtc.begin();
  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);


  //this is for the step counter / pedometer
  myIMU.begin();
}

void loop()
{
  if(myIMU.readFloatAccelX() > 1.2)
  {
    steps++;
    delay(500);
  }
  
  if(digitalRead(7))
  {
    screenNumber++;
    delay(250);
    if(screenNumber > 4)
    {
      screenNumber = 0;
    }
  }
  
  if(screenNumber == 0)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Hr: ");
    display.println(rtc.getHours());
    display.print("Min: ");
    display.println(rtc.getMinutes());
    display.print("Sec: ");
    display.println(rtc.getSeconds());
    display.display();
  }
  if(screenNumber == 1)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("STEPS:");
    display.println(steps);
    display.display();
  }
  if(screenNumber == 2)
  {
    if(axis == 0)
    {
      display.clearDisplay();
    }
    int photo = analogRead(A4);
    photo = map(photo, 0, 1023, 64, 0);
    display.drawPixel(photo,xaxis, WHITE); 
    display.display();
    
    xaxis++;
    if(xaxis > 128)
    {
      xaxis = 0;
    }
  }
  if(screenNumber == 3)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("This is the fourth screen");
    display.display();
  }
}


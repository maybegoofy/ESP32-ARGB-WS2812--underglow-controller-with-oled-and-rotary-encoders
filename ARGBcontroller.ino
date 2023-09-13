#include <WS2812FX.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h> 

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define LED_COUNT 150
#define LED_PIN1 15 //front strip
#define LED_PIN2 0 //skirt strip 1
#define LED_PIN3 2 //skirt strip 2
#define LED_PIN4 4 //interior strip
#define butt_PINb 14
#define butt_PINg 5
//#define butt_PINr 14

#define rPIN1 27 //dt
#define rPIN2 26 //clk
#define gPIN1 16 //dt
#define gPIN2 17 //clk
#define bPIN1 18 //dt
#define bPIN2 19 //clk

int rStart = 77;
int gStart = 0;
int bStart = 255;

volatile int rVAL = 77;
volatile int gVAL = 0;
volatile int bVAL = 255;

volatile int curMode = 0;
volatile int curCol = 0;
volatile int cusCol = 1;

#define TIMER_MS 50

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

WS2812FX str1 = WS2812FX(30, LED_PIN1, NEO_GRB + NEO_KHZ800);
WS2812FX str2 = WS2812FX(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);
WS2812FX str3 = WS2812FX(LED_COUNT, LED_PIN3, NEO_GRB + NEO_KHZ800);
WS2812FX str4 = WS2812FX(LED_COUNT, LED_PIN4, NEO_GRB + NEO_KHZ800);

unsigned long last_change = 0;
unsigned long now = 0;


static unsigned long lastInterruptTime = 0;

void isrR(){
  if (digitalRead(rPIN2) == LOW) {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    if (interruptTime - lastInterruptTime > 5) {
      if (digitalRead(rPIN1) == LOW)
      {
        rVAL-- ;
      }
      else  { //if (digitalRead(rPIN1) == HIGH)
        rVAL++ ;
      }
      rVAL = constrain(rVAL,0,255);
      
      lastInterruptTime = interruptTime;
      str1.setColor(rVAL,gVAL,bVAL);
      str2.setColor(rVAL,gVAL,bVAL);
      str3.setColor(rVAL,gVAL,bVAL);
      str4.setColor(rVAL,gVAL,bVAL);
      cusCol = 1;
      }
  }
}
void isrG(){
  if (digitalRead(gPIN2) == LOW) {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    if (interruptTime - lastInterruptTime > 5) {
      if (digitalRead(gPIN1) == LOW)
      {
        gVAL-- ;
      }

      else  { //if (digitalRead(gPIN1) == HIGH)
        gVAL++ ;
      }

      gVAL = constrain(gVAL,0,255);
      
      lastInterruptTime = interruptTime;
      str1.setColor(rVAL,gVAL,bVAL);
      str2.setColor(rVAL,gVAL,bVAL);
      str3.setColor(rVAL,gVAL,bVAL);
      str4.setColor(rVAL,gVAL,bVAL);
      cusCol = 1;
    }
  }
}
void isrB(){
  if (digitalRead(bPIN2) == LOW) {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    if (interruptTime - lastInterruptTime > 5) {
      lastInterruptTime = interruptTime;
      if (digitalRead(bPIN1) == LOW)
      {
        bVAL-- ;
      }
      else  { //if (digitalRead(bPIN1) == HIGH)
        bVAL++ ;
      }

      bVAL = constrain(bVAL,0,255);
      
      lastInterruptTime = interruptTime;
      str1.setColor(rVAL,gVAL,bVAL);
      str2.setColor(rVAL,gVAL,bVAL);
      str3.setColor(rVAL,gVAL,bVAL);
      str4.setColor(rVAL,gVAL,bVAL);
      cusCol = 1;
    }
  }
}

void buttPressG() {
  if (digitalRead(butt_PINg) == LOW) {
    curCol++;
    curCol = curCol % 6;
    //str1.setColor(0,0,255);
    //delay(1000);
    //str1.setColor(rVAL,gVAL,bVAL);
    if (curCol == 0) { //Purple
      rVAL = 77;
      gVAL = 0;
      bVAL = 255;
    }
    else if (curCol == 1) {//red
      rVAL = 255;
      gVAL = 0;
      bVAL = 0;
    }
    else if (curCol == 2) {//green
      rVAL = 0;
      gVAL = 255;
      bVAL = 0;
    }
    else if (curCol == 3) {//blue
      rVAL = 0;
      gVAL = 0;
      bVAL = 255;
    }
    else if (curCol == 4) {//other purple
      rVAL = 123;
      gVAL = 0;
      bVAL = 255;
    }
    else if (curCol == 5) {//pink
      rVAL = 255;
      gVAL = 0;
      bVAL = 215;
    }
    else if (curCol == 6) {//yellow
      rVAL = 255;
      gVAL = 255;
      bVAL = 0;
    }
    else if (curCol == 7) {//light blue
      rVAL = 0;
      gVAL = 255;
      bVAL = 255;
    }
    else if (curCol == 8) {//white
      rVAL = 255;
      gVAL = 255;
      bVAL = 255;
    }
    else if (curCol == 9) {//orange
      rVAL = 255;
      gVAL = 119;
      bVAL = 0;
    }
    str1.setColor(rVAL,gVAL,bVAL);
    str2.setColor(rVAL,gVAL,bVAL);
    str3.setColor(rVAL,gVAL,bVAL);
    str4.setColor(rVAL,gVAL,bVAL);
    cusCol = 0;
  }
  else {
    //str1.setColor(255,0,0);
    //delay(1000);
    //str1.setColor(rVAL,gVAL,bVAL);
  }
}

void buttPressB() {
 if (digitalRead(butt_PINb) == LOW) {
    curMode++;
    curMode = curMode % 6;
    //str1.setColor(0,0,255);
    //delay(1000);
    //str1.setColor(rVAL,gVAL,bVAL);
    if (curMode == 0) { //"Static Cruise"
      str1.setColor(rVAL,gVAL,bVAL);
      str2.setColor(rVAL,gVAL,bVAL);
      str3.setColor(rVAL,gVAL,bVAL);
      str4.setColor(rVAL,gVAL,bVAL);

      str1.setMode(FX_MODE_STATIC);
      str2.setMode(FX_MODE_STATIC);
      str3.setMode(FX_MODE_STATIC);
      str4.setMode(FX_MODE_STATIC);
    }
    else if (curMode == 1) {//"Breathe"
      str1.setSpeed(1000);
      str2.setSpeed(1000);
      str3.setSpeed(1000);
      str4.setSpeed(1000);

      str1.setColor(rVAL,gVAL,bVAL);
      str2.setColor(rVAL,gVAL,bVAL);
      str3.setColor(rVAL,gVAL,bVAL);
      str4.setColor(rVAL,gVAL,bVAL);

      str1.setMode(FX_MODE_BREATH);
      str2.setMode(FX_MODE_BREATH);
      str3.setMode(FX_MODE_BREATH);
      str4.setMode(FX_MODE_BREATH);
    }
    else if (curMode == 2) {//knight rider
      str1.setColor(rVAL,gVAL,bVAL);
      str2.setColor(0,0,0);
      str3.setColor(0,0,0);
      str4.setColor(0,0,0);

      str1.setSpeed(500);
      str2.setSpeed(1000);
      str3.setSpeed(1000);
      str4.setSpeed(1000);


      str1.setMode(FX_MODE_LARSON_SCANNER);
      str2.setMode(FX_MODE_BREATH);
      str3.setMode(FX_MODE_BREATH);
      str4.setMode(FX_MODE_BREATH);
    }
    else if (curMode == 3) {//star power
      str1.setColor(rVAL,gVAL,bVAL);
      str2.setColor(rVAL,gVAL,bVAL);
      str3.setColor(rVAL,gVAL,bVAL);
      str4.setColor(rVAL,gVAL,bVAL);
      
      str1.setSpeed(1000);
      str2.setSpeed(1000);
      str3.setSpeed(1000);
      str4.setSpeed(1000);

      str1.setMode(FX_MODE_RAINBOW_CYCLE);
      str2.setMode(FX_MODE_RAINBOW_CYCLE);
      str3.setMode(FX_MODE_RAINBOW_CYCLE);
      str4.setMode(FX_MODE_RAINBOW_CYCLE);
    }
    else if (curMode == 4) {//kylo crackle
      str1.setColor(rVAL,gVAL,bVAL);
      str2.setColor(rVAL,gVAL,bVAL);
      str3.setColor(rVAL,gVAL,bVAL);
      str4.setColor(rVAL,gVAL,bVAL);
      
      str1.setSpeed(100);
      str2.setSpeed(100);
      str3.setSpeed(100);
      str4.setSpeed(100);


      str1.setMode(FX_MODE_FIRE_FLICKER);
      str2.setMode(FX_MODE_FIRE_FLICKER);
      str3.setMode(FX_MODE_FIRE_FLICKER);
      str4.setMode(FX_MODE_FIRE_FLICKER);
    }
    else if (curMode == 5) {//Twinkle
      str1.setColor(rVAL,gVAL,bVAL);
      str2.setColor(rVAL,gVAL,bVAL);
      str3.setColor(rVAL,gVAL,bVAL);
      str4.setColor(rVAL,gVAL,bVAL);
      
      str1.setMode(FX_MODE_TWINKLEFOX);
      str2.setMode(FX_MODE_TWINKLEFOX);
      str3.setMode(FX_MODE_TWINKLEFOX);
      str4.setMode(FX_MODE_TWINKLEFOX);
    }
  }
  else {
    //str1.setColor(255,0,0);
    delay(1000);
    //str1.setColor(rVAL,gVAL,bVAL);
  }
}
void refreshScreen() {
  char* mOdes[6] = {"Static Cruise","Breathe","Knight Rider","Star Power","Kylo Crackle","Twinkle"};
  char* colName[10] = {"Purple","Red","Green","Blue","Purple2","Pink","Yellow","Light Blue","White","Orange"};

  oled.clearDisplay();

  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(10, 0);       // set position to display
  oled.println("RED"); // set text
  //oled.display();              // display on OLED

  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(45, 0);       // set position to display
  oled.println("GREEN"); // set text
  //oled.display();              // display on OLED

  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(90, 0);       // set position to display
  oled.println("BLUE"); // set text
  //oled.display();

  oled.setTextSize(2);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(85, 20);       // set position to display
  oled.println(bVAL); // set text
  //oled.display();

  oled.setTextSize(2);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(45, 20);       // set position to display
  oled.println(gVAL); // set text
  //oled.display();

  oled.setTextSize(2);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(5, 20);       // set position to display
  oled.println(rVAL); // set text
  //oled.display();

  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(10, 50);       // set position to display
  oled.println(mOdes[curMode]); // set text
  //oled.display();

  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(90, 50);       // set position to display
  if (cusCol == 0) {
    oled.println(colName[curCol]); // set text
  }
  else if (cusCol == 1) {
    oled.println("Custom"); // set text
  }
  
  oled.display();
}
void setup() {
  pinMode(rPIN1, INPUT);
  pinMode(rPIN2, INPUT);
  pinMode(gPIN1, INPUT);
  pinMode(gPIN2, INPUT);
  pinMode(bPIN1, INPUT);
  pinMode(bPIN2, INPUT);
  pinMode(butt_PINb, INPUT);
  
  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  //attachInterrupt(digitalPinToInterrupt(rPIN1), isrR, LOW);
  attachInterrupt(digitalPinToInterrupt(rPIN2), isrR, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(gPIN1), isrG, LOW);
  attachInterrupt(digitalPinToInterrupt(gPIN2), isrG, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(bPIN1), isrB, LOW);
  attachInterrupt(digitalPinToInterrupt(bPIN2), isrB, CHANGE);

  attachInterrupt(butt_PINb, buttPressB , CHANGE);
  attachInterrupt(butt_PINg, buttPressG , CHANGE);

  delay(2000);
  

  str1.init();
  str1.setBrightness(255);
  str1.setSpeed(1000);
  str1.setColor(rVAL,gVAL,bVAL);
  str1.setMode(FX_MODE_STATIC);
  str1.start();
  str2.init();
  str2.setBrightness(255);
  str2.setSpeed(1000);
  str2.setColor(rVAL,gVAL,bVAL);
  str2.setMode(FX_MODE_STATIC);
  str2.start();
  str3.init();
  str3.setBrightness(255);
  str3.setSpeed(1000);
  str3.setColor(rVAL,gVAL,bVAL);
  str3.setMode(FX_MODE_STATIC);
  str3.start();
  str4.init();
  str4.setBrightness(255);
  str4.setSpeed(1000);
  str4.setColor(rVAL,gVAL,bVAL);
  str4.setMode(FX_MODE_STATIC);
  str4.start();

  refreshScreen();
  
}

void loop() {
  now = millis();
  
  str1.service();
  str2.service();
  str3.service();
  str4.service();

  

  if(now - last_change > TIMER_MS) {
    //str1.setMode((str1.getMode() + 0) % str1.getModeCount());
    //str2.setMode((str2.getMode() + 0) % str2.getModeCount());
    //str3.setMode((str3.getMode() + 0) % str3.getModeCount());
    //str4.setMode((str4.getMode() + 0) % str4.getModeCount());
    last_change = now;
    
    refreshScreen();

  }
}

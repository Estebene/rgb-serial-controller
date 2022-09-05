#include <EEPROM.h>

int bluePin = 6;
int greenPin = 3;
int redPin = 5;

int wait = 10;
const int address = 0;
int mode = 0;
bool isOpen = false;
unsigned long lastUpdate = 0;

typedef struct {
    int red;
    int green;
    int blue;
} rgb_t;

#define YELLOW (rgb_t){255, 40, 0}
#define BLUE (rgb_t){0, 0, 255}
#define RED (rgb_t){255, 0, 0}
#define GREEN (rgb_t){0, 255, 0}
#define WHITE (rgb_t){255, 140, 90}
#define ORANGE (rgb_t){255, 20, 2}
#define PURPLE (rgb_t){255, 0, 255}

#define VALUE_MAX 255
#define VALUE_MIN 0

#define COLORNUM 7
const rgb_t possibleColors[COLORNUM] = {YELLOW, BLUE, RED, GREEN, WHITE, ORANGE, PURPLE};

rgb_t strobeTarget = WHITE;

rgb_t rgb = {0, 0, 0};

void update(rgb_t* ptr) {
  analogWrite(redPin, 255 - ptr->red);
  analogWrite(greenPin, 255 - ptr->green);
  analogWrite(bluePin, 255 - ptr->blue);
}

void setRGB(rgb_t* ptr, rgb_t newrgb) {
  ptr->blue = newrgb.blue;
  ptr->green = newrgb.green;
  ptr->red = newrgb.red;
}

void adjustRGB(rgb_t* ptr, int mode) { 
  switch (mode) {
  case 'y':
    setRGB(ptr, YELLOW);
    break;
  case 'o':
    setRGB(ptr, ORANGE);
    break;
  case 'r':
    setRGB(ptr, RED);
    break; 
  case 'g':
    setRGB(ptr, GREEN);
    break;
  case 'b':
    setRGB(ptr, BLUE);
    break;
  case 'p':
    setRGB(ptr, PURPLE);
    break;
  case 'w':
    setRGB(ptr, WHITE);
    break;
  case 's':
    if (isrgbEqual(ptr, &strobeTarget)) {
//      int ran = possibleColors[random(COLORNUM)].red;
//      Serial.println(ran);
      strobeTarget = possibleColors[random(COLORNUM)];
    } else {
      rgbStep(ptr, &strobeTarget);
//      printRgb(ptr);
    }
    break;
  }
}

void updateMode(int* ptr, int newMode, int adr) {
  *ptr = newMode;
  EEPROM.write(adr, *ptr);
}

void updateWait(int* ptr, int adr) {
  if (*ptr > 10000) {
    *ptr = 10000;
  } else {
    EEPROM.write(adr, *ptr);
    Serial.println("Speed: " + String(10000/(wait + 1)));
  }
}

bool isrgbEqual(rgb_t* rgb1, rgb_t* rgb2) {
  bool equal = true;
  if (rgb1->blue != rgb2->blue) { equal = false; }
  if (rgb1->red != rgb2->red) { equal = false; }
  if (rgb1->green != rgb2->green) { equal = false; }
  return equal;
}

//void printRgb(rgb_t* currRgb) {
//  Serial.print("rgb: ");
//  Serial.print(currRgb->blue);
//  Serial.print(currRgb->red);
//  Serial.println(currRgb->green);
//}

void rgbStep(rgb_t* currentRgb, rgb_t* targetRgb) {
  colorStep(&(currentRgb->red), targetRgb->red);
  colorStep(&(currentRgb->green), targetRgb->green);
  colorStep(&(currentRgb->blue), targetRgb->blue);
}

void colorStep(int* color, int target) {
//  Serial.println(target);
  if (*color != target) {
    if (abs(*color - target) > 5 && wait < 100) {
      *color += *color > target ? -5 : 5;
    } else {
      *color += *color > target ? -1 : 1;
    }
    
  }
  if (*color < VALUE_MIN) *color = VALUE_MIN;
  if (*color > VALUE_MAX) *color = VALUE_MAX;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(bluePin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(redPin, OUTPUT); 
  mode = EEPROM.read(address);
  wait = EEPROM.read(address + sizeof(int));
  Serial.begin(9600);
}

void loop() {
  //Instructions
  if (Serial && !isOpen) {
    delay(100);
    isOpen = true;
    Serial.println("Welcome to RGB Utility V1.0");
    Serial.println("---------------------------");
    Serial.println("Colors: y, o, g, r, p, b, w");
    Serial.println("Modes: s");
    Serial.println("Commands (for s): u, d");
  } else if (isOpen && !Serial) {
    isOpen = false;
  }
  
  if (Serial.available() > 0 && Serial) {
    char newMode = Serial.read();
    if (newMode >= 'a') {
      if (newMode == 'u') {
        wait -= ceil(wait / 2);
        updateWait(&wait, address + sizeof(int));
      } else if ( newMode == 'd' ) {
        wait += wait + 1;
        updateWait(&wait, address + sizeof(int));
      } else {
        updateMode(&mode, newMode, address);
      }
    }
  }

  if (lastUpdate < (millis() - wait)) {
    lastUpdate = millis();
    adjustRGB(&rgb, mode);
    update(&rgb);
  }
}

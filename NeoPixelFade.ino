#include <Adafruit_NeoPixel.h>

int Power = 11;
int PIN  = 12;
#define NUMPIXELS 1
#define MIN_RGB 0
#define MAX_RGB 255
#define MIN_STEP 1
#define MAX_STEP 5
#define DELAY_MILLIS 400
#define CHANCE_STEP_CHANGE 10
#define CHANCE_STEP_FLIP 15

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int red;
int green;
int blue;

int redStep;
int greenStep;
int blueStep;

bool coinFlip(const int sides) {
  return random(sides) == 0;
}

int nextStep(int val) {
  int sgn = val < 0?-1:1;  //preserve direction
  if(coinFlip(CHANCE_STEP_CHANGE)) {
    val = random(MIN_STEP,MAX_STEP) * sgn;
    if(coinFlip(CHANCE_STEP_FLIP)) {
      val = -val;
    }
  }
  return val;
}

void updateSteps() {
  redStep = nextStep(redStep);
  greenStep = nextStep(greenStep);
  blueStep = nextStep(blueStep);
}

void updateColors() {
  if(red + redStep > MAX_RGB || red + redStep < MIN_RGB) redStep = -redStep;
  if(blue + blueStep > MAX_RGB || blue + blueStep  < MIN_RGB) blueStep = -blueStep;
  if(green + greenStep > MAX_RGB || green + greenStep  < MIN_RGB) greenStep = -greenStep;
  red += redStep;
  green += greenStep;
  blue += blueStep;
}

void printInfo() {
  char msg[250];  //Time backwards since naming stuff time causes compiler issues
  sprintf(msg, "Red: %03d, %02d Green: %03d, %02d Blue: %03d, %02d\n", red, redStep, green, greenStep, blue, blueStep);
  Serial.print(msg);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("\nStarting NeoPixel Fader...");
  randomSeed(analogRead(0));
  red = random(MIN_RGB,MAX_RGB + 1);
  green = random(MIN_RGB,MAX_RGB + 1);
  blue = random(MIN_RGB,MAX_RGB + 1);
  redStep = random(MIN_STEP, MAX_STEP + 1);
  if(coinFlip(2)) redStep = -redStep;
  greenStep = random(MIN_STEP, MAX_STEP + 1);
  if(coinFlip(2)) greenStep = -greenStep;
  blueStep = random(MIN_STEP, MAX_STEP + 1);
  if(coinFlip(2)) blueStep = -blueStep;
  pixels.begin();
  pinMode(Power,OUTPUT);
  digitalWrite(Power, HIGH);
  pixels.clear();
}

void loop() { 
  // update colors & steps
  updateSteps();
  updateColors();
  //printInfo();
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(red,green,blue)); 
  pixels.show();
  delay(DELAY_MILLIS);
}
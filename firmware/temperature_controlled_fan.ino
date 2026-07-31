// Library Installation // 
#include <LiquidCrystal.h>

// LCD Pin Variables //
const int RS = 3;
const int E = 4;
const int D4 = 5;
const int D5 = 7;
const int D6 = 8;
const int D7 = 12;

// Enabling LiquidCrystal // 
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

// Hardware / Pin Variable Settings //
const int buttonPin = 2; 
const int buttonPin2 = 13; 
const int RGBred = 9; 
const int RGBgreen = 10; 
const int RGBblue = 6; 
const int motorPin = 11; 
const int tempSensor = A0; 
const int potPin = A4; 

// Motor Speed Variables //
const int fspinSpeed = 255; 
const int mspinSpeed = 180;
const int lspinSpeed = 80; 
const int xspinSpeed = 0; 

// LED Brightness Variables //
const int Fbrightness = 255; 

// Input / Output Variables //
int buttonRead; 
int buttonRead2; 
int readValue; 
float Volts; 
int tempConvert; 
int Temperature; 
int potRead; 
int potWrite;
String myMode;

// Mode Variables //
bool manualMode = true;
int speedPercent; 


void setup() {
  // Serial Communication Initialisation //
  Serial.begin(9600); 

  // Output Device Initialisation // 
  pinMode(motorPin, OUTPUT); 

  // Sensor Initialisation // 
  pinMode(tempSensor, INPUT); 

  // RGB LED Initialisation // 
  pinMode(RGBred, OUTPUT); 
  pinMode(RGBgreen, OUTPUT);
  pinMode(RGBblue, OUTPUT); 

  // Button Initialisation // 
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(buttonPin2, INPUT_PULLUP); 

  // Potentiometer Initilisation 
  pinMode(potPin, INPUT); 

  // LCD Initilisation //
  lcd.begin(16,2);
}

void loop() {
  // Start-Up Screen //
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Auto or Manual?");
  lcd.setCursor(0, 1);
  lcd.print("Pick :)");

  // System Idle RGBLED Colour //
  analogWrite(RGBgreen, 255);
  analogWrite(RGBblue, 255);
  analogWrite(RGBred, 255);

  // User Mode Selection // 
  Serial.println("Which mode would you would you like (Automatic? / Manual?)"); // Ask User for Input
  while(Serial.available()==0) { }
  myMode = Serial.readString(); // Gets User Input 
  myMode.trim();

  // Mode Selection Logic // 
  if(myMode == "Automatic" || myMode == "automatic" || myMode == "Auto" || myMode == "auto" ) {
    manualMode = false;
  }

  
  else if(myMode == "Manual" || myMode == "manual" || myMode == "Man" || myMode == "man") {
      manualMode = true;
  }
  while(true){
  // Button State Reading //
  buttonRead = digitalRead(buttonPin);
  buttonRead2 = digitalRead(buttonPin2);
  
  // Mode Toggle Button // 
  if(buttonRead == LOW) {
    manualMode = !manualMode;
    delay(300);
  }
  
  // System Reset / Exit Button //
  if(buttonRead2 == LOW) {
    analogWrite(motorPin, 0);
    analogWrite(RGBgreen, 255);
    analogWrite(RGBblue, 255);
    analogWrite(RGBred, 255);
    delay(300);
    break;
  }
  // Manual Mode Operation // 
  if(manualMode){

    // Potentiometer to PWM Conversion // 
    buttonRead = digitalRead(buttonPin);
      potRead = analogRead(potPin); 
      potWrite = map(potRead, 0, 1023, 0, 255); 

      // PWM to Percentage Conversion //
      speedPercent = map(potWrite, 0, 255, 0, 100); 

      // Motor Control // 
      analogWrite(motorPin, potWrite); 

      // Manual Mode RGB Indicator //
      analogWrite(RGBgreen, 255 - potWrite);
      analogWrite(RGBblue, 255);
      analogWrite(RGBred, 255);

      // Manual Mode LCD Display //
      lcd.setCursor(0,0);
      lcd.print("(Manual Mode)");
      lcd.print("    ");
      Serial.print("Potentiometer Read Value: ");
      lcd.setCursor(0,1);
      lcd.print("Speed: ");
      lcd.print(speedPercent);
      lcd.print("%");
      lcd.print("        ");
      // Serial Monitor Debug Output // 
      Serial.println(potRead);
      Serial.println(" ");
      delay(250);
  }
  // Automatic Mode Conversion //
  else{
  // Temperature Sensor Conversion // 
  readValue = analogRead(tempSensor);

  Volts = readValue * (5.0 / 1023.0);

  float temperatureC = Volts * 100.0;

  // Automatic Mode LCD Display //
  lcd.setCursor(0, 0);
  lcd.print("(Automatic Mode)");
  lcd.setCursor(0, 1);
  lcd.print("Degrees (C): ");
  lcd.print(temperatureC, 1);
  lcd.print("   ");

  // Serial Monitor Temperature Output // 
  Serial.print("Raw ADC: ");
  Serial.println(readValue);

  Serial.print("Voltage: ");
  Serial.println(Volts, 3);

  Serial.print("Temperature: ");
  Serial.println(temperatureC, 1);

  delay(1000);

  // Temperature Control Logic //

  // Cool Temperature Range // 
  if(temperatureC < 20.) { 
    analogWrite(RGBred, 0);
    analogWrite(RGBgreen, 0);
    analogWrite(RGBblue, Fbrightness);
    analogWrite(motorPin, lspinSpeed);

  }
  // Warm Temperature Range //
  else if(temperatureC >= 20. && temperatureC <= 28. ) {
    analogWrite(RGBred, Fbrightness);
    analogWrite(RGBgreen, 40);
    analogWrite(RGBblue, 0);
    analogWrite(motorPin, mspinSpeed);

  }
  // Hot Temperature Range // 
  else{
    analogWrite(RGBred, Fbrightness);
    analogWrite(RGBgreen, 0);
    analogWrite(RGBblue, 0);
    analogWrite(motorPin, fspinSpeed);
  }
  
  }

  
  }
  }


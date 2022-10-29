#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(-1);
#include <SoftwareSerial.h>  // remove the inverted commas after you copy the code to the IDE
SoftwareSerial BT(10, 11);

float No_load_voltage = 0;
float No_load_speed = 0;
float load_voltage = 0;
float load_speed = 0;
//default value
float No_load_voltaged = 250;
float No_load_speedd = 1200;
float load_voltaged = 0;
float load_speedd = 0;

float a0;
float a1;
float a2;
float a3;
float a6;
float ra0;
float ra1;
float ra2;
float ra3;
int var = 1;
int button = 7;
String value;
int value_pot0;
int value_pot1;
int value_pot2;
int value_pot3;

//debounce variable
const int buttonPin = 2;
const int ledPin = 13;
const int ledPin1 = 4;
const int ledPin2 = 3;
int ledState = HIGH;
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 500; //for stable switch

int buttonPin1 = 7; //value and graph changing switch
int buttonPin2=6; //default value set switch
int ledState1 = 1;     //lcd graph or value (0 for first showing value)
int ledState2 = 1;
int buttonState1;      
int buttonState2;        
int lastButtonState1 = LOW;   
int lastButtonState2 = LOW;   
unsigned long lastDebounceTime1 = 0; 
unsigned long lastDebounceTime2 = 0; 
unsigned long debounceDelay1 = 50;    
unsigned long debounceDelay2 = 50;  

//delay variable code
unsigned long previousMillis = 0;
const long interval = 500;


void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(button, INPUT_PULLUP);
  //debounce setup
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
   pinMode(ledPin1, OUTPUT);
     pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin, ledState);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  
  //stability button code
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }
  digitalWrite(ledPin, ledState);
  lastButtonState = reading;


// value and graph changing code
  int reading1 = digitalRead(buttonPin1);
  if (reading1 != lastButtonState1) {
    // reset the debouncing timer
    lastDebounceTime1 = millis();
  }
  if ((millis() - lastDebounceTime1) > debounceDelay1) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == HIGH) {
        ledState1 = !ledState1;
      }
    }
  }
    digitalWrite(ledPin1, ledState1);
  lastButtonState1 = reading1;


// default button code
  int reading2 = digitalRead(buttonPin2);
  if (reading2 != lastButtonState2) {
    // reset the debouncing timer
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime2) > debounceDelay2) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == HIGH) {
        ledState2 = !ledState2;
      }
    }
  }
    digitalWrite(ledPin2, ledState2);
  lastButtonState2 = reading2;

  
  //data taking part site
  if (ledState == 0)
  {
    ra0 = analogRead(A0);
    ra1 = analogRead(A1);
    ra2 = analogRead(A2);
    ra3 = analogRead(A3);
  }

  a0 = ra0;
  a1 = ra1;
  a2 = ra2;
  a3 = ra3;
  a6 = analogRead(A6);
  No_load_speed = a2;

  //calculation part
  a0 = a0 * (5.0 / 1023.0);
  a1 = a1 * (100.0 / 1023.0);
  No_load_voltage = (a0 * 100) + a1;
  a2 = a2 * (20.0 / 1023.0);
  a3 = a3 * (100.0 / 1023.0);
  load_voltage = a6 * (500.0 / 1023.0);
  No_load_speed = (a2 * 100.0) + a3;
  load_speed = (load_voltage / No_load_voltage) * No_load_speed;

  //print part
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("no load voltage: ");
    Serial.print((int)No_load_voltage);
    Serial.println(" V");
    Serial.print("no load Speed: ");
    Serial.print((int)No_load_speed);
    Serial.println(" rpm");
    Serial.print("Load voltage: ");
    Serial.print((int)load_voltage);
    Serial.println(" V");
    Serial.print("Load Speed: ");
    Serial.print((int)load_speed);
    Serial.println(" rpm\n");

    value_pot0 = No_load_voltage;
    value_pot1 = No_load_speed;
    value_pot2 = load_voltage;
    value_pot3 = load_speed;
    //value = (String) value_pot0 + "," + (String) value_pot1 + "," + (String) value_pot2 + "," + (String) value_pot3;
    //    value=33,22,44,55;
    //    BT.println(value);


    BT.print(value_pot0); //send distance to MIT App
    BT.print(";");
    BT.print(value_pot1); //send distance to MIT App
    BT.print(";");//BT.println
    BT.print(value_pot2); //send distance to MIT App
    BT.print(";");
    BT.print(value_pot3); //send distance to MIT App
    BT.println(";");//BT.println

    switch (ledState1) {
      case 0:
        //olid value part
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(BLACK, WHITE);
        display.setCursor(90, 0);
        display.print(" VALUE");


        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("V0:");
        display.print((int)No_load_voltage);
        display.println("V");

        display.print("N0:");
        display.print((int)No_load_speed);
        display.println("rpm");

        display.print("V:");
        display.print((int)load_voltage);
        display.println("V");

        display.print("N:");
        display.print((int)load_speed);
        display.println("rpm");
        display.display();
        break;
      case 1:

        int a = map((int)load_voltage, 0, (int)No_load_voltage, 0, 125);
        int b = map((int)load_speed, 0, (int)No_load_speed, 0, 125);
        display.clearDisplay();

        display.setTextSize(1);
        display.setTextColor(BLACK, WHITE);
        display.setCursor(90, 0);
        display.print(" GRAPH");

        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("V:");
        display.print((int)load_voltage);
        display.println("V");

        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.drawRoundRect(0, 17, 125, 10, 8, WHITE);//x,y,w,h
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.fillRoundRect(0, 17, b , 10, 8, WHITE);

        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 33);
        display.print("N:");
        display.print((int)load_speed);
        display.println("rpm");

        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.drawRoundRect(0, 52, 125, 10, 8, WHITE);//x,y,w,h
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.fillRoundRect(0, 52, a , 10, 8, WHITE);
        display.display();
        break;
      default:
        break;
    }
  }
}

//ok

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Digital pin connected to the DHT sensor
#define DHTPIN 10
#define DHTTYPE DHT11
#define LDR A1        
#define lamp1 13
DHT dht(DHTPIN, DHTTYPE);
const int ROWS = 4; //four rows
const int COLS = 4; //four columns
int flame = 7;
int detectled = 9;
int pirPin = 8;
int motionDetected = 0;
int buzzer = 6;
int val = A0;
int value = 0;
int humtemp = 10;
int yellowpass = 38;
char correct[4] = {'1', '2', '3', '4'};
int pirValue ;
int redRGB = A4;
int blueRGB = A2;
int greenRGB = A3;
int lightsystem = 0;

int acled = 52;
char reading;
char passwordtemp[8];
int takeef;
int i;
int j;
int inhome = 0;
int flag = 0;
int flagpwmob = 0;
int motiondetected;
int inHouse = 0;
char password[4];
int passFlag = 0;
char temp;
int pos = 0;
float LDR_value=0;
float LED_value=0;
int threshold = 200;
int level;
float h, t, f, hif, hic;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {36, 34, 32, 30}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {28, 26, 24, 22}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void servoMotorOpen() {
  for (pos = 90; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }
}
void servoMotorClose() {
  for (pos = 180; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }
}
void brightChange(){
  LDR_value = analogRead(LDR);
  Serial.println(LDR_value);
    LED_value =255.0 - ((LDR_value * 255.0)/1000.0);
//    inputt = input_value / 700.0;
//    inpit = 255.0 - inputt;
if (lightsystem ==1)
{
  analogWrite(lamp1, LED_value);
    delay(500);
}
}

void passwordReciever()
{
  for (i = 0; i < 4 ; i++)
  {
    password[i] = keypad.getKey();
    while (!password[i]) {
      password[i] = keypad.getKey();
    }
    digitalWrite(buzzer,HIGH);
        delay(50);
            digitalWrite(buzzer,LOW);

    digitalWrite(yellowpass, HIGH);
    delay(300);
    digitalWrite(yellowpass, LOW);
  }
  for (i = 0; i < 4 ; i++)
  {
    if (password[i] == correct[i])
      flag++;
  }
  if (flag == 4) {
    lcd.setCursor(0, 0);
    //green light
    passFlag = 1;
    digitalWrite(buzzer, LOW);
    flag = 0;
    analogWrite(greenRGB,255);
    analogWrite(redRGB,0);
    analogWrite(blueRGB,0);
    servoMotorOpen();
    delay(5000);
    servoMotorClose();
    analogWrite(greenRGB, 0);
    analogWrite(redRGB,255);
    analogWrite(blueRGB,0);
    
  }
  else {
    lcd.setCursor(0, 0);
    analogWrite(greenRGB, 0);
    analogWrite(redRGB,255);
    analogWrite(blueRGB,0);
    lcd.print("password wrong");
    digitalWrite(buzzer, HIGH);
    flag = 0;
    passwordReciever();
  }
}
void humTemp() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  hif = dht.computeHeatIndex(f, h);
  hic = dht.computeHeatIndex(t, h, false);
  lcd.clear();
  lcd.print(t);
  lcd.setCursor(7, 0);
  lcd.print(h);
  lcd.setCursor(0, 1);
//  Serial.print(F(" Humidity: "));
//  Serial.print(h);
//  Serial.print(F("%  Temperature: "));
//  Serial.print(t);
//  Serial.print(F("C "));
//  Serial.print(f);
//  Serial.print(F("F  Heat index: "));
//  Serial.print(hic);
//  Serial.print(F("C "));
//  Serial.print(hif);
//  Serial.println(F("F"));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp=");
  lcd.setCursor(6, 0);
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity=");
  lcd.setCursor(10, 1);
  lcd.print(h);
}

void PIR() {
  pirValue = digitalRead(pirPin);
  Serial.print(pirValue);
  if (pirValue == 1)
  {
    lightsystem = 1;
    motiondetected = 1;
  }
  if (lightsystem == 1)
  {
    brightChange();
  }
}

void flameDetector() {
  value = analogRead(val);
  Serial.print("value of flame");
  Serial.println(value);
  delay(500);
  if (value <= 500)
  {
    digitalWrite(buzzer, HIGH);
  }
  else {
    digitalWrite(buzzer, LOW);
  }
}

void brightnessDetector() {
  level = analogRead(LDR);
  Serial.println(level);
  Serial.print("  LDR:  ");
  Serial.println(level);
  
}
void reset() {
    analogWrite(greenRGB, 0);
    analogWrite(redRGB,0);
    analogWrite(blueRGB,255);
  for (i = 0; i < 4; i++) {
    correct[i] = keypad.getKey();
    while (!correct[i]) {
      correct[i] = keypad.getKey();
    }
    digitalWrite(buzzer,HIGH);
    delay(50);
    digitalWrite(buzzer,LOW);
    digitalWrite(yellowpass, HIGH);
    delay(300);
    digitalWrite(yellowpass, LOW);
  }
}
void passwordMode()
{
  temp = keypad.getKey();
  if (temp == '#') {
    Serial.println("password reciever");
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    passwordReciever();
    temp = '0';
  }
  else if (temp == '*')
  {
    Serial.println("password reset");
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    reset();
    temp = '0';
    Serial.println();
    Serial.println(temp);
    
  }
}
void mobileApp() {
  if (Serial.available() > 0)
  {
    reading = Serial.read();
    if (reading == 'O') {
      analogWrite(acled, 255);
    }
    else if (reading == 'f') {
      analogWrite(acled, 0);
    }
    else if (reading == 'M') {
      motiondetected = 0;
      lightsystem = 0;
      digitalWrite(lamp1,LOW);
    }
    else if(reading == 'l')
    {
      for (pos = 90; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);  
    }
    }
    else if(reading == 'c')
    {
      for (pos = 180; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                  
    }
    }
    else if (reading == 'p') {
      for (int i = 0; i < 8; i++)
      {
        reading = ' ';
        while (true) {
          reading = Serial.read();
          Serial.println("Please Enter Password");
          if (reading == '1' ||
              reading == '2' ||
              reading == '3' ||
              reading == '4' ||
              reading == '5' ||
              reading == '6' ||
              reading == '7' ||
              reading == '8' ||
              reading == '9') {
            break;
          }
        }
        if (reading == '1') passwordtemp[i] = '1';
        else if (reading == '2') passwordtemp[i] = '2';
        else if (reading == '3') passwordtemp[i] = '3';
        else if (reading == '4') passwordtemp[i] = '4';
        else if (reading == '5') passwordtemp[i] = '5';
        else if (reading == '6') passwordtemp[i] = '6';
        else if (reading == '7') passwordtemp[i] = '7';
        else if (reading == '8') passwordtemp[i] = '8';
        else if (reading == '9') passwordtemp[i] = '9';
        Serial.println(passwordtemp[i]);
        Serial.println(i);
      }
      j = 0;
      for (int i = 0; i < 4 ; i++)
      {
        password[i] = passwordtemp[j];
        if (password[i] == correct[i]) {
          flag++;
        }
        j += 2;
      }
      if (flag == 4) {
        passFlag = 1;
        digitalWrite(buzzer, LOW);
        servoMotorOpen();
        delay(3000);
        servoMotorClose();
      }
      else {
        lcd.setCursor(0, 0);
        //red light (default)
        lcd.print("password wrong");
        digitalWrite(buzzer, HIGH);
      }
      flag = 0;
    }
  }
}


void setup() {
  dht.begin();
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(flame, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(detectled, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(detectled, LOW);
  myservo.attach(A5);  // attaches the servo on pin 9 to the servo object
}

void loop() {
     if (passFlag == 0) {
   passwordReciever();
   }
  analogWrite(redRGB,0);
  analogWrite(greenRGB,0);
  analogWrite(blueRGB,0);
  humTemp();
  PIR();
  flameDetector();
  brightnessDetector();
  passwordMode();
  analogWrite(redRGB,0);
  analogWrite(greenRGB,0);
  analogWrite(blueRGB,0);
  mobileApp();
  //// 3mlt led esmaha acled e3melo beha el AC
}

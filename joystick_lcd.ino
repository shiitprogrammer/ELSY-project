#include "IRremote.hpp"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define IR_RECEIVE_PIN 8
#define IR_SEND_PIN 2
#define BUTTON 3

typedef struct {
   String name;
   int address;
   int command;
   int repeats;
} Device;

int i = 0;
int device = 0;
int screenState = 0;
Device devices[10];



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
int screenState = 0;
lcd.init(); 
lcd.backlight(); 
IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);  
IrSender.begin(IR_SEND_PIN);
pinMode(A0, INPUT);
pinMode(A1, INPUT);
pinMode(A2, INPUT);
pinMode(3,INPUT_PULLUP);
devices[0].name = "RGBcontroller";
devices[0].address = 0;
devices[0].command = 0;
devices[0].repeats = 0;

devices[1].name = "TVremote1";
devices[1].address = 0;
devices[1].command = 0;
devices[1].repeats = 0;

devices[2].name = "TVremote2";
devices[2].address = 0;
devices[2].command = 0;
devices[2].repeats = 0;

devices[3].name = "ACremote";
devices[3].address = 0;
devices[3].command = 0;
devices[3].repeats = 0;

lcd.setCursor(0, 0);
lcd.print(devices[i].name);
lcd.setCursor(0, 1);
lcd.print(devices[i+1].name);
lcd.setCursor(14, 0);
lcd.print("<-");
}

void update_screen() {
  switch (screenState) {
    case 0:
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print(devices[i].name);
    lcd.setCursor(0, 1);
    lcd.print(devices[i+1].name);
    lcd.setCursor(14, 0);
    lcd.print("<-");
    break;
    case 1:
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print(devices[i].name);
    lcd.setCursor(0, 1);
    lcd.print("address:");
    lcd.setCursor(9, 1);
    lcd.print(devices[i].address);
    lcd.setCursor(14, 0);
    lcd.print("<-");
    break;
    default:
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print(devices[i].name);
    lcd.setCursor(0, 1);
    lcd.print(devices[i+1].name);
    lcd.setCursor(14, 0);
    lcd.print("<-");
    break;
  }
  
  
}

void read_IR() {
  devices[i].address = IrReceiver.decodedIRData.address;
  Serial.println(IrReceiver.decodedIRData.address);
  devices[i].command = IrReceiver.decodedIRData.command;
  devices[i].repeats = IrReceiver.decodedIRData.repeats;
  IrReceiver.printIRResultMinimal(&Serial);
  IrReceiver.resume();
}
void send_IR() {
  IrSender.sendNEC(devices[i].address, devices[i].command, devices[i].repeats);
}



void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(A2) > 900 || analogRead(A2) < 150) {
    if (analogRead(A2) > 900 && i > 0) {
      i--;
    }
    if (analogRead(A2) < 150) {
      i++;
    }
  }
  if (analogRead(A1) > 700 && screenState < 1) {
    int device = i;
    screenState++;
  }
  if (analogRead(A1) < 400 && screenState > 0) {
    int device = i;
    screenState--;
  }
  if (digitalRead(BUTTON) == LOW && screenState == 1) {
    while (IrReceiver.decode() != 1) {
      delay(50);
    }
    if (IrReceiver.decode()) {
      read_IR();
    }
  
  }
  if (digitalRead(BUTTON) == LOW && screenState == 0) {
    send_IR();
  }
  update_screen();
  delay(200);
}

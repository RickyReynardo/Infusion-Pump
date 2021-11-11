/*
Arduino 2x16 LCD - LCD Special Char
modified on 18 Feb 2019
by Saeed Hosseini
https://electropeak.com/learn/
*/

#include <LiquidCrystal.h>
#include <Keypad.h>
#include<EEPROM.h>

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

//keypad pin to arduino
byte pin_rows[ROW_NUM] = {21, 20, 19, 18}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {17, 16, 15, 14}; //connect to the column pinouts of the keypad

//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 

const int pin_BL = 10;

int i = 0;

LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

int a = 0; //used for moving the cursor left or right. 
int conv = 0;

byte numbr[5];  // This will hold 4 bytes, and if global, will init to all zeros
byte indx;  //This will hold an index to the array. If global, it will init to 0
byte targ; // target value

//Numpad
char volume[5];
int relay_pin = 10;
char key_pressed=0;
const byte rows = 4; 
const byte columns = 4; 

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

Keypad keypad_key = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );


void setup() {
  
  lcd.begin(16, 2);

  lcd.setCursor(0,0);

  lcd.print("Infusion pump");
  lcd.setCursor(0,1);
  lcd.print("Volume:");
  lcd.setCursor(14,1);
  lcd.print("ml");
}

void loop() {
  while ( i < 4 ){    //I don't think this is right here. It might be x < 800?
 int x; 
  x = analogRead (0);   
  lcd.setCursor(8,1);

  //Numpad
  digitalWrite(relay_pin, HIGH);

  key_pressed = keypad_key.getKey();

  if(key_pressed=='#')

    lcd.clear();

  if (key_pressed)

  {

    volume[i++]=key_pressed;

    lcd.print(key_pressed);

      }
  if(i==5)

  {

    delay(200);
  
  //lcd button
  else if (x < 60) {
   //right button
   if (indx < 4 ) { 
     indx++;
     a++;
   }
  
  
  }
  else if (x < 200) {
  //up button
   if (numbr[indx] < 1000 ) {
     numbr[indx]++;
    lcd.print(numbr[indx]);
   }

    
  }
  else if (x < 400){
  //down button
    if (numbr[indx] > 0 ) {
    numbr[indx]--;
    lcd.print(numbr[indx]);
    }

  }
   else if (x < 600){
   //left button
     if (indx > 0) {
     indx--;
     a--;
     }

     
  }
  
 else if (x < 800){
 targ = 0;
   for (int i = 0; i < 1000; i++) {        
     
     
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Running");
    delay(200);
  
  }
  
  }
  
  
  delay(100);
}

//I will put another function here that will use the conv number and loop continuously. 

}

/*
Arduino 2x16 LCD - LCD Special Char
modified on 18 Feb 2019
by Saeed Hosseini
https://electropeak.com/learn/
*/

#include <LiquidCrystal.h>
#include <Keypad.h>

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
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[ROWS] = {21, 20, 19, 18}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {17, 16, 15, 14}; //connect to the column pinouts of the kpd

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long loopCount;
unsigned long startTime;
String msg;


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  lcd.setCursor(0,0);

  lcd.print("Infusion pump");
  lcd.setCursor(0,1);
  lcd.print("Volume:");
  lcd.setCursor(14,1);
  lcd.print("ml");
}

long getKeypadIntegerMulti()
{
  
  long value = 0;                                // the number accumulator
  long keyvalue;                                     // the key pressed at current moment
  int isnum;
  do
  {
    keyvalue = keypad.getKey();                          // input the key
    isnum = (keyvalue >= '0' && keyvalue <= '9');         // is it a digit?
    if (isnum)
    {
      lcd.print(keyvalue - '0');
      value = value * 10 + keyvalue - '0';               // accumulate the input number
    }

  } while (isnum || !keyvalue);                          // until not a digit or while no key pressed
  //
  return value;

}//getKeypadInteger

void loop() {
  while ( i < 4 ){    //I don't think this is right here. It might be x < 800?
 int x; 
  x = analogRead (0);   
  lcd.setCursor(8,1);
  char key = keypad.getKey();
    //Numpad
  int val= getKeypadIntegerMulti();
  Serial.println(val);

  
  
  
  if (x < 60) {
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

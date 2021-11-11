//written by Aritra Roy
//github.com/royaritra




#include <Keypad.h>

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
    
}
long getKeypadIntegerMulti()
{
  
  long value = 0;                                // the number accumulator
  long keyvalue;                                     // the key pressed at current moment
  int isnum;
  Serial.println("Enter the digits,press any non-digit to end ");
  Serial.print("You have typed: ");
  do
  {
    keyvalue = keypad.getKey();                          // input the key
    isnum = (keyvalue >= '0' && keyvalue <= '9');         // is it a digit?
    if (isnum)
    {
      Serial.print(keyvalue - '0');
      value = value * 10 + keyvalue - '0';               // accumulate the input number
    }

  } while (isnum || !keyvalue);                          // until not a digit or while no key pressed
  //
  Serial.println(" "); 
  Serial.print("Returning from funtion: "); 
  Serial.println(value);
  return value;

}//getKeypadInteger

void loop() 
{
   int val= getKeypadIntegerMulti(); 
   Serial.println("Value is");
   Serial.println(val);
   delay(2500);
} 

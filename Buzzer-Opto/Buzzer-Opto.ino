int Opto = 3; // connect ir sensor to arduino pin 2
int buzz = 2; // conect Led to arduino pin 13

void setup() 
{
  pinMode (Opto, INPUT); // sensor pin INPUT
  pinMode (buzz, OUTPUT); // Led pin OUTPUT
  Serial.begin(9600);
}

void loop()
{
  int statusSensor = analogRead(Opto);
  Serial.println(statusSensor);
  delay(500);
  if (statusSensor >= 244){
    digitalWrite(buzz, LOW); // LED LOW
  }
  else if (statusSensor <=243)
  {
    digitalWrite(buzz, HIGH); // LED High
  }
  
}

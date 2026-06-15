// C++ code
//

void setup()
{
  int LEDred = 6;
  int taster=7;
  int tasterstatus= 0;
}

void loop()
{
  tasterstatus=digitalRead(taster);
  if(tasterstatus == HIGH){
    digitalWrite(LEDred, HIGH);

  }
  else(tasterstatus == LOW);
  {
    digitalWrite(LEDred, LOW);
  }
}
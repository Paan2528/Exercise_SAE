int LED_RED=10; //LED Red connect with pin 10
int LED_GREEN =9; // LED Green connect with pin 9
int LED_YELLOW=6; //LED Yello connect with pin 10
int height = 150;
int low = 0;
int fedeschritte=5;

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
}

void loop() {
  //Red
  analogWrite(LED_RED, height);
  delay(1000);
  analogWrite(LED_RED, low);
  delay(100);
  //Yellow
  analogWrite(LED_YELLOW, height);
  delay(500);
  analogWrite(LED_YELLOW, low);
  delay(100);
  //Green
  analogWrite(LED_GREEN, height);
  delay(1000);
  analogWrite(LED_GREEN, low);
  delay(100);
  

}

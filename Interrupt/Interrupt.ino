const byte ledPin = 6;

    const byte interruptPin = 7;

    volatile byte state = LOW;

    void setup() {

      pinMode(ledPin, OUTPUT);

      pinMode(interruptPin, INPUT_PULLUP);

      attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
    }

    void loop() {

      digitalWrite(ledPin, state);
    }

    void blink() {

      state = !state;
    }
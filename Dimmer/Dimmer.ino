int eingang = A0;
int LED = 13;

int sensorwert = 0;
int intervall = 0;

unsigned long letzteZeit = 0;
int ledZustand = LOW;

void setup()
{
  pinMode(LED, OUTPUT);
}

void loop()
{
  sensorwert = analogRead(eingang);

  // Potentiometerwert 0-1023 in eine schnelle Blinkzeit umwandeln
  intervall = map(sensorwert, 0, 1023, 1, 20);

  if (millis() - letzteZeit >= intervall)
  {
    letzteZeit = millis();

    if (ledZustand == LOW)
    {
      ledZustand = HIGH;
    }
    else
    {
      ledZustand = LOW;
    }

    digitalWrite(LED, ledZustand);
  }
}
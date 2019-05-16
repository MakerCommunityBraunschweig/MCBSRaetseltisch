#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
/*
  SCK  13
  MISO 12
  MOSI 11
  SDA  10
  RST  9
*/
#define rLED 7
#define gLED 8
#define progressLED1 2
#define progressLED2 3
#define progressLED3 4
#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
//--------------------Station colourCards------------------------------
char output = 0;
byte gamestatus = 0;
const byte UID1[] = {41, 24, 150, 38};
const byte UID2[] = {41, 226, 205, 39};
const byte UID3[] = {137, 29, 31, 86};

void setup() {
  Serial.begin(9600);
  Serial.println("Initialisiere...");
  SPI.begin(); // Init SPI bus
  Wire.begin(1);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event

  pinMode(rLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(progressLED1, OUTPUT);
  pinMode(progressLED2, OUTPUT);
  pinMode(progressLED3, OUTPUT);

  rfid.PCD_Init(); // Init MFRC522

  //testing all LED's
  for ( int i = 2; i < 9; i++) {
    digitalWrite(i, HIGH);
    delay (200);
  }
  for ( int i = 2; i < 9; i++) {
    digitalWrite(i, LOW);
    delay (200);
  }
  reset();
  Serial.println("Initialisierung abgeschlossen.");
}

void loop() {
  if (gamestatus > 0 && gamestatus < 5) {
     byte cardnumber = getCardnumber();
    if (cardnumber == gamestatus && gamestatus < 4 && cardnumber > 0) {
      progress();
    }
    else if (cardnumber != gamestatus && gamestatus < 4 && cardnumber > 0) {
      fail();
    }
    else if ( gamestatus == 4) {
      win();
    }
  }
  updateProgressLed();
}
#include <Keypad.h>

const byte rows = 5; //rows
const byte cols = 4; //columns
char keys [rows][cols] = {
  {'1','4','7','#'},
  {'2','5','8','0'},
  {'3','6','9','*'},
  {'A','B','C','D'}
};

byte rowPins[rows] = {5, 4, 3, 2};
byte colPins[cols] = {9, 8, 7, 6};
Keypad mykeypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char thisKey = mykeypad.getKey();

  if (thisKey){
    Serial.println(thisKey);
  }

  //int trash = Serial.read();
  //Serial.println(trash);
}

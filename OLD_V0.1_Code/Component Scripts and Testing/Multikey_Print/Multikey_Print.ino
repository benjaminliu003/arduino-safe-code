#include <Keypad.h>

int myNum[4];

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad 8,7,6,5
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad 4,3,2,1
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600); 
}

void loop() {
  Serial.println("Enter four-digit num -");  // request for input

  for (int i = 0; i < 4; ++i){
    while((myNum[i] = keypad.getKey())==NO_KEY) {
      delay(1); // Just wait for a key
    } 
    // Wait for the key to be released
    while(keypad.getKey() != NO_KEY) {
      delay(1);
    } 

  }

  Serial.print("Entered number is = ");
  Serial.print(myNum[0]);
  Serial.print(myNum[1]);
  Serial.print(myNum[2]);
  Serial.println(myNum[3]);
  delay(500);
}

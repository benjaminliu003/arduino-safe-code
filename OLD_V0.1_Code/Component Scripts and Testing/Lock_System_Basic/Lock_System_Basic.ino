/* 
 * TRY QR CODE AND TFT LCD.
 */

#include <Keypad.h>
#include <swRTC.h>
#include <sha1.h>
#include <TOTP.h>
#include "Base32_Encode.h"
#include <Adafruit_Fingerprint.h>

#define fingerSerial Serial1

const int Act = 43;

int monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
 * White to pin 18 TX1(in)
 * Green to pin 19 RX1(out)
 */
Adafruit_Fingerprint finSensor = Adafruit_Fingerprint(&fingerSerial);

uint8_t finID = 1;

String userName;

swRTC rtc;

TOTP totp = TOTP(((uint8_t*)Passcode), 10);

bool hasBeenSetup = false;

const int rowCount = 5;
const int columnCount = 4;

char closed[1];

char keyBinds [rowCount][columnCount] = {
  {'1','4','7','#'},
  {'2','5','8','0'},
  {'3','6','9','*'},
  {'A','B','C','D'}
};

byte rowPins[rowCount] = {5, 4, 3, 2}; //TRY TO SWITCH THESE INTO ANALOG FOR HIGH/LOW SENSITIVITY
byte columnPins[columnCount] = {9, 8, 7, 6};
Keypad lockKeypad = Keypad(makeKeymap(keyBinds), rowPins, columnPins, rowCount, columnCount);

char keyEntered[10];

char* twoFACode;
char totpEntered[6];

int totpEntered_indx;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // FOR DEGBUG PURPOSES ONLY
  pinMode(Act, OUTPUT);
  
  timeSet();
}

void timeSet(){
  int actualMonth;
  char compile_date[] = __DATE__ " " __TIME__;
  Serial.println(compile_date);

  char compile_year[4];
  compile_year[0] = compile_date[7];
  compile_year[1] = compile_date[8];
  compile_year[2] = compile_date[9];
  compile_year[3] = compile_date[10];
  String midYear = compile_year;
  int actualYear = midYear.toInt();
  //Serial.print("Year: ");
  //Serial.println(actualYear);
  
  char compile_month[3];
  compile_month[0] = compile_date[0];
  compile_month[1] = compile_date[1];
  compile_month[2] = compile_date[2];
  String midMonth = compile_month;
  if (midMonth == "Jan"){
    actualMonth = 1;
  } else if (midMonth ==  "Feb"){
    actualMonth = 2;
  } else if (midMonth == "Mar"){
    actualMonth = 3;
  } else if (midMonth == "Apr"){
    actualMonth = 4;
  } else if (midMonth == "May"){
    actualMonth = 5;
  } else if (midMonth == "Jun"){
    actualMonth = 6;
  } else if (midMonth == "Jul"){
    actualMonth = 7;
  } else if (midMonth == "Aug"){
    actualMonth = 8;
  } else if (midMonth == "Sep"){
    actualMonth = 9;
  } else if (midMonth == "Oct"){
    actualMonth = 10;
  } else if (midMonth == "Nov"){
    actualMonth = 11;
  } else if (midMonth == "Dec"){
    actualMonth = 12;
  } else {
    actualMonth = 1;
  }
  int thisMonth = monthDays[actualMonth];
  //Serial.print("Month: ");
  //Serial.println(actualMonth);
  
  char compile_day[2];
  compile_day[0] = compile_date[4];
  compile_day[1] = compile_date[5];
  String midDay = compile_day;
  int actualDay = midDay.substring(0,2).toInt();
  //Serial.print("Day: ");
  //Serial.println(actualDay);
  
  if (thisMonth == 30){
    if (actualDay > 30){
      actualDay = 1;
      actualMonth = actualMonth + 1;
    }
  } else {
    if (actualDay > 31){
      actualDay = 1;
      actualMonth = actualMonth + 1;
    }
    if (actualMonth > 12){
      actualYear = actualYear + 1;
      actualMonth = 1;
    }
  }
  
  char compile_hour[1];
  compile_hour[0] = compile_date[12];
  compile_hour[1] = compile_date[13];
  String midHour = compile_hour;
  int actualHour = midHour.toInt() + 5;
  if (actualHour >= 24){
    actualHour = actualHour - 24;
    actualDay = actualDay + 1;
  }
  //Serial.print("Hour: ");
  //Serial.println(actualHour);

  char compile_minute[2];
  compile_minute[0] = compile_date[15];
  compile_minute[1] = compile_date[16];
  String midMin = compile_minute;
  int actualMinute = midMin.substring(0,2).toInt();
  //Serial.print("Minute: ");
  //Serial.println(actualMinute);

  char compile_seconds[2];
  compile_seconds[0] = compile_date[18];
  compile_seconds[1] = compile_date[19];
  String midSecs = compile_seconds;
  int actualSeconds = midSecs.substring(0,2).toInt();
  //Serial.print("Seconds: ");
  //Serial.println(actualSeconds);
  
  rtc.stopRTC();
  rtc.setDate(actualDay, actualMonth, actualYear);
  rtc.setTime(actualHour, actualMinute, actualSeconds);
  rtc.startRTC();
  
  Serial.println("Real Time Clock has started...");
  takePass();
}

void takePass(){
  Serial.println("Please Input a 10 Digit Passcode: ");
  for (int i = 0; i < 10; i++){
    while ((Passcode[i] = lockKeypad.getKey()) == NO_KEY){
      delay(1);
    }
    while (lockKeypad.getKey() != NO_KEY) {
      delay(1);
    }
    Serial.print('*');
  }
  Serial.println(" ");
  Serial.print("Inputed Passcode: ");
  Serial.println(Passcode);
  textBin();
}

void textBin(){
  for(int i = 0; i < 10; i++){
   int ascInt = Passcode[i];
   itoa(ascInt, thisBin, 2);
   int binWritten = Serial.write(thisBin);
   Serial.print(" ");
   Serial.print(" | ");
   
   if (i == 0){
    if (binWritten == 6){
      strcpy(whyBinned, "00");
    }
    else if (binWritten == 7){
      strcpy(whyBinned, "0");
    }
    strcat(whyBinned, thisBin);
   }
   else if (i > 0){
    if (binWritten == 6){
      strcat(whyBinned, "00");
    }
    else if (binWritten == 7){
      strcat(whyBinned, "0");
    }
    strcat(whyBinned, thisBin);
   }
  }

  maybeBinned = whyBinned;
  
  int n = 0;
  
  for (int i = 0; i < 16; i++){
    pointNumBins = maybeBinned.substring(n,n + 5);
    pointNumBins.toCharArray(bins[i], 6);
    
    n = n + 5;
  }
  
  binBase();
}

void binBase(){
  /*
   * CONVERT BINARY TO INT AND THEN USE SWITCH CASE
   */
  strcpy(secretCode, "#"); //GETS RID OF FIRST BIT, MAKE SURE TO REMOVE WHEN PRINTING
  for (int i = 0; i < 16; i++){
    String pointBins = bins[i];
    
    if (pointBins == "00000"){
      strcat(secretCode, "A");
    }
    else if (pointBins == "00001"){
      strcat(secretCode, "B");
    }
    else if (pointBins == "00010"){
      strcat(secretCode, "C");
    }
    else if (pointBins == "00011"){
      strcat(secretCode, "D");
    }
    else if (pointBins == "00100"){
      strcat(secretCode, "E");
    }
    else if (pointBins == "00101"){
      strcat(secretCode, "F");
    }
    else if (pointBins == "00110"){
      strcat(secretCode, "G");
    }
    else if (pointBins == "00111"){
      strcat(secretCode, "H");
    }
    else if (pointBins == "01000"){
      strcat(secretCode, "I");
    }
    else if (pointBins == "01001"){
      strcat(secretCode, "J");
    }
    else if (pointBins == "01010"){
      strcat(secretCode, "K");
    }
    else if (pointBins == "01011"){
      strcat(secretCode, "L");
    }
    else if (pointBins == "01100"){
      strcat(secretCode, "M");
    }
    else if (pointBins == "01101"){
      strcat(secretCode, "N");
    }
    else if (pointBins == "01110"){
      strcat(secretCode, "O");
    }
    else if (pointBins == "01111"){
      strcat(secretCode, "P");
    }
    else if (pointBins == "10000"){
      strcat(secretCode, "Q");
    }
    else if (pointBins == "10001"){
      strcat(secretCode, "R");
    }
    else if (pointBins == "10010"){
      strcat(secretCode, "S");
    }
    else if (pointBins == "10011"){
      strcat(secretCode, "T");
    }
    else if (pointBins == "10100"){
      strcat(secretCode, "U");
    }
    else if (pointBins == "10101"){
      strcat(secretCode, "V");
    }
    else if (pointBins == "10110"){
      strcat(secretCode, "W");
    }
    else if (pointBins == "10111"){
      strcat(secretCode, "X");
    }
    else if (pointBins == "11000"){
      strcat(secretCode, "Y");
    }
    else if (pointBins == "11001"){
      strcat(secretCode, "Z");
    }
    else if (pointBins == "11010"){
      strcat(secretCode, "2");
    }
    else if (pointBins == "11011"){
      strcat(secretCode, "3");
    }
    else if (pointBins == "11100"){
      strcat(secretCode, "4");
    }
    else if (pointBins == "11101"){
      strcat(secretCode, "5");
    }
    else if (pointBins == "11110"){
      strcat(secretCode, "6");
    }
    else if (pointBins == "11111"){
      strcat(secretCode, "7");
    }
  }
  Serial.println(" ");
  Serial.println("Your Secret Code is: ");
  midCode = secretCode;
  Serial.println(midCode.substring(1,17));

  keypadDebug();
}

/* IMPLEMENT LATER-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkSetup() {
  if (setupCompleted == false) {
    takePass();
    setupCompleted = true;
  }
  else {
    keypadDebug();
  }
}
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void keypadDebug(){
  int i;
  
  Serial.println("Please enter your 10 digit PIN.");
  
  for (i = 0; i < 10; i++){
    while ((keyEntered[i] = lockKeypad.getKey()) == NO_KEY){
      delay(1);
    }
    while (lockKeypad.getKey() != NO_KEY) {
      delay(1);
    }
    Serial.print('*');
  }
  Serial.println(" ");
  
  for (i = 0; i < 10; i++){
    if (Passcode[i] != keyEntered[i]){
      Serial.println("Passcode is incorrect.");
      Serial.println("Safe Locked, Please Retry.");
      keypadDebug();
    }
    else{
      if (i == 9){
        Serial.println("Please enter 2 Factor Authentication Code");
        delay(1);
        getInput2FA();
      }
    }
  }
}

void getInput2FA(){
  
  for (int i = 0; i < 6; i++){
    while ((totpEntered[i] = lockKeypad.getKey()) == NO_KEY){
      delay(1);
    }
    while (lockKeypad.getKey() != NO_KEY) {
      delay(1);
    }
    Serial.print(totpEntered[i]);
  }
  Serial.println(" ");

  long GMT = rtc.getTimestamp();
  twoFACode = totp.getCode(GMT);

  for (int i = 0; i <6; i++){
    if (totpEntered[i] != twoFACode[i]){
      Serial.print("Correct code according to GMT time is:");
      Serial.println(twoFACode);
      
      Serial.println("Safe Locked, Please Retry.");
      keypadDebug();
    }
    else{ 
      if (i == 5){
        Serial.println("2FA Code accepted.");
        finStart();
      }
    }
  }
}

void finStart(){
  do{
    delay(1);
  }while(!Serial);
  
  finSensor.begin(57600);

  if(finSensor.verifyPassword() == true){
    Serial.println("Fingerprint Sensor Initialized...");
  }
  else{
    Serial.println("Fingerprint Sensor not Detected.... Check Wiring?"); //CHANGE THIS TO TAMPER ALARM IN FUTURE
    for(int i = 1; i > 0; i++){
      delay(1);
    }
  }
  if (hasBeenSetup == false){
    Serial.print("Please enter your name: ");
    while(!Serial.available()){
      Serial.flush();
      if(Serial.available()){
        userName = Serial.readString();
        Serial.println(userName);
        finPreEnroll(userName);
      }
    }
  }
  else{
    finEnroll(userName);
  }
}

void finPreEnroll(String y){
  Serial.println("Please Wait...");
  delay(1300);
  Serial.println("Only one finger will be enrolled into the lock system");
  delay(1250);
  Serial.println("This is to prevent others from randomly adding fingerprints into the system.");
  delay(1250);
  Serial.println("To add multiple people into the system, please refer to documentation."); //----------------------------------------------ADD THIS FEATURE
  delay(4250);
  Serial.println(" ");
  
  finEnroll(y);
}

void finEnroll(String u){
  int img = -1;
  
  for (int i = 1; i < 3; i++){
  img = -1;

  if (i == 1){
    Serial.println("Please gently press your finger against the fingerprint sensor...");
    delay(3750);
  } else{
    Serial.println("Please gently press your finger against the fingerprint sensor... again");
    delay(3750);
  }
  
  while (img != 1) {
    img = finSensor.getImage();
    if (img == FINGERPRINT_OK){
      Serial.println("Fingerprint Registered...");
      Serial.println("Please keep your finger on the sensor...");
      delay(250);
      break;
    } else if (img == FINGERPRINT_NOFINGER){
      Serial.println("Are you sure your finger is on the sensor? Please Try again.");
      delay(2500);
      finEnroll(u);
    } else if (img == FINGERPRINT_PACKETRECIEVEERR){
      Serial.println("The sensor has lost connection to the control unit.");
      Serial.println("Rebooting...");
      delay(2000);
      finStart();
    } else if (img == FINGERPRINT_IMAGEFAIL) {
      Serial.println("The sensor was unable to capture a valid print.");
      delay(2500);
      finEnroll(u);
    } else{
      Serial.println("Something has gone horribly wrong...");
      for(int i = 1; i > 0; i++){
      delay(1);
      }
    }
  }

  img = finSensor.image2Tz(i);
  if (img == FINGERPRINT_OK){
    Serial.println("Print has been sucessfully converted into a byte array");
    delay(250);
  } else if (img == FINGERPRINT_IMAGEMESS){
    Serial.println("The taken print was too messy to use. Let's try again...");
    delay(250);
    finEnroll(u);
  } else if (img == FINGERPRINT_PACKETRECIEVEERR){
    Serial.println("The sensor has lost connection to the control unit.");
    Serial.println("Rebooting...");
    delay(1000);
    finStart();
  } else if (img == FINGERPRINT_FEATUREFAIL){
    Serial.println("No distinguishable fingerprint features were detected...");
    Serial.println("Let's try again with another finger...");
    delay(500);
    finEnroll(u);
  } else if (img == FINGERPRINT_INVALIDIMAGE){
    Serial.println("This isn't a valid image...");
    Serial.println("Let's try again with another finger...");
    delay(500);
    finEnroll(u);
  } else{
    Serial.println("Oh Noes! Something isn't working!!");
    for(int i = 1; i > 0; i++){
    delay(1);
    }
  }

  if (i == 1){
    if (hasBeenSetup == false){
      Serial.println("Please remove your finger from the sensor...");
      delay(3500);
      img = 0;
      while (img != FINGERPRINT_NOFINGER){
        img = finSensor.getImage();
      }
    } else{
      img = finSensor.fingerFastSearch();
      if (img == FINGERPRINT_OK){
        Serial.print("Fingerprint matched! Welcome ");
        Serial.println(u);
        stillOpen:
        digitalWrite(Act, HIGH);
//DOESN"T WORK, MUST FIX CLOSING OF SAFE-------------------------------------------------------------------------------------------------------------------------------------------------------
        for(int i = 0; i == 1; i++){
          while ((closed[i] = lockKeypad.getKey()) == NO_KEY){
            delay(1);
          }
          while (lockKeypad.getKey() != NO_KEY) {
            delay(1);
          } 
        }

        if (closed[i] == '#'){
          digitalWrite(Act, LOW);
          Serial.println("Safe Closed");
        } else{
          goto stillOpen;
        }
        //------------------------------------------------------------------------------------------------------------------------------------
      } else if (img == FINGERPRINT_PACKETRECIEVEERR){
        Serial.println("The sensor has lost connection to the control unit.");
        Serial.println("Rebooting...");
        delay(1000);
        finEnroll(u);
      } else if (img == FINGERPRINT_NOTFOUND){
        Serial.println("This Fingerprint is not valid");
        Serial.println("Please Try again...");
        delay(2000);
        keypadDebug();
      }
    }
  }
 }

  Serial.println("Proceeding to fingerprint model creation...");
  Serial.println("Please keep your finger on the sensor and please keep still");
  delay(3500);
  
  img = finSensor.createModel();
  if (img == FINGERPRINT_OK){
    Serial.println("Fingerprint matches with prints & model. Model creation complete...");
    Serial.println("Please continue to keep your finger on the sensor and still");
    delay(250);
  } else if (img == FINGERPRINT_PACKETRECIEVEERR){
    Serial.println("The sensor has lost connection to the control unit.");
    Serial.println("Rebooting...");
    delay(1000);
    finStart();
  } else if (img == FINGERPRINT_ENROLLMISMATCH){
    Serial.println("Fingerprint DOES NOT match with recorded prints and model.");
    Serial.println("Let's try again with another finger...");
    delay(500);
    finEnroll(u);
  } else{
    Serial.println("Something happenning that isn't supposed to happen...");
    for(int i = 1; i > 0; i++){
    delay(1);
    }
  }

  Serial.println("Proceeding to store fingerprint model...");
  img = finSensor.storeModel(finID);
  if (img == FINGERPRINT_OK){
    Serial.println("Print & model stored! You may now remove your finger.");
    delay(2500);
    hasBeenSetup = true;
    Serial.println("Rebooting...");
    delay(1000);
    keypadDebug();
  } else if (img == FINGERPRINT_PACKETRECIEVEERR){
    Serial.println("The sensor has lost connection to the control unit.");
    Serial.println("Rebooting...");
    delay(1000);
    finStart();
  } else if (img == FINGERPRINT_BADLOCATION){
    Serial.println("The sensor has tried to store the fingerprint in occupied memory");
    Serial.println("The print enrollment process will now restart...");
    delay(500);
    finEnroll(u);
  } else if (img == FINGERPRINT_FLASHERR){
    Serial.println("The sensor has tried to write to unaccessible memory.");
    Serial.println("The print enrollment process will now restart...");
    delay(500);
    finEnroll(u);
  } else{
    Serial.println("Mr. Stark, I don't feel so good.");
    for(int i = 1; i > 0; i++){
    delay(1);
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  
}

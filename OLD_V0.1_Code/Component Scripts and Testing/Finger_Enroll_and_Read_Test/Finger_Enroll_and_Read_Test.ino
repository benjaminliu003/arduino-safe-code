/*
 * ADD NAME PERSONIFICATION FOR EACH PRINT
 */

#include <Adafruit_Fingerprint.h>

#define fingerSerial Serial1
/*
 * White to pin 18 TX1(in)
 * Green to pin 19 RX1(out)
 */

Adafruit_Fingerprint finSensor = Adafruit_Fingerprint(&fingerSerial);

uint8_t finID = 1;

bool hasBeenSetup = false;

String userName;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  finStart();
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
      if(Serial.available()){
        userName = Serial.readString();
        Serial.println(userName);
        finPreEnroll(userName);
      }
    }
  }
  else{
    finEnroll(userName); //ADD FINGER READING FUNCTION HERE
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

uint8_t finEnroll(String u){
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
        /*
         * REMOVE THIS LOOP ONCE UNLOCKING IS WORKING
         */
        for(int i = 1; i > 0; i++){
          delay(1);
        }
        //JUMP TO UNLOCK FUNCTION HERE----------------------------------------------------------------------------------------------------------------------------------------------------
      } else if (img == FINGERPRINT_PACKETRECIEVEERR){
        Serial.println("The sensor has lost connection to the control unit.");
        Serial.println("Rebooting...");
        delay(1000);
        //BRING BACK TO MAIN MENU-------------------------------------------------------------------------------------------------------------------------------------------
      } else if (img == FINGERPRINT_NOTFOUND){
        Serial.println("This Fingerprint is not valid");
        Serial.println("Please Try again...");
        delay(2000);
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
    hasBeenSetup = true; // SAFETY REMOVE COMMENTS AFTER TESTING------------------------------------------------------------------------------------------------------------------------------------------------------
    Serial.println("Rebooting...");
    delay(1000);
    finStart();
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

char recStr[10] = "";

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial2.flush();

  changePassword();
}

void changePassword() {
  while (!Serial2.available()) {
    Serial.println("Detected no input");
    Serial.println(Serial2.peek());
    if (Serial2.available()) {
      delay(1000);
      Serial2.readBytesUntil('`', recStr, 10);
      Serial.println("Detected input");
      Serial.println(recStr);
      //int s = sizeof(recStr);
      if (recStr[0] == '1') {
        Serial.println("Received data: ");
        Serial.println(recStr);
        char recStr[10] = "";
        Serial.println("Received from Node; now sending - positive");
        Serial2.print("positive");
        break;
      } else if (recStr[0] == 't') {
        Serial.println("Safe is remotely locked");
        char recStr[10] = "";
        unlock();
      }
    }
  }
}
void unlock() {
  while (!Serial2.available()) {
    Serial.println("Again no input");
    Serial.println(Serial2.peek());
    if (Serial2.available()) {
      delay(1000);
      Serial2.readBytesUntil('`', recStr, 10);
      Serial.println("Detected input");
      Serial.println(recStr);
      if (recStr[0] == 'f') {
        Serial.println("Safe is remotely unlocked");
        char recStr[10] = "";
        break;
      }
    }
  }
}



  void loop()
  {
    //changePassword();
    //Serial.println("looping");
  }

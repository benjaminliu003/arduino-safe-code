/*
 * Blynk and Serial Comms for NodeMCU + Mega
 * 
 * This code will allow the NodeMCU chip to communicate with the arduino Mega chip and the Blynk application on your phone.
 * It will communicate via software serial to the Mega's Serial 2 ports.
 * This script allows the user to receive realtime emails and notifications on lock activity from their phone as well as the ability to remotely lock the safe or change its passcode.
 * The connection is also unique as the authentication key determines to which Blynk device + project the device will connect to.
 * 
 * Refer to arduino Mega script for the local safe setup.
 * 
 * created Jan 22, 2020
 * by Benjamin Liu TEJ3M
 */


/*
 * This section of code calls up required libraries and defines which serial ports Blynk will use.
 * It also defines which pins will be used for software Serial communication with the arduino Mega.
 * 
 * A terminal widget is also defined here to Virtual Pin 2 on the Blynk interface.
 * Virtual pins are used to send and receive input to and from the user's mobile, with one assigned to (almost) each functionality.
 */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

WidgetTerminal terminal(V2);

#include<SoftwareSerial.h>
SoftwareSerial SUART(4, 5);

/*
 * This char array stores a new passcode once a passcode reset is requested by the user's mobile phone.
 */
char received[10] = "";

/*
 * The unique authentication ID for the user's project and device ID
 */
char auth[] = "CMO66FknjT1S7Gze6uID498wCoS1Btst";

/*
 * Network SSIDs and Passcodes are stored here.
 */
char ssid[] = "H_SES_38944";
char pass[] = "Ahxm@792";

//At School:
//char ssid[] = "OnePlus 7";
//char pass[] = "ESP-8266-2";

/*
 * This function sends and receives data to anbd from virtual pin 2, the terminal on the Blynk UI.
 * 
 * It's used for resetting and defining a new pascode before passing it off to the arduino Mega.
 * Once a confirmation is received from the Mega that the password has been changed, it notifies the user and also send then an email.
 * On the terminal assigned to Virtual Pin 2, the user will see the output written to it.
 */
BLYNK_WRITE(V2) {
  String i = param.asStr();
  Serial.print("V1 Text is: ");
  Serial.println(i);
  SUART.print(i);

  delay(2500);

  Serial.println("Initializing loop");
  Serial.println("Detected no input");
  Serial.println(SUART.peek());
  if (SUART.available()) {
    Serial.println("Detected input");
    SUART.readBytesUntil('`', received, 10);
    Serial.println("Input is not NULL");
    Serial.println(received);

    if (received[0] == 'p') {
      Serial.println("Received Positive");
      terminal.println(" ");
      Blynk.virtualWrite(V2, "Password Changed to: ");
      Blynk.virtualWrite(V2, i);
      delay(600);
      char received[10] = "";
      Blynk.notify("Password has been changed to: " + i);
      Blynk.email("Password Changed", "Your password has been changed to: " + i);
    }
  }
}

/*
 * This function interacts with Vistual Pin 0 and is used to check if a button is pressed for the remote lock.
 * 
 * If it is, the remote lock signal will be sent to the arduino Mega and the user will be notified.
 * If it button is pressed again and the remote lock is deactivated, the remote unlock signal will be sent to the safe.
 */

BLYNK_WRITE(V0) {
  int p = param.asInt();
  if (p == 1) {
    terminal.clear();
    SUART.print("true");
    SUART.print("false");
    Serial.println("locked");
    Blynk.virtualWrite(V2, "Your safe has been remotely locked.");
    Blynk.notify("Your safe has been remotely locked");
    Blynk.email("Remote Lock", "Your safe has been remotely locked");
  } else {
    terminal.clear();
    SUART.print("false");
    SUART.print("false");
    Serial.println("unlocked");
    Blynk.virtualWrite(V2, "Your safe has been remotely unlocked.");
  }
}

/*
 * In the setup, serial communication is started and the software seriall communication with the arduno Mega is started as well.
 * 
 * Blynk then connects to the specified WiFi network and the terminal prints out instructions for the user.
 */

void setup()
{
  // Debug console
  Serial.begin(9600);
  Serial.flush();
  SUART.begin(9600);
  SUART.flush();

  Blynk.begin(auth, ssid, pass);

  terminal.clear();
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println("To select a new passcode, it must be 10 characters in length.");
  terminal.println("It must also begin with the number '1' as the first character.");
  terminal.println("IF YOU WANT TO CHANGE THE PASSCODE, THE REMOTE LOCK MUST NOT BE ACTIVATED.");
  terminal.println("Select a new passcode: ");
  terminal.flush();
  //Blynk.virtualWrite(V1, "Safe Closed");
}

/*
 * Within the loop function, Blynk.run constantly checks if any of the Bl;ynk functions have been called.
 * 
 * The code beneath it is to check if any input has been received from the safe to determine if the safe has been opened or closed.
 */

void loop()
{
  Blynk.run();

  if (SUART.available()) {
    delay(600);
    Serial.println("Detected input");
    SUART.readBytesUntil('`', received, 10);
    Serial.println("Input is not NULL");
    Serial.println(received);

    if (received[0] == 'o') {
      Serial.println("Safe Open");
      Blynk.virtualWrite(V1, "Safe Open");
      Blynk.notify("Your safe has been opened");
      Blynk.email("Safe Open", "Your safe has been opened");
      delay(600);
      char received[10] = "";
      SUART.flush();
    } else if (received[8] == '.') {
      Serial.println("Safe Closed");
      Blynk.virtualWrite(V1, "Safe Closed");
      Blynk.notify("Your safe has been closed");
      Blynk.email("Safe Closed", "Your safe has been closed");
    }
  }
}

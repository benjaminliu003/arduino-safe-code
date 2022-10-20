#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

WidgetTerminal terminal(V2);

#include<SoftwareSerial.h>
SoftwareSerial SUART(4, 5);

char received[10] = "";
bool repeated = false;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "CMO66FknjT1S7Gze6uID498wCoS1Btst";

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "H_SES_38944";
//char pass[] = "Ahxm@792";

//At School:
char ssid[] = "OnePlus 7";
char pass[] = "ESP-8266-2";

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V2) {
  //int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  String i = param.asStr();
  //char This[10];
  //i.toCharArray(This, 10);
  // double d = param.asDouble();
  Serial.print("V1 Text is: ");
  Serial.println(i);
  SUART.println(i);

  delay(2500);

  Serial.println("Initializing loop");
  Serial.println("Detected no input");
  Serial.println(SUART.peek());
  if (SUART.available()) {
    Serial.println("Detected input");
    SUART.readBytesUntil('`', received, 20);
    Serial.println("Input is not NULL");
    Serial.println(received);

    //Blynk.virtualWrite(V2, "Password has been changed");
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

BLYNK_WRITE(V0) {
  int p = param.asInt();
  if (p == 1) {
    SUART.println("true");
    SUART.println("false");
    Serial.println("locked");
    Blynk.virtualWrite(V2, "Your safe has been remotely locked.");
    Blynk.notify("Your safe has been remotely locked");
    Blynk.email("Remote Lock", "Your safe has been remotely locked");
  } else {
    SUART.println("false");
    SUART.println("false");
    Serial.println("unlocked");
    Blynk.virtualWrite(V2, "Your safe has been remotely unlocked.");
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Serial.flush();
  SUART.begin(115200);
  SUART.flush();

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  terminal.clear();
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println("To select a new passcode, it must be 10 characters in length.");
  terminal.println("It must also begin with the number '1' as the first character.");
  terminal.println("Select a new passcode: ");
  terminal.flush();
}

void loop()
{
  Blynk.run();
}

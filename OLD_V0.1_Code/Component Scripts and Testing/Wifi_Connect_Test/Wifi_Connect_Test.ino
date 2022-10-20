#include <ESP8266WiFi.h>

const char* ssid = "H_SES_38944";
const char* password = "Ahxm@792"; 

int ledPin = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED ){
    delay(500);
    Serial.print(".");
  }

  digitalWrite(ledPin, HIGH);
  Serial.println("Wifi is connected");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

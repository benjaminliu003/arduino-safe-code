char recStr[10] = "";

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop()
{
  byte n = Serial1.available();
  if (n != 0)
  {
    Serial1.readBytesUntil('>', recStr, 10);
    i

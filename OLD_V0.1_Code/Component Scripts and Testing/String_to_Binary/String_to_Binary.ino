String Passcode = "123456789A";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  textBin();
}

void textBin(){
  Serial.print("Inputed Passcode: ");
  Serial.println(Passcode);

  Serial.println("Passcode in Binary: ");
  for(int i = 0; i < Passcode.length(); i++){
   char codeChar = Passcode.charAt(i);
   
    for(int i = 7; i >= 0; i--){
      byte codebyte = bitRead(codeChar,i);
      Serial.print(codebyte, BIN);
    }
    Serial.print(" ");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
 * 
 */

String Passcode;

//DEMO PASSCODE:
//String Passcode = "123456789A";

char thisBin[9] = {};
char whyBinned[81];
String maybeBinned;

String pointNumBins;

char binA[6];
char binB[6];
char binC[6];
char binD[6];
char binE[6];
char binF[6];
char binG[6];
char binH[6];
char binI[6];
char binJ[6];
char binK[6];
char binL[6];
char binM[6];
char binN[6];
char binO[6];
char binP[6];

char* bins[] = {binA, binB, binC, binD, binE, binF, binG, binH, binI, binJ, binK, binL, binM, binN, binO, binP};

char bin1[4];
char bin2[4];
char bin3[4];
char bin4[4];
char bin5[4];
char bin6[4];
char bin7[4];
char bin8[4];
char bin9[4];
char bin10[4];

char* binNums[] = {bin1, bin2, bin3, bin4, bin5, bin6, bin7, bin8, bin9, bin10};

uint8_t bin0x1;
uint8_t bin0x2;
uint8_t bin0x3;
uint8_t bin0x4;
uint8_t bin0x5;
uint8_t bin0x6;
uint8_t bin0x7;
uint8_t bin0x8;
uint8_t bin0x9;
uint8_t bin0x10;

uint8_t insertBin[40];

char secretCode[17];
String midCode;

char hexThis[11];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  takePass();
}

void takePass(){
  Serial.println("Please Input a 10 Digit Passcode: ");

  while(!Serial.available()){
    if (Serial.available()){
      Passcode = Serial.readString();
      Serial.print("Inputed Passcode: ");
      Serial.println(Passcode);

      textBin();
    }
  }
}

void textBin(){
  Serial.println("Passcode in Binary: ");
  
  for(int i = 0; i < 10; i++){
   char codeChar = Passcode.charAt(i);
   int ascInt = codeChar;
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

  textHex();
}

void textHex() {
  //Passcode.toCharArray(hexThis, 11);

  String hexString;
  char hexSingle[2];
    
  //insertBin[0] = (uint8_t)atoi(hexThis);
  
  for (int i = 0; i < 10; i++){;
    hexString = Passcode.substring(i, i+1);
    hexString.toCharArray(hexSingle, 2);
    
    Serial.print("hexSingle value: ");
    Serial.println(hexSingle);
    
    switch(i){
      case 0:
        bin0x1 = (uint8_t)atoi(hexSingle);
        break;
      case 1:
        bin0x2 = (uint8_t)atoi(hexSingle);
        break;
      case 2:
        bin0x3 = (uint8_t)atoi(hexSingle);
        break;
      case 3:
        bin0x4 = (uint8_t)atoi(hexSingle);
        break;
      case 4:
        bin0x5 = (uint8_t)atoi(hexSingle);
        break;
      case 5:
        bin0x6 = (uint8_t)atoi(hexSingle);
        break;
      case 6:
        bin0x7 = (uint8_t)atoi(hexSingle);
        break;
      case 7:
        bin0x8 = (uint8_t)atoi(hexSingle);
        break;
      case 8:
        bin0x9 = (uint8_t)atoi(hexSingle);
        break;
      case 9:
        bin0x10 = (uint8_t)atoi(hexSingle);
        break;
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:

}

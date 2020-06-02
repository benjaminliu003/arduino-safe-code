/*
 * Sets arrays used by the code during the conversion process from the inputted string passcode to the Base32 Secret code for the google authenticator.
 */

char Passcode[10]; //STORE TO EEPROM AFTER FIRST TIME

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

char secretCode[17];
String midCode;

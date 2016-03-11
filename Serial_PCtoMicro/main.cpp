//------------------------------------------------------------------------------------
//
//	Code: PC to uController Serial (main.cpp)
//	Authors: Zachary Garrard, Justin Cox
//	Date: 3/10/2016
//
//------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <Windows.h>
#include "Serial.h"

#define G0 0x4730
#define G1 0x4731
#define G4 0x4734
#define M2 0x4D32
#define M4 0x4D34
#define M5 0x4D35
#define S0 0x5330
#define S1 0x5331
#define S2 0x5332
#define S3 0x5333
#define S4 0x5334
#define S5 0x5335
#define S6 0x5336
#define S7 0x5337

void readUart(void);

bool getRowData(FILE *inFile, uint16_t xCoor[], uint16_t yCoor[], uint16_t gCode[], uint16_t pauseP[], uint16_t size);

//------------------------------------------------------------------------------------
//	FUNCTION MAIN
//------------------------------------------------------------------------------------

CSerial serial;


int main(){

	//*************************
	//	Serial Vars
	//*************************
	int port = 3; 
	int baudRate = 9600; 
	int dispType = 0;
	int nBytesSent = 0;
	const int SIZE = 1600;
	uint8_t buffer[SIZE];
	//*************************
	//	File IO Vars
	//*************************
	FILE *fp;
	
	uint16_t xInit = 0;
	uint16_t yInit = 0;
	uint16_t pInit = 0;
	uint16_t g00 = G0;
	uint16_t g4 = G4;
	
	uint8_t garbage[100] = {0};
	uint8_t chG;

	uint16_t *xCoors;
	uint16_t *yCoors;
	uint16_t *gCodes;
	uint16_t *pauseP;

	uint16_t sizeCol = 0;
	uint16_t sizeRow = 0;

	//-----------------------------------
	//		PARSER CODE
	//-----------------------------------
	fp = fopen("fox.gcode", "r");

	if(!fp){
		printf("Error opening file! \n");
		return 0;
	}

	//Get Size of image for arrays
	fscanf(fp, "size %hd,%hd", &sizeCol, &sizeRow);
	chG = fgetc(fp);
	//printf("SizeX is: %hd and SizeY is: %hd \n", sizeX, sizeY);

	//Dynamically set size of arrays
	xCoors = new uint16_t [(sizeCol * 5)];
	yCoors = new uint16_t [(sizeCol * 5)];
	pauseP = new uint16_t [(sizeCol * 5)];
	gCodes = new uint16_t [(sizeCol * 5)];

	//Get initial position
	fscanf(fp, "G00 X%hd Y%hd", &xInit, &yInit);
	chG = fgetc(fp);
	
	fscanf(fp, "G04 P0.%hd", &pInit);
	chG = fgetc(fp);

	//Get Data from First Row
	if(getRowData(fp, xCoors, yCoors, gCodes, pauseP, sizeCol)){
		for(int i = 0; i < 100; i = i + 5){
			printf("X:%hd Y:%hd \n", xCoors[i], yCoors[i]);
		}
	}
	else{
		//TODO: END PROGRAM
	}

	//printf("PInit: %hd \n",pInit);
	//printf("xInit is: %hd and yInit is: %hd \n", xInit, yInit);

	//-----------------------------------
	//		SERIAL CODE
	//-----------------------------------
	if (!serial.Open(port, baudRate)){
		printf("Error opening COM port! \n");
		return 0;
	}

	//-----------------------------------
	//	WRITE DATA
	//-----------------------------------
	
	nBytesSent = serial.SendData(buffer, SIZE);

	serial.Close();

	delete xCoors;
	delete yCoors;
	delete gCodes;
	delete pauseP;

	return 0;
}

bool getRowData(FILE *inFile, uint16_t xCoor[], uint16_t yCoor[], uint16_t gCode[], uint16_t pauseP[], uint16_t size){
	uint8_t firstChar;
	uint8_t secondChar;
	uint8_t thirdChar;

	uint8_t chG;

	for(int i = 0; i < (size * 5); i++){
		
		firstChar = fgetc(inFile);
		printf("Character in question: %c \n",firstChar);

		switch(firstChar){
			case 'G':
				secondChar = fgetc(inFile);
				thirdChar = fgetc(inFile);
				if(thirdChar == '1'){
					fscanf(inFile, " X%hd Y%hd", &xCoor[i], &yCoor[i]);
					printf("X and Y: %hd %hd \n",xCoor[i], yCoor[i]);
					chG = fgetc(inFile);
					gCode[i] = G1;
				}
				else if(thirdChar == '4'){
					fscanf(inFile, " P0.00%hd", &pauseP[i]);
					chG = fgetc(inFile);
					gCode[i] = G4;
				}
				else{
					printf("Unkown Gcode with 'G' \n");
				}
				break;
			case 'S':
				secondChar = fgetc(inFile);
				switch(secondChar){
					case '0':
						gCode[i] = S0;
						break;
					case '1':
						gCode[i] = S1;
						break;
					case '2':
						gCode[i] = S2;
						break;
					case '3':
						gCode[i] = S3;
						break;
					case '4':
						gCode[i] = S4;
						break;
					case '5':
						gCode[i] = S5;
						break;
					case '6':
						gCode[i] = S6;
						break;
					case '7':
						gCode[i] = S7;
						break;
					default:
						printf("Unkown S code \n");
						break;
				} //end of switch
				chG = fgetc(inFile);
				break;
			case 'M':
				secondChar = fgetc(inFile);
				thirdChar = fgetc(inFile);

				if(thirdChar == '4'){
					gCode[i] = M4;
				}
				else if(thirdChar == '5'){
					gCode[i] = M5;
				}
				else if(thirdChar == '2'){
					return false;
				}
				else {
					printf("Unknown M code \n");
				}
				chG = fgetc(inFile);
				break;
			default:
				printf("Unknown line from file \n");
				break;
		} //end of switch
	}// end of for

	return true;
}

void readUart(void){

	int nBytesRead = 0;
	int curT = 0; 
	int oldT = 0;
	char readBuffer[1];
	
	while(readBuffer[0] != '\n')
	{
		curT = GetTickCount();

		if (curT - oldT > 5)
		{
			nBytesRead = serial.ReadData(readBuffer, sizeof(readBuffer));
	
			if (nBytesRead > 0)
			{

				for(int i = 0; i < nBytesRead; i++){
					printf("%c", readBuffer[i]);
				}

			}

			oldT = curT;
		}

	}

}
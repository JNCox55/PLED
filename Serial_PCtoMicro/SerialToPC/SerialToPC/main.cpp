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
void splitShort(uint16_t short2Split, uint8_t &msb, uint8_t &lsb, uint8_t buffer[]);
bool getRowData(FILE *inFile, uint16_t xCoor[], uint16_t yCoor[], uint16_t gCode[], uint16_t pauseP[], uint16_t size);
void sendRowData(uint16_t xCoor[], uint16_t yCoor[], uint16_t gCode[], uint16_t pauseP[], uint16_t size);

//------------------------------------------------------------------------------------
//	FUNCTION MAIN
//------------------------------------------------------------------------------------

CSerial serial;


int main(){

	//*************************
	//	Serial Vars
	//*************************
	int port = 3;//3; 
	int baudRate = 9600;//9600; 
	int dispType = 0;
	int nBytesSent = 0;
	
	uint8_t buffer[2];

	uint8_t shortMSB; 
	uint8_t shortLSB;
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
	xCoors = new uint16_t [sizeCol];
	yCoors = new uint16_t [sizeCol];
	pauseP = new uint16_t [sizeCol];
	gCodes = new uint16_t [(sizeCol * 5)];

	//Get initial position
	fscanf(fp, "G00 X%hd Y%hd", &xInit, &yInit);
	chG = fgetc(fp);
	
	fscanf(fp, "G04 P0.%hd", &pInit);
	chG = fgetc(fp);

	//Open Serial Port
	if (!serial.Open(port, baudRate)){
		printf("Error opening COM port! \n");
		return 0;
	}

	//Get Data from First Row
	if(getRowData(fp, xCoors, yCoors, gCodes, pauseP, sizeCol)){
		for(int i = 0; i < sizeCol; i++){
			//printf("X:%hd Y:%hd \n", xCoors[i], yCoors[i]);
			//printf("P value: %hd \n", pauseP[i]);
		}

		//----------------------------
		//Send inital values
		//----------------------------
		//	ZZ sizeCol, sizeRow
		buffer[0] = 'Z';
		buffer[1] = 'Z';
		serial.SendData(buffer, 2);

		splitShort(sizeCol, shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		splitShort(sizeRow, shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		//	G0, Xinit, Yinit
		splitShort(g00, shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		splitShort(xInit, shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		splitShort(yInit, shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		//	G4, Pinit
		splitShort(g4, shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		splitShort(pInit, shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);

		//----------------------------
		//Send row 
		//----------------------------
		sendRowData(xCoors, yCoors, gCodes, pauseP, sizeCol);


		//	Read GO to send next row
		//readUart();

	}
	else{
		//TODO: END PROGRAM
	}

	serial.Close();

	delete [] xCoors;
	delete [] yCoors;
	delete [] gCodes;
	delete [] pauseP;

	return 0;
}

bool getRowData(FILE *inFile, uint16_t xCoor[], uint16_t yCoor[], uint16_t gCode[], uint16_t pauseP[], uint16_t size){
	uint8_t firstChar;
	uint8_t secondChar;
	uint8_t thirdChar;

	uint8_t chG;
	uint32_t iPix = 0;
	uint32_t counter = 1;

	for(int i = 0; i < (size * 5); i++){
		
		firstChar = fgetc(inFile);
		//printf("Character in question: %c \n",firstChar);
		
		switch(firstChar){
			case 'G':
				secondChar = fgetc(inFile);
				thirdChar = fgetc(inFile);
				if(thirdChar == '1'){
					fscanf(inFile, " X%hd Y%hd", &xCoor[iPix], &yCoor[iPix]);
					//printf("X and Y: %hd %hd \n",xCoor[iPix], yCoor[iPix]);
					chG = fgetc(inFile);
					gCode[i] = G1;
				}
				else if(thirdChar == '4'){
					fscanf(inFile, " P0.00%hd", &pauseP[iPix]);
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

		if(counter == 5){
			iPix++;
			counter = 0;
		}

		counter++;
	}// end of for

	return true;
}

void sendRowData(uint16_t xCoor[], uint16_t yCoor[], uint16_t gCode[], uint16_t pauseP[], uint16_t size){
	uint8_t shortMSB;
	uint8_t shortLSB;
	uint8_t buffer[2];

	uint32_t nBytesSent = 0;

	uint32_t iPix = 0;

	for(int i = 0; i < (size * 5); i = i + 5){
		//Order to send
		//	Gcode[i], X[iPix], Y[iPix]  <--- G00 X#,Y#
		splitShort(gCode[i], shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		splitShort(xCoor[iPix], shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		splitShort(yCoor[iPix], shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		//  Gcode[i+1]	<--- S#
		splitShort(gCode[i+1], shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		//  Gcode[i+2]	<--- M04
		splitShort(gCode[i+2], shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		//	Gcode[i+3], P[iPix]	<--- G04 P#
		splitShort(gCode[i+3], shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		splitShort(pauseP[iPix], shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);
		//	Gcode[i+4]	<--- M05
		splitShort(gCode[i+4], shortMSB, shortLSB, buffer);
		serial.SendData(buffer, 2);

		iPix++;
	}
	
	//	RD when row has been sent
	buffer[0] = 'R';
	buffer[1] = 'D';
	serial.SendData(buffer, 2);

	//nBytesSent = serial.SendData(buffer, SIZE);

}

void splitShort(uint16_t short2Split, uint8_t &msb, uint8_t &lsb, uint8_t buffer[]){
	msb = short2Split >> 8;
	lsb = short2Split & 0x00FF;

	buffer[0] = msb;
	buffer[1] = lsb;
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
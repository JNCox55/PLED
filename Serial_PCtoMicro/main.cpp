//------------------------------------------------------------------------------------
//
//	Code: PC to uController Serial (main.cpp)
//	Authors: Zachary Garrard, Justin Cox
//	Date: 1/30/2016
//
//------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "Serial.h"

//------------------------------------------------------------------------------------
//	Function Prototypes
//------------------------------------------------------------------------------------

void sendArrayData(int arraySizes[], short xC[], short yC[], int index);
void waitForCommand(char *nextArray);

//------------------------------------------------------------------------------------
//	FUNCTION MAIN
//------------------------------------------------------------------------------------

int main(){

	//*************************
	//	SERIAL
	//*************************
	int port = 1; 
	int baudRate = 9600; 
	int dispType = 0;

	CSerial serial;

	//*************************
	//	OTHER VARIABLES
	//*************************
	short xC[50] = {0};
	short yC[50] = {0};

	int arraySizes[50] = {0};
	char nextArrayF = 0;



	//-----------------------------------
	//
	//		SERIAL CODE
	//
	//-----------------------------------
	if (!serial.Open(port, baudRate)){
		printf("Error opening COM port! \n");
		return 0;
	}

	//-----------------------------------
	//	Send 1st array
	//-----------------------------------
	sendArrayData(arraySizes, xC, yC, 0);
	waitForCommand(&nextArrayF);
	
	/*if(nextArray == 1 && numOfArrays > 1){
		//-----------------------------------
		//	Send 2nd array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x2C, y2C,1);
		waitForCommand(&nextArray);
	}*/


	//SEND BACK TO ORIGIN
	nBytesSent = serial.SendData(finished, 2);

	if(nBytesSent == 0){
		printf("Trouble sending All Done Command\n", i);
	}

	serial.Close();

	//-----------------------------------
	//	Free Dynamic Memory
	//-----------------------------------


	return 0;
}

//------------------------------------------------------------------------------------
//	Function Definitions
//------------------------------------------------------------------------------------

void sendArrayData(int arraySizes[], short xC[], short yC[], int index){
	
	int i = 0;
	int nBytesSent = 0;
	//B = 0x42, G = 0x47, R = 0x52
	//M = 0x4D
	//X = 0x58, D = 0x44
	char commandB[2] = {0x4D,0x42};
	char commandG[2] = {0x4D,0x47};
	char commandR[2] = {0x4D,0x52};
	char commandX[2] = {0x58,0x44};
	char commandY[2] = {0x59,0x44};

	char halfShort[2] = {0};
	
	//-----------------------------------
	//	SEND DATA
	//-----------------------------------
	//Send X's
	for(i = 0; i <arraySizes[index] ; i++){
		halfShort[0] = (xC[i]>>8) & 0xFF;
		halfShort[1] = (xC[i]) & 0xFF;
		nBytesSent = serial.SendData(halfShort, 2);

		if(nBytesSent == 0){
			printf("Trouble sending X's on i: &d \n", i);
		}
	}

	//Tell uController that we are done with X's
	nBytesSent = serial.SendData(commandX, 2);
	printf("Done sending X's\n");

	//Send Y's
	for(i = 0; i <arraySizes[index] ; i++){
		halfShort[0] = (yC[i]>>8) & 0xFF;
		halfShort[1] = (yC[i]) & 0xFF;
		nBytesSent = serial.SendData(halfShort, 2);

		if(nBytesSent == 0){
			printf("Trouble sending Y's on i: &d \n", i);
		}
	}

	//Tell uController that we are done with Y's
	nBytesSent = serial.SendData(commandY, 2);
	printf("Done sending Y's\n");

}


void waitForCommand(char *nextArray){	
	
	int nBytesRead = 0;
	int curT = 0; 
	int oldT = 0;
	
	//-----------------------------------
	//	READ DATA
	//-----------------------------------
	while (*nextArray == 0)
	{
		curT = GetTickCount();

		if (curT - oldT > 10)
		{
			char buffer[1];
			nBytesRead = serial.ReadData(buffer, sizeof(buffer));
	
			if (nBytesRead > 0)
			{

				for(int i = 0; i<1; i++){
					printf("%c", buffer[i]);
				}
				printf("\n");

			}

			if(buffer[0] == 'N'){
				*nextArray = 1;
			}

			oldT = curT;
		}
	}

	//printf("I made it out of the while loop! \n");

}

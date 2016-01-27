#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "Serial.h"

void getNextArray(FILE *fp, char *garbage, char chG, float xN[], float yN[], int *arrayCount);
void findFooter(FILE *fp, char *endF);
void sendArrayData(int arraySizes[], short xC[], short yC[], int index);
void waitForCommand(char *nextArray);


CSerial serial;

int main () {
	
	//*************************
	//	SERIAL
	//*************************
	int port = 1; 
	int baudRate = 9600; 
	int dispType = 0;
	
	char nextArray = 0;

	char finished[2] = {'A','D'};
	int nBytesSent = 0;
	
	//*************************
	//	FILE I/O
	//*************************
	FILE *fp;

	int i = 0;
	char ch1 = 0;
	char ch2 = 0;
	char chG = 0;
	int varRet = 0;

	float startX = 0;
	float startY = 0;

	const int SIZE = 5000;

	float x[SIZE] = {0};
	float y[SIZE] = {0};

	int arrayCount = 0;
	int numOfArrays = 0;
	int arraySizes[49] = {0};

	short *xC;
	short *yC;
	short *x2C;
	short *y2C;
	short *x3C;
	short *y3C;
	short *x4C;
	short *y4C;
	short *x5C;
	short *y5C;
	short *x6C;
	short *y6C;
	short *x7C;
	short *y7C;
	short *x8C;
	short *y8C;
	short *x9C;
	short *y9C;
	short *x10C;
	short *y10C;
	short *x11C;
	short *y11C;
	short *x12C;
	short *y12C;
	short *x13C;
	short *y13C;
	short *x14C;
	short *y14C;
	short *x15C;
	short *y15C;
	short *x16C;
	short *y16C;
	short *x17C;
	short *y17C;
	short *x18C;
	short *y18C;
	short *x19C;
	short *y19C;
	short *x20C;
	short *y20C;
	short *x21C;
	short *y21C;
	short *x22C;
	short *y22C;
	short *x23C;
	short *y23C;
	short *x24C;
	short *y24C;
	short *x25C;
	short *y25C;
	short *x26C;
	short *y26C;
	short *x27C;
	short *y27C;
	short *x28C;
	short *y28C;
	short *x29C;
	short *y29C;
	short *x30C;
	short *y30C;
	short *x31C;
	short *y31C;
	short *x32C;
	short *y32C;
	short *x33C;
	short *y33C;
	short *x34C;
	short *y34C;
	short *x35C;
	short *y35C;
	short *x36C;
	short *y36C;
	short *x37C;
	short *y37C;
	short *x38C;
	short *y38C;
	short *x39C;
	short *y39C;
	short *x40C;
	short *y40C;
	short *x41C;
	short *y41C;
	short *x42C;
	short *y42C;
	short *x43C;
	short *y43C;
	short *x44C;
	short *y44C;
	short *x45C;
	short *y45C;
	short *x46C;
	short *y46C;
	short *x47C;
	short *y47C;
	short *x48C;
	short *y48C;
	short *x49C;
	short *y49C;
	short *x50C;
	short *y50C;


	char garbage[100] = {0};
	char begF = 0;
	char endF = 0;


	//Open G-cdoe File
	//fp = fopen("gCodeText.txt", "r");
	//fp = fopen("jcLogo.txt", "r");
	//fp = fopen("Mario.txt", "r");
	//fp = fopen("smashLogo.txt", "r");
	//fp = fopen("usuLogo.txt", "r");
	//fp = fopen("bull.txt", "r");
	//fp = fopen("cripps.txt", "r");

	//-----------------------------------
	//	SKIP TO FIRST G00 command
	//-----------------------------------
	while(begF == 0){

		ch1 = fgetc(fp);
		ch2 = fgetc(fp);
		
		if(ch1 == 'G' && ch2 == '0'){
			begF = 1;
		}
		else if (ch1 == '0' && ch2 == 'G'){
			begF = 1;
		}
		else if (ch1 == EOF || ch2 == EOF){
			break;
		}

	}
	//-----------------------------------
	//	GET STARTING POSITION
	//-----------------------------------
	fscanf(fp, "%[^\n]", garbage);
	chG = fgetc(fp);
	fscanf(fp, "G00 X%f Y%f", &startX, &startY);

	//printf("X0: %f, Y0: %f \n", startX,startY);
	chG = fgetc(fp);
	chG = fgetc(fp);

	fscanf(fp, "%[^\n]", garbage);
	chG = fgetc(fp);
	
	//-----------------------------------
	//	GET FIRST ARRAY OF VALUES
	//-----------------------------------
	x[0] = startX;
	y[0] = startY;
	i = 1;
	arrayCount = 1;
	varRet = fscanf(fp, "G01 X%f Y%f", &x[i], &y[i]);
	while(varRet == 2){
		arrayCount++;
		fscanf(fp, "%[^\n]", garbage);
		chG = fgetc(fp);
		i++;
		varRet = fscanf(fp, "G01 X%f Y%f", &x[i], &y[i]);
	}

	arraySizes[0] = arrayCount;
	numOfArrays++;

	xC =(short*)calloc(arrayCount,sizeof(short));
	yC =(short*)calloc(arrayCount,sizeof(short));

	for(i = 0; i < arrayCount; i++){
		xC[i] = (short)x[i];
		yC[i] = (short)y[i];
		
		//printf("X: %f, Y: %f \n", x[i],y[i]);
		//printf("X: %d, Y: %d \n", xC[i],yC[i]);
	}


	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 2nd ARRAY OF VALUES
		//-----------------------------------
		
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[1] = arrayCount;
		numOfArrays++;

		x2C =(short*)calloc(arrayCount,sizeof(short));
		y2C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x2C[i] = (short)x[i];
			y2C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x2C[i],y2C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 3rd ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[2] = arrayCount;
		numOfArrays++;

		x3C =(short*)calloc(arrayCount,sizeof(short));
		y3C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x3C[i] = (short)x[i];
			y3C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x3C[i],y3C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 4th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[3] = arrayCount;
		numOfArrays++;

		x4C =(short*)calloc(arrayCount,sizeof(short));
		y4C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x4C[i] = (short)x[i];
			y4C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x4C[i],y4C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 5th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[4] = arrayCount;
		numOfArrays++;

		x5C =(short*)calloc(arrayCount,sizeof(short));
		y5C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x5C[i] = (short)x[i];
			y5C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x5C[i],y5C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 6th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[5] = arrayCount;
		numOfArrays++;

		x6C =(short*)calloc(arrayCount,sizeof(short));
		y6C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x6C[i] = (short)x[i];
			y6C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x6C[i],y6C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){

		//-----------------------------------
		//	GET 7th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[6] = arrayCount;
		numOfArrays++;

		x7C =(short*)calloc(arrayCount,sizeof(short));
		y7C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x7C[i] = (short)x[i];
			y7C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x7C[i],y7C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 8th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[7] = arrayCount;
		numOfArrays++;

		x8C =(short*)calloc(arrayCount,sizeof(short));
		y8C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x8C[i] = (short)x[i];
			y8C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x8C[i],y8C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 9th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[8] = arrayCount;
		numOfArrays++;

		x9C =(short*)calloc(arrayCount,sizeof(short));
		y9C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x9C[i] = (short)x[i];
			y9C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x9C[i],y9C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 10th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[9] = arrayCount;
		numOfArrays++;

		x10C =(short*)calloc(arrayCount,sizeof(short));
		y10C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x10C[i] = (short)x[i];
			y10C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x10C[i],y10C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 11th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[10] = arrayCount;
		numOfArrays++;

		x11C =(short*)calloc(arrayCount,sizeof(short));
		y11C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x11C[i] = (short)x[i];
			y11C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x11C[i],y11C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 12th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[11] = arrayCount;
		numOfArrays++;

		x12C =(short*)calloc(arrayCount,sizeof(short));
		y12C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x12C[i] = (short)x[i];
			y12C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x12C[i],y12C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 13th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[12] = arrayCount;
		numOfArrays++;

		x13C =(short*)calloc(arrayCount,sizeof(short));
		y13C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x13C[i] = (short)x[i];
			y13C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x13C[i],y13C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 14th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[13] = arrayCount;
		numOfArrays++;

		x14C =(short*)calloc(arrayCount,sizeof(short));
		y14C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x14C[i] = (short)x[i];
			y14C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x14C[i],y14C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 15th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[14] = arrayCount;
		numOfArrays++;

		x15C =(short*)calloc(arrayCount,sizeof(short));
		y15C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x15C[i] = (short)x[i];
			y15C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x15C[i],y15C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 16th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[15] = arrayCount;
		numOfArrays++;

		x16C =(short*)calloc(arrayCount,sizeof(short));
		y16C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x16C[i] = (short)x[i];
			y16C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x16C[i],y16C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 17th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[16] = arrayCount;
		numOfArrays++;

		x17C =(short*)calloc(arrayCount,sizeof(short));
		y17C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x17C[i] = (short)x[i];
			y17C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x17C[i],y17C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 18th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[17] = arrayCount;
		numOfArrays++;

		x18C =(short*)calloc(arrayCount,sizeof(short));
		y18C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x18C[i] = (short)x[i];
			y18C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x18C[i],y18C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 19th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[18] = arrayCount;
		numOfArrays++;

		x19C =(short*)calloc(arrayCount,sizeof(short));
		y19C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x19C[i] = (short)x[i];
			y19C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x19C[i],y19C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 20th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[19] = arrayCount;
		numOfArrays++;

		x20C =(short*)calloc(arrayCount,sizeof(short));
		y20C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x20C[i] = (short)x[i];
			y20C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x20C[i],y20C[i]);
		}
	}
	
	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 21st ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[20] = arrayCount;
		numOfArrays++;

		x21C =(short*)calloc(arrayCount,sizeof(short));
		y21C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x21C[i] = (short)x[i];
			y21C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x21C[i],y21C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 22nd ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[21] = arrayCount;
		numOfArrays++;

		x22C =(short*)calloc(arrayCount,sizeof(short));
		y22C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x22C[i] = (short)x[i];
			y22C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x22C[i],y22C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 23rd ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[22] = arrayCount;
		numOfArrays++;

		x23C =(short*)calloc(arrayCount,sizeof(short));
		y23C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x23C[i] = (short)x[i];
			y23C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x23C[i],y23C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 24th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[23] = arrayCount;
		numOfArrays++;

		x24C =(short*)calloc(arrayCount,sizeof(short));
		y24C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x24C[i] = (short)x[i];
			y24C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x24C[i],y24C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 25th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[24] = arrayCount;
		numOfArrays++;

		x25C =(short*)calloc(arrayCount,sizeof(short));
		y25C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x25C[i] = (short)x[i];
			y25C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x25C[i],y25C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 26th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[25] = arrayCount;
		numOfArrays++;

		x26C =(short*)calloc(arrayCount,sizeof(short));
		y26C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x26C[i] = (short)x[i];
			y26C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x26C[i],y26C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 27th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[26] = arrayCount;
		numOfArrays++;

		x27C =(short*)calloc(arrayCount,sizeof(short));
		y27C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x27C[i] = (short)x[i];
			y27C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x27C[i],y27C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 28th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[27] = arrayCount;
		numOfArrays++;

		x28C =(short*)calloc(arrayCount,sizeof(short));
		y28C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x28C[i] = (short)x[i];
			y28C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x28C[i],y28C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 29th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[28] = arrayCount;
		numOfArrays++;

		x29C =(short*)calloc(arrayCount,sizeof(short));
		y29C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x29C[i] = (short)x[i];
			y29C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x29C[i],y29C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 30th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[29] = arrayCount;
		numOfArrays++;

		x30C =(short*)calloc(arrayCount,sizeof(short));
		y30C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x30C[i] = (short)x[i];
			y30C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x30C[i],y30C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 31st ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[30] = arrayCount;
		numOfArrays++;

		x31C =(short*)calloc(arrayCount,sizeof(short));
		y31C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x31C[i] = (short)x[i];
			y31C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x31C[i],y31C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 32nd ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[31] = arrayCount;
		numOfArrays++;

		x32C =(short*)calloc(arrayCount,sizeof(short));
		y32C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x32C[i] = (short)x[i];
			y32C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x32C[i],y32C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 33rd ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[32] = arrayCount;
		numOfArrays++;

		x33C =(short*)calloc(arrayCount,sizeof(short));
		y33C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x33C[i] = (short)x[i];
			y33C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x33C[i],y33C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 34th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[33] = arrayCount;
		numOfArrays++;

		x34C =(short*)calloc(arrayCount,sizeof(short));
		y34C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x34C[i] = (short)x[i];
			y34C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x34C[i],y34C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 35th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[34] = arrayCount;
		numOfArrays++;

		x35C =(short*)calloc(arrayCount,sizeof(short));
		y35C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x35C[i] = (short)x[i];
			y35C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x35C[i],y35C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 36th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[35] = arrayCount;
		numOfArrays++;

		x36C =(short*)calloc(arrayCount,sizeof(short));
		y36C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x36C[i] = (short)x[i];
			y36C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x36C[i],y36C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 37th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[36] = arrayCount;
		numOfArrays++;

		x37C =(short*)calloc(arrayCount,sizeof(short));
		y37C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x37C[i] = (short)x[i];
			y37C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x37C[i],y37C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 38th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[37] = arrayCount;
		numOfArrays++;

		x38C =(short*)calloc(arrayCount,sizeof(short));
		y38C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x38C[i] = (short)x[i];
			y38C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x38C[i],y38C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 39th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[38] = arrayCount;
		numOfArrays++;

		x39C =(short*)calloc(arrayCount,sizeof(short));
		y39C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x39C[i] = (short)x[i];
			y39C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x39C[i],y39C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 40th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[39] = arrayCount;
		numOfArrays++;

		x40C =(short*)calloc(arrayCount,sizeof(short));
		y40C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x40C[i] = (short)x[i];
			y40C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x40C[i],y40C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 41st ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[40] = arrayCount;
		numOfArrays++;

		x41C =(short*)calloc(arrayCount,sizeof(short));
		y41C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x41C[i] = (short)x[i];
			y41C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x41C[i],y41C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 42nd ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[41] = arrayCount;
		numOfArrays++;

		x42C =(short*)calloc(arrayCount,sizeof(short));
		y42C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x42C[i] = (short)x[i];
			y42C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x42C[i],y42C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 43rd ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[42] = arrayCount;
		numOfArrays++;

		x43C =(short*)calloc(arrayCount,sizeof(short));
		y43C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x43C[i] = (short)x[i];
			y43C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x43C[i],y43C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 44th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[43] = arrayCount;
		numOfArrays++;

		x44C =(short*)calloc(arrayCount,sizeof(short));
		y44C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x44C[i] = (short)x[i];
			y44C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x44C[i],y44C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 45th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[44] = arrayCount;
		numOfArrays++;

		x45C =(short*)calloc(arrayCount,sizeof(short));
		y45C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x45C[i] = (short)x[i];
			y45C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x45C[i],y45C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 46th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[45] = arrayCount;
		numOfArrays++;

		x46C =(short*)calloc(arrayCount,sizeof(short));
		y46C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x46C[i] = (short)x[i];
			y46C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x46C[i],y46C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 47th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[46] = arrayCount;
		numOfArrays++;

		x47C =(short*)calloc(arrayCount,sizeof(short));
		y47C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x47C[i] = (short)x[i];
			y47C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x47C[i],y47C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 48th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[47] = arrayCount;
		numOfArrays++;

		x48C =(short*)calloc(arrayCount,sizeof(short));
		y48C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x48C[i] = (short)x[i];
			y48C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x48C[i],y48C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 49th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[48] = arrayCount;
		numOfArrays++;

		x49C =(short*)calloc(arrayCount,sizeof(short));
		y49C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x49C[i] = (short)x[i];
			y49C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x49C[i],y49C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	if(endF == 0){
		//-----------------------------------
		//	GET 50th ARRAY OF VALUES
		//-----------------------------------
		for(i = 0; i < SIZE; i++){
			x[i] = 0;
			y[i] = 0;
		}
		
		getNextArray(fp, garbage, chG, x, y, &arrayCount);
		arraySizes[49] = arrayCount;
		numOfArrays++;

		x50C =(short*)calloc(arrayCount,sizeof(short));
		y50C =(short*)calloc(arrayCount,sizeof(short));

		for(i = 0; i < arrayCount; i++){
			x50C[i] = (short)x[i];
			y50C[i] = (short)y[i];
		
			//printf("X: %f, Y: %f \n", x[i],y[i]);
			//printf("X: %d, Y: %d \n", x50C[i],y50C[i]);
		}
	}

	//CHECK FOR FOOTER
	findFooter(fp,&endF);

	fclose(fp);

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
	waitForCommand(&nextArray);
	
	if(nextArray == 1 && numOfArrays > 1){
		//-----------------------------------
		//	Send 2nd array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x2C, y2C,1);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 2){
		//-----------------------------------
		//	Send 3rd array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x3C, y3C,2);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 3){
		//-----------------------------------
		//	Send 4th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x4C, y4C,3);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 4){
		//-----------------------------------
		//	Send 5th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x5C, y5C,4);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 5){
		//-----------------------------------
		//	Send 6th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x6C, y6C,5);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 6){
		//-----------------------------------
		//	Send 7th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x7C, y7C,6);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 7){
		//-----------------------------------
		//	Send 8th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x8C, y8C,7);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 8){
		//-----------------------------------
		//	Send 9th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x9C, y9C,8);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 9){
		//-----------------------------------
		//	Send 10th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x10C, y10C,9);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 10){
		//-----------------------------------
		//	Send 11th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x11C, y11C,10);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 11){
		//-----------------------------------
		//	Send 12th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x12C, y12C,11);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 12){
		//-----------------------------------
		//	Send 13th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x13C, y13C,12);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 13){
		//-----------------------------------
		//	Send 14th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x14C, y14C,13);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 14){
		//-----------------------------------
		//	Send 15th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x15C, y15C,14);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 15){
		//-----------------------------------
		//	Send 16th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x16C, y16C,15);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 16){
		//-----------------------------------
		//	Send 17th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x17C, y17C,16);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 17){
		//-----------------------------------
		//	Send 18th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x18C, y18C,17);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 18){
		//-----------------------------------
		//	Send 19th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x19C, y19C,18);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 19){
		//-----------------------------------
		//	Send 20th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x20C, y20C,19);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 20){
		//-----------------------------------
		//	Send 21st array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x21C, y21C,20);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 21){
		//-----------------------------------
		//	Send 22nd array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x22C, y22C,21);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 22){
		//-----------------------------------
		//	Send 23rd array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x23C, y23C,22);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 23){
		//-----------------------------------
		//	Send 24th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x24C, y24C,23);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 24){
		//-----------------------------------
		//	Send 25th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x25C, y25C,24);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 25){
		//-----------------------------------
		//	Send 26th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x26C, y26C,25);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 26){
		//-----------------------------------
		//	Send 27th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x27C, y27C,26);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 27){
		//-----------------------------------
		//	Send 28th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x28C, y28C,27);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 28){
		//-----------------------------------
		//	Send 29th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x29C, y29C,28);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 29){
		//-----------------------------------
		//	Send 30th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x30C, y30C,29);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 30){
		//-----------------------------------
		//	Send 31st array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x31C, y31C,30);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 31){
		//-----------------------------------
		//	Send 32nd array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x32C, y32C,31);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 32){
		//-----------------------------------
		//	Send 33rd array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x33C, y33C,32);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 33){
		//-----------------------------------
		//	Send 34th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x34C, y34C,33);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 34){
		//-----------------------------------
		//	Send 35th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x35C, y35C,34);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 35){
		//-----------------------------------
		//	Send 36th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x36C, y36C,35);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 36){
		//-----------------------------------
		//	Send 37th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x37C, y37C,36);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 37){
		//-----------------------------------
		//	Send 38th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x38C, y38C,37);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 38){
		//-----------------------------------
		//	Send 39th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x39C, y39C,38);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 39){
		//-----------------------------------
		//	Send 40th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x40C, y40C,39);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 40){
		//-----------------------------------
		//	Send 41st array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x41C, y41C,40);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 41){
		//-----------------------------------
		//	Send 42nd array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x42C, y42C,41);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 42){
		//-----------------------------------
		//	Send 43rd array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x43C, y43C,42);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 43){
		//-----------------------------------
		//	Send 44th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x44C, y44C,43);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 44){
		//-----------------------------------
		//	Send 45th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x45C, y45C,44);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 45){
		//-----------------------------------
		//	Send 46th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x46C, y46C,45);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 46){
		//-----------------------------------
		//	Send 47th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x47C, y47C,46);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 47){
		//-----------------------------------
		//	Send 48th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x48C, y48C,47);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 48){
		//-----------------------------------
		//	Send 49th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x49C, y49C,48);
		waitForCommand(&nextArray);
	}

	if(nextArray == 1 && numOfArrays > 49){
		//-----------------------------------
		//	Send 50th array
		//-----------------------------------
		nextArray = 0;
		sendArrayData(arraySizes, x50C, y50C,49);
		waitForCommand(&nextArray);
	}

	//SEND BACK TO ORIGIN
	nBytesSent = serial.SendData(finished, 2);

	if(nBytesSent == 0){
		printf("Trouble sending All Done Command\n", i);
	}

	printf("Going back to the origin...taking it's time\n");

	serial.Close();

	if(numOfArrays > 0){
		free(xC);
		free(yC);
	}
	if(numOfArrays > 1){
		free(x2C);
		free(y2C);
	}
	if(numOfArrays > 2){
		free(x3C);
		free(y3C);
	}
	if(numOfArrays > 3){
		free(x4C);
		free(y4C);
	}
	if(numOfArrays > 4){
		free(x5C);
		free(y5C);
	}
	if(numOfArrays > 5){
		free(x6C);
		free(y6C);
	}
	if(numOfArrays > 6){
		free(x7C);
		free(y7C);
	}
	if(numOfArrays > 7){
		free(x8C);
		free(y8C);
	}
	if(numOfArrays > 8){
		free(x9C);
		free(y9C);
	}
	if(numOfArrays > 9){
		free(x10C);
		free(y10C);
	}
	if(numOfArrays > 10){
		free(x11C);
		free(y11C);
	}
	if(numOfArrays > 11){
		free(x12C);
		free(y12C);
	}
	if(numOfArrays > 12){
		free(x13C);
		free(y13C);
	}
	if(numOfArrays > 13){
		free(x14C);
		free(y14C);
	}
	if(numOfArrays > 14){
		free(x15C);
		free(y15C);
	}
	if(numOfArrays > 15){
		free(x16C);
		free(y16C);
	}
	if(numOfArrays > 16){
		free(x17C);
		free(y17C);
	}
	if(numOfArrays > 17){
		free(x18C);
		free(y18C);
	}
	if(numOfArrays > 18){
		free(x19C);
		free(y19C);
	}
	if(numOfArrays > 19){
		free(x20C);
		free(y20C);
	}
	if(numOfArrays > 20){
		free(x21C);
		free(y21C);
	}
	if(numOfArrays > 21){
		free(x22C);
		free(y22C);
	}
	if(numOfArrays > 22){
		free(x23C);
		free(y23C);
	}
	if(numOfArrays > 23){
		free(x24C);
		free(y24C);
	}
	if(numOfArrays > 24){
		free(x25C);
		free(y25C);
	}
	if(numOfArrays > 25){
		free(x26C);
		free(y26C);
	}
	if(numOfArrays > 26){
		free(x27C);
		free(y27C);
	}
	if(numOfArrays > 27){
		free(x28C);
		free(y28C);
	}
	if(numOfArrays > 28){
		free(x29C);
		free(y29C);
	}
	if(numOfArrays > 29){
		free(x30C);
		free(y30C);
	}
	if(numOfArrays > 30){
		free(x31C);
		free(y31C);
	}
	if(numOfArrays > 31){
		free(x32C);
		free(y32C);
	}
	if(numOfArrays > 32){
		free(x33C);
		free(y33C);
	}
	if(numOfArrays > 33){
		free(x34C);
		free(y34C);
	}
	if(numOfArrays > 34){
		free(x35C);
		free(y35C);
	}
	if(numOfArrays > 35){
		free(x36C);
		free(y36C);
	}
	if(numOfArrays > 36){
		free(x37C);
		free(y37C);
	}
	if(numOfArrays > 37){
		free(x38C);
		free(y38C);
	}
	if(numOfArrays > 38){
		free(x39C);
		free(y39C);
	}
	if(numOfArrays > 39){
		free(x40C);
		free(y40C);
	}
	if(numOfArrays > 40){
		free(x41C);
		free(y41C);
	}
	if(numOfArrays > 41){
		free(x42C);
		free(y42C);
	}
	if(numOfArrays > 42){
		free(x43C);
		free(y43C);
	}
	if(numOfArrays > 43){
		free(x44C);
		free(y44C);
	}
	if(numOfArrays > 44){
		free(x45C);
		free(y45C);
	}
	if(numOfArrays > 45){
		free(x46C);
		free(y46C);
	}
	if(numOfArrays > 46){
		free(x47C);
		free(y47C);
	}
	if(numOfArrays > 47){
		free(x48C);
		free(y48C);
	}
	if(numOfArrays > 48){
		free(x49C);
		free(y49C);
	}
	if(numOfArrays > 49){
		free(x50C);
		free(y50C);
	}

	printf("The program has ended...\n");
	
	return 0;
}


void getNextArray(FILE *fp, char *garbage, char chG, float xN[], float yN[], int *arrayCount){ 
	int i = 0;
	int varRet = 0;
	float startX = 0;
	float startY = 0;

	//fscanf(fp, "%[^\n]", garbage);
	//chG = fgetc(fp);
	//chG = fgetc(fp);
	fscanf(fp, "%[^\n]", garbage);
	chG = fgetc(fp);

	fscanf(fp, "G00 X%f Y%f", &startX, &startY);

	//printf("\nX0: %f, Y0: %f \n", startX,startY);
	chG = fgetc(fp);
	chG = fgetc(fp);

	fscanf(fp, "%[^\n]", garbage);
	chG = fgetc(fp);
	
	xN[0] = startX;
	yN[0] = startY;
	(*arrayCount) = 1;
	i = 1;
	varRet = fscanf(fp, "G01 X%f Y%f", &xN[i], &yN[i]);
	while(varRet == 2){
		(*arrayCount)++;
		fscanf(fp, "%[^\n]", garbage);
		chG = fgetc(fp);
		i++;
		varRet = fscanf(fp, "G01 X%f Y%f", &xN[i], &yN[i]);
	}

	//for(i = 0; i < arrayCount; i++){
	//	printf("X: %f, Y: %f \n", xN[i],yN[i]);
	//}

}

void findFooter(FILE *fp, char *endF){

	char foundChar = 0;
	char ch1 = 0;
	char ch2 = 0;
	
	while(foundChar == 0){

		ch1 = fgetc(fp);
		ch2 = fgetc(fp);
		
		if(ch1 == 'G' && ch2 == '0'){
			foundChar = 1;
		}
		else if (ch1 == '0' && ch2 == 'G'){
			foundChar = 1;
		}
		else if(ch1 == 'F' && ch2 == 'o'){
			foundChar = 1;
			*endF = 1;
		}
		else if (ch1 == 'o' && ch2 == 'F'){
			foundChar = 1;
			*endF = 1;
		}
		else if (ch1 == EOF || ch2 == EOF){
			break;
		}

	}
}
	

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

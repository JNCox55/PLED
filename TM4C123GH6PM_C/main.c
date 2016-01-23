//System Control Base Address (P. 577)
unsigned int *SYSCTL = (unsigned int *) 0x400FE000;

//Port B Base Address (RX=PB.0, TX=PB.1) (P. 659)
unsigned int *PB = (unsigned int *) 0x40005000;

//Port E Base Address (RX=PB.0, TX=PB.1) (P. 659)
unsigned int *PE = (unsigned int *) 0x40024000;

//Port F Base Address (RX=PB.0, TX=PB.1) (P. 659)
unsigned int *PF = (unsigned int *) 0x40025000;

//UART1 Base Address: GPIODATA UART1 (P. 900)
unsigned int *UART1 = (unsigned int *) 0x4000D000;

// m3 core peripherals base
unsigned int *M3CP = (unsigned int *) 0xE000E000;

//Bit banding
//-----------------------------------------------------------
//Bit band address for PE.0
volatile unsigned int *PE_0 = (unsigned int *) 0x42487F80;

//Bit band address for PE.1
volatile unsigned int *PE_1 = (unsigned int *) 0x42487F84; 

//Bit band address for PF.0
volatile unsigned int *PF_0 = (unsigned int *) 0x424A7F80;

//Bit band address for PF.4
volatile unsigned int *PF_4 = (unsigned int *) 0x424A7F90;
//------------------------------------------------------------

//UART 1 Data reg./buffer UARTDR Offset 0x00 (P. 903)
volatile unsigned int UART1_Data __attribute__((at(0x4000D000)));

//UART1 buffer state flag reg. UARTFR Offset 0x018 (P. 908)
volatile unsigned int UART1_Bufferflag __attribute__((at(0x4000D018)));

// RCGC2 (for enabling clock to gpio port E)
volatile unsigned int RCGC2 __attribute__((at(0x400FE108)));

//Global stack--datasize=256 elements
volatile unsigned char stack[256];		//Set 1000 Stack

volatile unsigned int globalcount=0;	//# of interupts

volatile unsigned int count=0;				//used to see if we get a byte of data

volatile unsigned int flip=0;				//iterator to invert bit order from keyboard

volatile unsigned int data;					//byte of data to convert to ASCII

volatile unsigned int totaldatacount=0;	//iterator to compare with golbalcount

volatile unsigned int bufflag;				//used to store the buffer flag

//Interupt on falling clock edge of serial signal
//Grab high/low data form signal on PE.1
//Store data to array--stack
void UART1_Handler(void)			    //interrupt when we recieve a message
{
	PE[0x41C]=1;												//clear interrupt request
//	if (count==33)											//if we recieve a complete keyboard message...
//	{	
//		count=0;													//set count to prepare for next message
//	}
//	PE[0x41C]=1;												//clear interrupt request

//	if (count>22 && count<31)						//grab the character data
//	{
//		compare=compare<<1;								//shift over for the next bit
//		compare |= *PE_1;									//take the bit and OR it with the current
//	}
//	
//	count++;									//increment count until we get a full byte in compare
//	if (count==31)											//if we have the full message
//	{
//		for (flip=0;flip<8;flip++)				//start inverting the bit order
//		{
//			data=data<<1;										//move bits left for incoming bit
//			data|=compare%2;								//grab the LSB from compare, store in data
//			compare=compare>>1;							//get rid of old LSB
//		}
//		flip=0;														//reset flip
//		stack[globalcount]=data;					//store the data byte in the stack
//		globalcount++;										//increment the number of bytes
//	}
	return;															//exit interrupt
}


int main(void)
{
	int count=0;
	char GCodeOne[33]="\n\rBreanna is my best friend :-)\n\r";
	//Initialize/Configure UART and Port B
	//-------------------------------------------------------------------------
	
	//Enable System Clock: UART1-Port B
	SYSCTL[0x618/4] = 0x2;				//Enable clock on port B, E, F
  SYSCTL[0x104/4] = 0x2; 				//Enable UART1
  SYSCTL[0x108/4] = 0x32; 			//Enable Port B, E, F

  //Enable alt. func. on PB
  PB[0x420/4] = 0x3; 						//Alternative functionality
  PB[0x51C/4] = 0x3; 						//PB.0/PB.1
	PB[0x52C/4] =	0x11;						//Set UART
  
  //Disable UART1
  UART1[0x30/4] = 0x0;				//Disable UART1
  
  //Baudrate divisor
  //Divisor = 16.0e6/(16*9600)= 100.195
	//-------------------------------------------
  //Integer portion: int(100.00)=104.167
  UART1[0x24/4] = 0x68;
  //Fractional portion: int(0.167*2^6+0.5)=11
  UART1[0x28/4] = 0xB;
	//-------------------------------------------
  
  //Set serial parameters: Start/8-bit data/Stop
  UART1[0x2C/4] = 0x70;
	
	//Set SysClck as the UART clock
	UART1[0xFC8/4] = 0x0;
  
  //Enable UART1: Transmit and recieve
  UART1[0x30/4] = 0x301;
	
	
	
		//Setup the interrupts on GPIOE and GPIOF
	
//	// 2. edge triggering Interupt sense GPIOIS (P. 661)
//	PE[0x404] = 0;		//Edge triggering for interupt
//	PF[0x404] = 0;		//Edge triggering for interupt
//	
//	// 3. Falling edge GPIOIBE (P. 662)
//	PE[0x408] = 0; 		//don't want both edges
//	PE[0x40C] = 0; 		//Falling edge
//	
//	// 3. Falling edge GPIOIBE (P. 662)
//	PF[0x408] = 0;		 //don't want both edges
//	PF[0x40C] = 0; 		//Falling edge
//	
//	// 4. enable pin interrupt
//	PE[0x410] = 1;		//enable the interupt on PB0
//	PF[0x410] = 0x10; //enable th interupt on PF4
	
	// 5. enable interrupt from port E: vector number 20, interrupt num 4
	//  interrupt num 3 is in first byte of EN0
	// Reg map 3.2 page 132 tabel 3-8//Page 102 table 2-9,
//	M3CP[0x100/4] = 0x40;			//Enable UART1 interupt

	
		for(count=0; count<33; count++) {
		UART1_Data=GCodeOne[count];
		
		//Monitor UART 1 buffer flag
		//Wait for UART1 buffer to clear from previous data send
		bufflag=UART1_Bufferflag;						//grab the whole flag register
		bufflag &= 0x20;										//AND it with 0x20 to see if the buffer is full
		
		while(bufflag==0x20)								//if it is full wait
		{
		bufflag=UART1_Bufferflag;						//poll buffer
		bufflag &= 0x20;										//Do nothing while buffer clears	
		}	
	}
	

	while(1);
}

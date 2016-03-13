//#include "inc\tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
//#include <string.h>
#include "driverlib\pwm.h"
#include "inc\hw_memmap.h"
#include "driverlib\gpio.h"
#include "driverlib\pin_map.h"
#include "driverlib\sysctl.h"
#include "inc\hw_ints.h"
#include "driverlib\debug.h"
#include "driverlib\fpu.h"
#include "driverlib\interrupt.h"
#include "driverlib\rom.h"
#include "driverlib\qei.h"
#include "driverlib\uart.h"
#include "inc\hw_gpio.h"
#include "inc\hw_types.h"
#include "driverlib\fpu.h"

volatile unsigned int *UART1=(unsigned int *) 0x4000D000; //This points to the base address for UART1
int timeEngrave=67;  //This is the denominator for the fraction of a second we are engraving
#define timeRest 0.5 //This is the denominator for the fraction of a second we rest between burns
#define QEIMaxPosition 0xFFFFFFFF //This is the maximum count for the encoder(s) to accumulate pulses to.

char laserKey=0;
int xPulsesSent;
int yPulsesSent;
int encoderPositionX;
int encoderPositionY;

char identifier[2];
uint32_t tempPosition;
short moveX;
short moveY;
short dwellTime;

char command[10];
int commandIndex=0;
uint32_t size=0;
short sizeColumns=0;
short sizeRows=0;
int i=0;
int j=0;
char stop[5]="stop";
char go[3]="go";
char done=0;
char start=0;
char readyToGo=0;
uint32_t ui32Status;

int32_t mytest;
uint32_t positionX;
uint32_t positionY;
//char drawingBuffer[10000];
short xCommands[1612];		//holds the desired x coordinates for an entire row
int xCommandsEnd;		//points to the end of the xCommand data
int xCommandsIndex;		//points to the current x command to be moved to
short yCommands[1612];		//holds the desired y coordinates for an entire row
int yCommandsEnd;		//points to the end of the yCommand data
int yCommandsIndex;		//points to the current y command to be moved to
char gCode[16012];	//enough space to store 2 chars per gcode*(1600 gcodes possible per row + jog to home command + end of program/row command + initial G04 code)
int gCodeEnd;		//points to the end of the gCode data
int gCodeIndex=0;		//points to the current gCode to be excecuted
short pauseValues[1612];	//holds the pause durations for the G04 Commands
int pauseValuesEnd=0;	//points to the end of the pauseValues data
int pauseValuesIndex=0; //points to the current pause time duration to dwell

void dwell()
{
	//THIS FUNCTION CAUSES THE LASER TO DWELL IN ITS CURRENT STATE
	SysCtlDelay((int) (SysCtlClockGet()*dwellTime/(3000.0)));
	return;
}

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void
UART1_Handler(void)
{
    //
    // Get the interrrupt status.
    //
    ui32Status = UARTIntStatus(UART1_BASE, true);
    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART1_BASE, ui32Status);
		// Grab the first byte of the identifier that tells us what type of G Code instruction we are getting	
		identifier[0]=UARTCharGetNonBlocking(UART1_BASE);
		//	Grab the second part of the identifier
		identifier[1]=UARTCharGetNonBlocking(UART1_BASE);
    //
    // Loop while there is an instruction in the receive FIFO.
    //
		if (identifier[0]=='Z' && identifier[1]=='Z')	//If we recieve the command to collect the size of the image...
		{
			size=0x00000000;
			sizeRows=0x0000;
			sizeColumns=0x0000;
			while(UARTCharsAvail(UART1_BASE) && i<4) //check to see if we have filled up our size integer with 4 bytes, i keeps count
			{
				size|=UARTCharGetNonBlocking(UART1_BASE);	//Grab a byte from the fifo buffer
				if (i==3)	//When we know all the parts of the sizing dimension information...
				{
					sizeRows=size&~0xFFFF0000;	//grab the size of the rows
					sizeColumns=size>>16;	//grab the size of the columns
				}
				size=size<<8;	//make room for the next byte
				i++;	//increment the number of bytes
			}
			i=0;	//reset i for next use
			return;
		}
		
		if (identifier[0]=='G' && identifier[1]=='0')	//If we recieve the command to jog to home...
		{
			tempPosition=0x00000000;
			moveY=0x0000;
			moveX=0x0000;
			while(UARTCharsAvail(UART1_BASE) && i<4) //check to see if we have filled up our size integer with 4 bytes, i keeps count
			{
				tempPosition|=UARTCharGetNonBlocking(UART1_BASE);	//Grab a byte from the fifo buffer
				if (i==3)	//When we know all the parts of the sizing dimension information...
				{
					moveY=tempPosition&~0xFFFF0000;	//grab the size of the rows
					moveX=tempPosition>>16;	//grab the size of the columns
				}
				tempPosition=tempPosition<<8;	//make room for the next byte
				i++;	//increment the number of bytes
			}
			xCommands[xCommandsEnd]=moveX;		//Store the x command in the buffer
			yCommands[yCommandsEnd]=moveY;		//Store the y command in the buffer
			xCommandsEnd++;	//move the index of the end of the xCommandBuffer to point to one past the last entry
			yCommandsEnd++;	//move the index of the end of the yCommandBuffer to point to one past the last entry
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			i=0;	//reset i for next use
			return;
		}
				
		if (identifier[0]=='G' && identifier[1]=='1')	//If we recieve the command to interpolate to the next position...
		{
			tempPosition=0x00000000;
			moveX=0;
			moveY=0;
			while(UARTCharsAvail(UART1_BASE) && i<4) //check to see if we have filled up our size integer with 4 bytes, i keeps count
			{
				tempPosition|=UARTCharGetNonBlocking(UART1_BASE);	//Grab a byte from the fifo buffer
				if (i==3)	//When we know all the parts of the sizing dimension information...
				{
					moveY=tempPosition&~0xFFFF0000;	//grab the size of the rows
					moveX=tempPosition>>16;	//grab the size of the columns
				}
				tempPosition=tempPosition<<8;	//make room for the next byte
				i++;	//increment the number of bytes
			}
			xCommands[xCommandsEnd]=moveX;		//Store the x command in the buffer
			yCommands[yCommandsEnd]=moveY;		//Store the y command in the buffer
			xCommandsEnd++;	//move the index of the end of the xCommandBuffer to point to one past the last entry
			yCommandsEnd++;	//move the index of the end of the yCommandBuffer to point to one past the last entry
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			i=0;	//reset i for next use
			return;
		}		
		
		if (identifier[0]=='G' && identifier[1]=='4')	//If we recieve the command to dwell at the current position...
		{
			dwellTime=0x0000;
			if(UARTCharsAvail(UART1_BASE)) //check to see if we have filled up our size integer with 4 bytes, i keeps count
			{
				dwellTime|=UARTCharGetNonBlocking(UART1_BASE);	//Grab a byte from the fifo buffer
				dwellTime=dwellTime<<8;	//make room for the next byte
				dwellTime|=UARTCharGetNonBlocking(UART1_BASE);  //Grab the second byte from the fifo buffer
				pauseValues[pauseValuesEnd]=dwellTime;
				pauseValuesEnd++;
				gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
				gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
				gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			}
			return;
		}	
		
		if (identifier[0]=='M' && identifier[1]=='4')	//If we recieve the command to turn the laser on...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}	
		
		if (identifier[0]=='M' && identifier[1]=='5')	//If we recieve the command to turn the laser off...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}	
		
		if (identifier[0]=='S' && identifier[1]=='0')	//If we recieve the command to set the laser at full intensity...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}	
		
		if (identifier[0]=='S' && identifier[1]=='1')	//If we recieve the command to set the laser 6/7 intensity...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}

		if (identifier[0]=='S' && identifier[1]=='2')	//If we recieve the command to set the laser at 5/7 intensity...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}	

		if (identifier[0]=='S' && identifier[1]=='3')	//If we recieve the command to set the laser at 4/7 intensity...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}	

		if (identifier[0]=='S' && identifier[1]=='4')	//If we recieve the command to set the laser at 3/7 intensity...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}			
		
		if (identifier[0]=='S' && identifier[1]=='5')	//If we recieve the command to set the laser at 2/7 intensity...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}	
		
		if (identifier[0]=='S' && identifier[1]=='6')	//If we recieve the command to set the laser at 1/7 intensity...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}	
		
		if (identifier[0]=='S' && identifier[1]=='7')	//If we recieve the command to set the laser at no power...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}	
		
		if (identifier[0]=='M' && identifier[1]=='2')		//If we recieve the command that the picture is complete...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
		}

				
			    /*while(UARTCharsAvail(UART1_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
				//mytest=UARTCharGetNonBlocking(UART1_BASE);
        //UARTCharPutNonBlocking(UART1_BASE,
        //                           mytest);
				drawingBuffer[head]=(char) UARTCharGetNonBlocking(UART1_BASE);
				//point to the next available spot IF we dont overrun the buffer
				if (head<9998 && (head+1)!=tail)
				{
					head++;
				}
				//wrap to the beginning of the buffer IF we dont overrun the buffer
				else if (!(head==9999 && tail==0))
				{
					head=0;
				}
				//If we are about to overrun the buffer, tell the PC to stop sending
				if ((head==9999 && tail==0) || (head+1)==tail)
				{
					while (UARTBusy(UART1_BASE) != 0)
					{
						//wait
					}
					for (i=0;i<3;i++)
					{
						UARTCharPutNonBlocking(UART1_BASE,
																			 go[i]);
					}
				}
        //
        // Blink the LED to show a character transfer is occuring.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));

        //
        // Turn off the LED
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }*/
}

void Sys_Clock_Set()
{
	int i;
	//
	// Set the clocking to run directly from the external crystal/oscillator.
	//
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	for (i=0;i<2000;i++)
		{
			//Wait while the clock frequency change settles
		}
	//
	// Can be used to check the clock frequency
	//
	mytest=SysCtlClockGet();
	return;
}

void PWM_Setup()
{
	//
	// Set the PWM clock to the system clock.
	//	
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	//
	// The PWM peripheral must be enabled for use.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	//
	// PWM0 is used with PB6 and PB7.
	// GPIO port B needs to be enabled so these pins can be used.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	//
	// Configure the GPIO pin muxing to select PWM00/PWM01 functions for these pins.
	// This step selects which alternate function is available for these pins.
	//
	GPIOPinConfigure(GPIO_PB6_M0PWM0);
	// | GPIO_PB7_M0PWM1
	//
	// Configure the PWM function for these pins.
	//
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
	//
	// Configure the PWM generator for count down mode with immediate updates
	// to the parameters.
	//
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	//
	// Set the PWM period to 250Hz.  To calculate the appropriate parameter
	// use the following equation: N = (1 / f) * SysClk.  Where N is the
	// function parameter, f is the desired frequency, and SysClk is the
	// system clock frequency.
	// In this case you get: (1 / 250Hz) * 16MHz = 64000 cycles.  Note that
	// the maximum period you can set is 2^16.
	//
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 480); //3813
	//
	// Set PWM0 to a duty cycle of 25%.  You set the duty cycle as a function
	// of the period.  Since the period was set above, you can use the
	// PWMGenPeriodGet() function.  For this example the PWM will be high for
	// 25% of the time or 16000 clock ticks (64000 / 4).
	//
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1);  //1907
	//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, (PWMGenPeriodGet(PWM0_BASE, PWM_OUT_1)*3) / 4);
	//
	// Enable the PWM0 Bit0 (PB6) and PWM1 Bit0 (PB7) output.
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
	// | PWM_OUT_1_BIT
	//
	// Enable the PWM generator block.
	//
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	return;
}

void UART1_Setup()
{
	//
	// Enable the GPIO port that is used for the on-board LED. (and UART1 CTS)
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	//
	// Enable the GPIO pins for the LED (PF2).
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
	//
	// Enable the peripherals for the LED and UART1.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	//
	// Enable processor interrupts.
	//
	IntMasterEnable();
	//
	// Set GPIO PB0 and PB1 as RX and TX on UART1, and
	// PC4 and PF1 as RTS and CTS for UART1
	//
	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinConfigure(GPIO_PF1_U1CTS);
	GPIOPinConfigure(GPIO_PC4_U1RTS);
	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4);
	GPIOPinTypeUART(GPIO_PORTF_BASE, GPIO_PIN_1);
	//
	// Configure the UART for 9600 8-N-1 operation.
	//
	UARTClockSourceSet(UART1_BASE, UART_CLOCK_SYSTEM);
	
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,
													(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
													 UART_CONFIG_PAR_NONE));
	//
	// Configure the UART for interrupts on < 7/8 of the TX buffer empty and > 1/2 RX full.
	//
	UARTFIFOLevelSet(UART1_BASE,UART_FIFO_TX7_8,UART_FIFO_RX4_8);
	//
	// Configure the UART to use RTS and CTS handshaking.
	//
	UARTFlowControlSet(UART1_BASE, UART_FLOWCONTROL_RX | UART_FLOWCONTROL_TX);
	//UART1[0x30]|=0x0000C000;
	//
	// Enable the UART interrupt.
	//
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
	UARTEnable(UART1_BASE);
}

void QEI_Setup()
{
	//
	// QEI0 is used with PC5 and PC6 and QEI1 is used with PD6 and PD7.
	// GPIO ports C and D need to be enabled so these pins can be used.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);  //already done in UART1 setup
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//
	// Enable the peripherals for QEI0 and QEI1.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
	//
	// Unlock GPIO D7
	//
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
	//
	// Set GPIO PD6 and PD7 as PhA and PhB on QEI0, and
	// PC5 and PC6 as PhA and PhB for QEI1
	//
	GPIOPinConfigure(GPIO_PD6_PHA0);
	GPIOPinConfigure(GPIO_PD7_PHB0);
	GPIOPinConfigure(GPIO_PC5_PHA1);
	GPIOPinConfigure(GPIO_PC6_PHB1);
	GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);
	//
	// Disable the encoders before configuration
	//
	QEIDisable(QEI0_BASE);
	QEIDisable(QEI1_BASE);
	//
	// Configure the UART for 9600 8-N-1 operation.
	//
	QEIConfigure(QEI0_BASE,QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP,QEIMaxPosition);
	QEIConfigure(QEI1_BASE,QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP,QEIMaxPosition);
	//
	// Enable the encoders
	//
	QEIEnable(QEI0_BASE);
	QEIEnable(QEI1_BASE);
}

void GPIO_Setup()
{
	//
	// GPIO PD0:PD1 are used for toggling the forward and reverse step on the stepper motor drivers.
	// GPIO PD2:PD3 are used for sending the step signal to the stepper motor drivers.
	// Enable port D so these pins can be used.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//
	// Enable the GPIO pins for controlling the stepper motors (PD0:PD3).
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void engrave()
{
	/*while (done==0)
	{
//		end9999=drawingBuffer[9999];
//		end9998=drawingBuffer[9998];
//		end9997=drawingBuffer[9997];
		if ((head>tail && (head-tail)<500) || (head<tail && (tail-head)>8499))
		{
			//test=UARTBusy(UART1_BASE);
			while (UARTBusy(UART1_BASE) != 0)
			{
				//wait
			}
			for (i=0;i<3;i++)
			{
				UARTCharPutNonBlocking(UART1_BASE,
                                   go[i]);
			}
			//SysCtlDelay(SysCtlClockGet() / (1 * 3));
		}
		// Engraving instruction commands
		if(drawingBuffer[tail]=='G' && drawingBuffer[tail+1]=='0' && drawingBuffer[tail+2]=='1')
		{
			//engrave at intensity level 0
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 411);
			//engrave for x milliseconds
			SysCtlDelay((int) ((SysCtlClockGet()/timeEngrave) / 3));
			//turn the laser off
			//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1);
			//wait before turning the laser back on
			SysCtlDelay(SysCtlClockGet()*1 / (3));
			
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1850);
			//SysCtlDelay(SysCtlClockGet() / (74 * 3));
		}
		else if(drawingBuffer[tail]=='G' && drawingBuffer[tail+1]=='0' && drawingBuffer[tail+2]=='2')
		{
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 5);
			//SysCtlDelay(SysCtlClockGet() / (167 * 3));
		}
		//loop back to the start of the buffer
		if (tail==9996 && head!=9996)
		{
			tail=0;
		}
		//move to the next instruction
		else
		{
			tail=tail+3;
		}
		//we can't set the tail to equal the head+1
		while (tail==9996 && head==9996)
		{
			for (i=0;i<3;i++)
			{
				UARTCharPutNonBlocking(UART1_BASE,
                                   go[i]);
			}
			SysCtlDelay(SysCtlClockGet() / (10 * 3));
		}
		//If we are out of new pixels to engrave, wait for more.
		while (tail<head && (tail+3)>head)
		{
			for (i=0;i<3;i++)
			{
				UARTCharPutNonBlocking(UART1_BASE,
                                   go[i]);
			}
			SysCtlDelay(SysCtlClockGet() / (10 * 3));
		}
	}*/
}

/*void ready()
{
	while (readyToGo==0)
	{
		while (start==0)  //First check and see if we recieve a Z for size
		{
			if(UARTCharsAvail(UART1_BASE))	//wait until there is a character available in the UART buffer
			{
				if (UARTCharGetNonBlocking(UART1_BASE)=='Z')	//The Z indicates what follows is 4 characters of size information
					start=1;	//set the flag to get the size
			}
		}
		while(UARTCharsAvail(UART1_BASE))	//grab the 4 size characters in order of Column MSB, Column LSB, Row MSB, Row LSB
		{
			if(i<=4) //check to see if we have filled up our size integer with 4 bytes, i keeps count
			{
				size|=UARTCharGetNonBlocking(UART1_BASE);
				size=size<<8;
				i++;
			}
			if (i==4)
			{
				sizeRows=size&~0xFFFF0000;
				sizeColumns=size>>16;
				readyToGo=1;
				i=0;
			}
			//SysCtlDelay(SysCtlClockGet() / (10 * 3));
			//positionX=QEIPositionGet(QEI0_BASE);
			//positionY=QEIPositionGet(QEI1_BASE);
		}
	}
}*/

void testBenchPulse()
{
	//This test pulses the laser in succession on a point
	SysCtlDelay((int) (SysCtlClockGet()*2)/3);
	for (j=7;j>0;j--)
	{
		for (i=0;i<j;i++)
		{
			//Turn the laser on at full power
			PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,479);
			//delay for x milliseconds
			//SysCtlDelay((int) ((SysCtlClockGet()/timeEngrave)/3));
			//turn the laser off to rest
			PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,1);
			//wait while we rest
			SysCtlDelay((int) (SysCtlClockGet()/timeRest)/3);
		}
		//wait 1 second while we move the wood sample to a new spot
		SysCtlDelay((int) (SysCtlClockGet())/3);
	}
}

void testBenchDuration()
{
	//THIS TEST VARIES THE DURATION OF EQUAL INTENTSITY PULSES
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6);
	SysCtlDelay((int) (SysCtlClockGet()*2)/3);
	for (i=9;i<16;i++)
	{
		//Turn the laser on at full power
		//PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,799);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_PIN_6);
		//delay for j*x milliseconds
		//for (j=0;j<=i;j++)
		{
		SysCtlDelay((int) (i*(SysCtlClockGet()/1000)/3.0));
		}
		//turn the laser off to rest
		//PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,1);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);
		//wait while we rest
		SysCtlDelay((int) (SysCtlClockGet()/timeRest)/3);
	}
}

void testBenchIntensity()
{
	//THIS CODE RUNS A TEST OF VARYING LASER INTENSITY
	//delay 2 seconds before first engraving
	SysCtlDelay((SysCtlClockGet()*2)/3);
	for (i=7;i>3;i--)
	{
		//start off at the high end and engrave in decreasing fractions of denominator 7 out of 100% power
		j= (int) (479.0*i/7.0);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,j);
		//engrave for 1/timeEngrave seconds
		SysCtlDelay((int) ((SysCtlClockGet()/timeEngrave) / 3));
		// turn the laser off
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1);
		// wait for 2 seconds while we slide wood over
		SysCtlDelay(SysCtlClockGet()*2 / (3));
		/*if (i==3)
			timeEngrave=50;
		if (i==2)
			timeEngrave=20;*/
	}
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,479);
		//engrave for 1/timeEngrave seconds
		SysCtlDelay((int) ((SysCtlClockGet()/100.0) / 3));
		// turn the laser off
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1);
		// wait for 2 seconds while we slide wood over
		SysCtlDelay(SysCtlClockGet()*2 / (3));
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,479);
		//engrave for 1/timeEngrave seconds
		SysCtlDelay((int) ((SysCtlClockGet()/125.0) / 3));
		// turn the laser off
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1);
		// wait for 2 seconds while we slide wood over
		SysCtlDelay(SysCtlClockGet()*2 / (3));
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,479);
		//engrave for 1/timeEngrave seconds
		SysCtlDelay((int) ((SysCtlClockGet()/200) / 3));
		// turn the laser off
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1);
		// wait for 2 seconds while we slide wood over
		SysCtlDelay(SysCtlClockGet()*2 / (3));
}

void testBenchMotor()
{
	//THIS CODE ROTATES THE MOTOR ONE FULL REVOLUTION ONE DIRECTION THE SWITCHES
	//
	// Enable the GPIO port that is used to send the clock signal to the motors and direction
	// PD0:PD1 are forward or reverse and PD2:PD3 are single step
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//
	// Enable the GPIO pins
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	//
	// Start a loop to rotate the motor both directions one full revolution
	//
//	for (j=0;j<2;j++)
//	{
//		positionX=QEIPositionGet(QEI0_BASE);
//		positionY=QEIPositionGet(QEI1_BASE);
//		if (j%2==0)
//		{
//		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
//		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
//		}
//		else
//		{
//		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0);
//		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);
//		}
//		for (i=0;i<1600;i++)
//		{
//		//
//		// Delay for 2.5 millisecond.  Each SysCtlDelay is about 3 clocks.
//		//
//		SysCtlDelay(SysCtlClockGet() / (400 * 3));
//		//
//		// Bring the clocks high.
//		//
//			positionX=QEIPositionGet(QEI0_BASE);
//			positionY=QEIPositionGet(QEI1_BASE);
//		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
//		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);
//		//
//		// Delay for 2.5 millisecond.  Each SysCtlDelay is about 3 clocks.
//		//
//		SysCtlDelay(SysCtlClockGet() / (400 * 3));
//		//
//		// Bring the clocks low.
//		//
//			positionX=QEIPositionGet(QEI0_BASE);
//			positionY=QEIPositionGet(QEI1_BASE);
//		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
//		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
//		}
//	}

while (1)
{
		positionX=QEIPositionGet(QEI0_BASE);
		positionY=QEIPositionGet(QEI1_BASE);
		if (positionY<0x88888888 && positionY>0x10)
		{
			//set the direction
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0);
			//set the clock output high
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
			//wait 25 milliseconds
			SysCtlDelay(SysCtlClockGet() / (4000 * 3));
			//Set the clock output low
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
			//wait 25 milliseconds
			SysCtlDelay(SysCtlClockGet() / (4000 * 3));
		}
		else if (positionY>0x88888888 && positionY<0xFFFFFFEF)
		{
			//set the direction
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
			//set the clock output high
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
			//wait 25 milliseconds
			SysCtlDelay(SysCtlClockGet() / (4000 * 3));
			//Set the clock output low
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
			//wait 25 milliseconds
			SysCtlDelay(SysCtlClockGet() / (4000 * 3));
		}
			if (positionX<0x88888888 && positionX>0x10)
		{
			//set the direction
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);
			//set the clock output high
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);
			//wait 25 milliseconds
			SysCtlDelay(SysCtlClockGet() / (4000 * 3));
			//Set the clock output low
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
			//wait 25 milliseconds
			SysCtlDelay(SysCtlClockGet() / (4000 * 3));
		}
		else if (positionX>0x88888888 && positionX<0xFFFFFFEF)
		{
			//set the direction
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
			//set the clock output high
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);
			//wait 25 milliseconds
			SysCtlDelay(SysCtlClockGet() / (4000 * 3));
			//Set the clock output low
			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
			//wait 25 milliseconds
			SysCtlDelay(SysCtlClockGet() / (4000 * 3));
		}
}
}

int main(void)
{
	Sys_Clock_Set();
	FPUEnable();
	UART1_Setup();
	//PWM_Setup();
	//QEI_Setup();
	//GPIO_Setup();

	//ready();
	//engrave();
	//testBenchIntensity();
	//testBenchMotor();
	//testBenchPulse();
	//testBenchDuration(); //dont use with PWM_Setup()!!!
	
	while(1)
	{
		//positionX=QEIPositionGet(QEI0_BASE);
		//positionY=QEIPositionGet(QEI1_BASE);
//			for (i=0;i<3;i++)
//			{
//				UARTCharPutNonBlocking(UART1_BASE,
//                                   go[i]);
//			}
//			SysCtlDelay(SysCtlClockGet() / (1 * 3));
	}
}

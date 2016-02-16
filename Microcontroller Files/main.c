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

volatile unsigned int *UART1=(unsigned int *) 0x4000D000;
bool shoot;

char end9999;
char end9998;
char end9997;

//char gCode[3];
char command[10];
int i=0;
int tail=0;
int head=0;
char stop[5]="stop";
char go[3]="go";
char done=0;
char start=0;
#define QEIMaxPosition 0xFFFFFFFF

int32_t mytest;
uint32_t positionX;
char drawingBuffer[10000];

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void
UART1_Handler(void)
{
    uint32_t ui32Status;
    //
    // Get the interrrupt status.
    //
    ui32Status = UARTIntStatus(UART1_BASE, true);
    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART1_BASE, ui32Status);
    //
    // Loop while there are characters in the receive FIFO.
    //
    while(UARTCharsAvail(UART1_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
				//mytest=UARTCharGetNonBlocking(UART1_BASE);
        //UARTCharPutNonBlocking(UART1_BASE,
        //                           mytest);
			drawingBuffer[head]=(char) UARTCharGetNonBlocking(UART1_BASE);
			if (head<10000)
			{
				head++;
			}
			else
			{
				head=0;
			}
			//SysCtlDelay(SysCtlClockGet() / (10 * 3));
//			if ((tail-head)==60)
//			{
//				for (i=0;i<5;i++)
//				{
//					UARTCharPutNonBlocking(UART1_BASE,
//                                   stop[i]);
//				}
//			}
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
    }
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
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 3813);
	//
	// Set PWM0 to a duty cycle of 25%.  You set the duty cycle as a function
	// of the period.  Since the period was set above, you can use the
	// PWMGenPeriodGet() function.  For this example the PWM will be high for
	// 25% of the time or 16000 clock ticks (64000 / 4).
	//
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1907);
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
	//
	// Enable the GPIO pins for the LED (PF2).
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
	//
	// Enable the peripherals for the LED and UART1.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
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
	// Configure the UART for interrupts on < 1/8 of the TX buffer empty and > 3/4 RX full.
	//
	UARTFIFOLevelSet(UART1_BASE,UART_FIFO_TX1_8,UART_FIFO_RX6_8);
	//
	// Configure the UART to use RTS and CTS handshaking.
	//
	UART1[0x30]|=0x0000C000;
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
	// Configure the UART for 9600 8-N-1 operation.
	//
	QEIConfigure(QEI0_BASE,QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP,QEIMaxPosition);
	QEIConfigure(QEI1_BASE,QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP,QEIMaxPosition);
	QEIEnable(QEI0_BASE);
	QEIEnable(QEI1_BASE);
}

void engrave()
{
	while (done==0)
	{
		end9999=drawingBuffer[9999];
		end9998=drawingBuffer[9998];
		end9997=drawingBuffer[9997];
		if ((head-tail)<500)
		{
			while (UARTBusy(UART1_BASE) == true)
			{
				//wait
			}
			for (i=0;i<3;i++)
			{
				UARTCharPutNonBlocking(UART1_BASE,
                                   go[i]);
			}
				//SysCtlDelay(SysCtlClockGet() / (7 * 3));
		}
		if(drawingBuffer[tail]=='G' && drawingBuffer[tail+1]=='0' && drawingBuffer[tail+2]=='1')
		{
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1850);
			//SysCtlDelay(SysCtlClockGet() / (74 * 3));
		}
		else if(drawingBuffer[tail]=='G' && drawingBuffer[tail+1]=='0' && drawingBuffer[tail+2]=='2')
		{
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 5);
			//SysCtlDelay(SysCtlClockGet() / (167 * 3));
		}
		if (tail>9999)
		{
			tail=0;
		}
		else
		{
			tail=tail+3;
		}
	}
}

void ready()
{
	while (start==0)
	{
//		while(UARTCharsAvail(UART1_BASE))
//		{
//			command[i]=(char) UARTCharGetNonBlocking(UART1_BASE);
//			i++;
//		}
		if (command[0]=='g' && command[1]=='o')
		{
			start=1;
		}
		SysCtlDelay(SysCtlClockGet() / (10 * 3));
		positionX=QEIPositionGet(QEI1_BASE);
		shoot=QEIErrorGet(QEI1_BASE);
	}
}

int main(void)
{
	Sys_Clock_Set();
	//UART1_Setup();
	//PWM_Setup();
	QEI_Setup();
//	//
//	// Enable the GPIO port that is used for the on-board LED. (and UART1 CTS)
//	//
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//	//
//	// Enable the GPIO pins for the LED (PF2).
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6);
//	//
//	// Blink the LED to show a character transfer is occuring.
//	//
//	for (i=0;i<400;i++)
//	{
//	SysCtlDelay(SysCtlClockGet() / (400 * 3));
//	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_PIN_6);

//	//
//	// Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
//	//
//	SysCtlDelay(SysCtlClockGet() / (400 * 3));

//	//
//	// Turn off the LED
//	//
//	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);
//	}

	mytest=SysCtlClockGet();
	ready();
	engrave();
	while(1)
	{

	}
}

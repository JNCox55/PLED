	char test_keys[]={'G','1','\0','\0','\0','\0','S','3','M','4','G','4','\0','\0','M','5'};
	char gCodeCycleIndex=0;
	
	
	//
    // Get the interrrupt status.
    //
    ui32Status = UARTIntStatus(UART1_BASE, true);
    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART1_BASE, ui32Status);
		UARTRxErrorClear(UART1_BASE);
		// Grab the first byte of the identifier that tells us what type of G Code instruction we are getting	
	firstIdentifier:
			temp=UARTCharGet(UART1_BASE);
			if ((0xFF-temp)!=0)		//Remove potential null terminating characters
				identifier[0]=temp;
			else
				goto firstIdentifier;
				//identifier[0]=UARTCharGetNonBlocking(UART1_BASE);
			//	Grab the second part of the identifier
	secondIdentifier:
			temp=UARTCharGet(UART1_BASE);
			if ((0xFF-temp)!=0)		//Remove potential null terminating characters
				identifier[1]=temp;
			else
				goto secondIdentifier;
				//identifier[1]=UARTCharGetNonBlocking(UART1_BASE);
		//Put data in the appropriate arrays
		if (identifier[0]=='Z' && identifier[1]=='Z')	//If we recieve the command to collect the size of the image...
		{
			size=0x00000000;
			sizeRows=0x0000;
			sizeColumns=0x0000;
			while(UARTCharsAvail(UART1_BASE) && i<4) //check to see if we have filled up our size integer with 4 bytes, i keeps count
			{
				size|=UARTCharGet(UART1_BASE);	//Grab a byte from the fifo buffer
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
		
		if (identifier[0]=='G' && identifier[1]=='0')	//If we recieve the command to jog to a given coordinate...
		{
			tempPosition=0x00000000;
			moveY=0x0000;
			moveX=0x0000;
			while(UARTCharsAvail(UART1_BASE) && i<4) //check to see if we have filled up our size integer with 4 bytes, i keeps count
			{
				tempPosition|=UARTCharGet(UART1_BASE);	//Grab a byte from the fifo buffer
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
			moveX=0x0000;
			moveY=0x0000;
			i=0;
			while(UARTCharsAvail(UART1_BASE) && i<4) //check to see if we have filled up our size integer with 4 bytes, i keeps count
			{
				tempPosition|=UARTCharGet(UART1_BASE);	//Grab a byte from the fifo buffer
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
				dwellTime|=UARTCharGet(UART1_BASE);	//Grab a byte from the fifo buffer
				dwellTime=dwellTime<<8;	//make room for the next byte
				temp=UARTCharGetNonBlocking(UART1_BASE);	//Grab the next byte from the fifo buffer
				if (temp!='\0')		//Remove potential null terminating characters
					dwellTime|=temp;	
				else
					dwellTime|=UARTCharGetNonBlocking(UART1_BASE);		//If there was a terminating character read the next byte
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
			return;
		}
		if (identifier[0]=='R' && identifier[1]=='D')		//If we recieve the command that the picture is complete...
		{
			gCode[gCodeEnd]=identifier[0];		//Store the first character of the G code
			gCode[gCodeEnd+1]=identifier[1];	//Store the second character of the G code
			numberOfRows++;
			gCodeEnd=gCodeEnd+2;		//move the index of the gCode buffer to point to one past the last entry
			return;
		}
		wellShit=1;
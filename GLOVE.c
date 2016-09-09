
/*
 * GLOVE.c

 */

#include "pmod.h"
#include "microblaze_sleep.h"
#include "grove_hapt.h"
#include "grove_colorsens.h"

#include <stdio.h>
#include <stdint.h>
#include "platform.h"

#include "xil_io.h"
#include "xparameters.h"
#include "xgpio_l.h"

#define leve XGPIO_DATA_OFFSET
#define base XPAR_SWSLEDS_GPIO_BASEADDR

int main(void)
{
	colorPoint tempRead;
	int x;
	//Initialize the color sensor
	init_csens();

	//initialize the hapt AND Calibrate the hapt

	for (x=1; x<4; x++){
		init_hapt(x);
		autoCal_hapt(x);
	}

	u8 val, ampR, ampG, ampB;


	int VIB, SLEEP;



	while (1)
	{
		//with a switch we turn it on

	    val = Xil_In8 (base+leve);
	    if ((val==1) || (val==2) || (val==4) || (val==8))

	    {
	    	xil_printf ("Il dispositivo è acceso\n");


	    		tempRead = readRGB_csens();
	    		xil_printf("Red value is %d. \n",tempRead.Red);
	    		xil_printf("Green value is %d. \n",tempRead.Green);
	    		xil_printf("Blue value is %d. \n",tempRead.Blue);
	    		xil_printf("Clear value is %d.\n",tempRead.Clear);
	   			ampR = 12 * tempRead.Red /25;
	   			ampG = 12 * tempRead.Green /25;
	    		ampB = 12 * tempRead.Blue /25;

		if (tempRead.Clear > 100)
				{
					VIB= 500;
					SLEEP=500;
				}
			else
				{
					VIB= 2000;
					SLEEP= 2000;
				}

			{
				write_hapt(MODE_Reg, 0x05, 1, 1);
				write_hapt(MODE_Reg, 0x05, 1, 2);
				write_hapt(MODE_Reg, 0x05, 1, 3);


				write_hapt(RTP_INPUT_Reg, ampR, 1, 1);
				write_hapt(RTP_INPUT_Reg, ampG, 1, 2);
				write_hapt(RTP_INPUT_Reg, ampB, 1, 3);

	    		MB_Sleep (VIB);

	    		write_hapt(MODE_Reg,0x40,1, 1);
	    		write_hapt(MODE_Reg,0x40,1, 2);
	    		write_hapt(MODE_Reg,0x40,1, 3);

	    		MB_Sleep (SLEEP);

	    		write_hapt(GO_Reg, 0x00,1, 1);
	    		write_hapt(GO_Reg, 0x00,1, 2);
	    		write_hapt(GO_Reg, 0x00,1, 3);

			}
	    }
	}
	return 0;
}

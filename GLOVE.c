
/*
 * HAPTMOTOR.c

 *
 *  Created on: Jun 21, 2016
 *      Author: exeless
 */

#include "pmod.h"
#include "microblaze_sleep.h"
#include "grove_glove.h"
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
	u8 val, ampR, ampG, ampB;
	colorPoint tempRead;
	int VIB, SLEEP, n;
	/*initialize the hapt*/
	init_hapt();

	autoCal_hapt();
	//nb: con il primo switch accendo


	while (1){



	    val = Xil_In8 (base+leve);
	    if (val==1 | val==2 | val==4 | val==8) {
	    	xil_printf ("Il dispositivo è acceso\n");

	    	{
	    			tempRead = readRGB_csens();
	    			xil_printf("Red value is %d. ",tempRead.Red);
	    			xil_printf("Green value is %d. ",tempRead.Green);
	    			xil_printf("Blue value is %d. ",tempRead.Blue);
	    			xil_printf("Clear value is %d.\n",tempRead.Clear);
	    			ampR = 12; //* tempRead.Red /25;
	    			ampG = 12 * tempRead.Green /25;
	    			ampB = 12 * tempRead.Blue /25;
if (tempRead.Clear > 100)
	{VIB= 500;
SLEEP=1000;
	}
else VIB= 2000;
SLEEP=4000;
	    		}

	    	playAmp(ampR);
	    	playAmp(ampG);
	    	playAmp(ampB);
	    	MB_Sleep (SLEEP);
	    }
	    for (n=1; n<4; n++)
	    write_hapt(GO_Reg, 0x00,1,n);

	}


	return 0;
	}


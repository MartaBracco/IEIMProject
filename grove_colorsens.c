/*
 * grove_colorsens.c
 *
 *  Created on: 22/giu/2016
 *      Author: exeless
 */

#include "pmod.h"
#include "grove_colorsens.h"
#include "microblaze_sleep.h"

void init_csens()
{
	// Initialize the default switch at GR1
	configureSwitch(0,SCL,GPIO_1,GPIO_2,GPIO_3,SDA,GPIO_5,GPIO_6,GPIO_7);

	/*Power up - Not sure if working*/
	write_csens(REG_CTL,0x03,1);
}

int write_csens(u8 reg, u32 data, u8 bytes)
{
	   u8 data_buffer[3];
	   data_buffer[0] = reg;
	   if(bytes ==2){
	      data_buffer[1] = data & 0x0f; // Bits 11:8
	      data_buffer[2] = data & 0xff; // Bits 7:0
	   }else{
	      data_buffer[1] = data & 0xff; // Bits 7:0
	   }

	   return iic_write(0,COLOR_SENSOR_ADDR, data_buffer, bytes+1);
}

u32 read_csens(u8 reg){
   u8 data_buffer[2];
   u32 sample;

   data_buffer[0] = reg; // Set the address pointer register
   iic_write(0, COLOR_SENSOR_ADDR, data_buffer, 1);

   iic_read(0, COLOR_SENSOR_ADDR,data_buffer,2);
   sample = ((data_buffer[0]&0x0f) << 8) | data_buffer[1];
   return sample;
}

colorPoint readRGB_csens()
{
	u32 readData[8];
	colorPoint readColor;

	/*begin transmission*/
	//write_csens(COLOR_SENSOR_ADDR,REG_BLOCK_READ,1);
	write_csens(REG_CTL,0x03,1);

	/*wait for data*/
	MB_Sleep(100); //TO BE RESOLVED

	/*read Data*/
	int i ;
	u32 maxcol, mincol;
	double tmp;
	for(i=0;i<8;i++)
	{

		readData[i] = read_csens(REG_BLOCK_READ+i); //TO BE CHECKED
	}
	/*
	readColor.Green = readData[1] << 8 | readData[0];
	readColor.Red = readData[3] << 8 | readData[2];
	readColor.Blue = readData[5] << 8 | readData[4];
	readColor.Clear = readData[7] << 8 | readData[6];
	*/
if (1)
{
	if (readColor.Red > readColor.Green)
	maxcol=readColor.Red;
	else maxcol=readColor.Green;
		if (readColor.Blue > maxcol)
			maxcol=readColor.Blue;

		tmp= 250.0/maxcol;
		readColor.Green *= tmp;
			readColor.Red *= tmp;
			readColor.Blue *= tmp;
}

		if (readColor.Red < readColor.Green)
		mincol=readColor.Red;
		else mincol=readColor.Green;
			if (readColor.Blue < mincol)
				mincol=readColor.Blue;

			if (readColor.Red > readColor.Green)
			maxcol=readColor.Red;
			else maxcol=readColor.Green;
				if (readColor.Blue > maxcol)
					maxcol=readColor.Blue;
	 u32 greentmp=readColor.Green;
	 u32 redtmp=readColor.Red;
	 u32 bluetmp=readColor.Blue;

if (readColor.Red < 0.8 * maxcol && readColor.Red >= 0.6 * maxcol)
	readColor.Red *= 0.4;
	else if
	(readColor.Red < 0.6 *maxcol)
		readColor.Red *= 0.2;
if (readColor.Green < 0.8 * maxcol && readColor.Green >= 0.6 * maxcol)
	readColor.Green *= 0.4;
else if (readColor.Green < 0.6 *maxcol)
{ if (maxcol == redtmp && greentmp >= 2 * bluetmp && greentmp >= 0.2 * redtmp)
	readColor.Green *= 5;
readColor.Green *= 0.2;
}

if (readColor.Blue<0.8*maxcol && readColor.Blue >= 0.6*maxcol)
	readColor.Blue *= 0.4;
else if (readColor.Blue < 0.6*maxcol){

		if (maxcol == redtmp && greentmp >= 2 * bluetmp && greentmp >= 0.2 * redtmp)
			readColor.Blue *= 0.5;
		if (maxcol == redtmp && greentmp <= bluetmp && bluetmp >= 0.2 * redtmp)
			readColor.Blue *= 5;

		readColor.Blue *= 0.2;
}

if (readColor.Red < readColor.Green)
		mincol=readColor.Red;
		else mincol=readColor.Green;

			if (readColor.Blue < mincol)
				mincol=readColor.Blue;

if (maxcol == readColor.Green && readColor.Red >= 0.85 * maxcol && mincol == readColor.Blue){
	readColor.Red = maxcol;
	readColor.Blue *= 0.4;
}


	return readColor;

	/*White LED value correction TBD*/
}





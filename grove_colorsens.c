
/*
 * grove_colorsens.c
 */

#include "pmod.h"
#include "grove_colorsens.h"
#include "math.h"
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
	  data_buffer[1] = data & 0xff; //bits 7:0

	   return iic_write(0,COLOR_SENSOR_ADDR, data_buffer, 2);
}

u32 read_csens(u8 reg){
   u8 data_buffer[2];
   u32 sample;

   data_buffer[0] = reg; // Set the address pointer register
   iic_write (0,COLOR_SENSOR_ADDR, data_buffer, 1);

   iic_read(0, COLOR_SENSOR_ADDR,data_buffer,2);
   sample = data_buffer[0];
   return sample;
}

colorPoint readRGB_csens()
{
	u8 readData[9];
	colorPoint readColor;

	/*begin transmission*/
	//write_csens(COLOR_SENSOR_ADDR,REG_BLOCK_READ,1);
	write_csens(REG_CTL,0x03,1);

	/*wait for data*/
	MB_Sleep(15); //TO BE RESOLVED

	/*read Data*/
	int i;
	for(i=0;i<8;i++)
	{
		readData[i] = read_csens(REG_BLOCK_READ+i); //TO BE CHECKED
	}
	readColor.Green = readData[1] << 8 | readData[0];
	readColor.Red = readData[3] << 8 | readData[2];
	readColor.Blue = readData[5] << 8 | readData[4];
	readColor.Clear = readData[7] << 8 | readData[6];

	//Color correction LED is OFF!

	int maxColor,tmp;
	if(readColor.Red > readColor.Green)
		maxColor = readColor.Red;
	else
		maxColor = readColor.Green;
	if(maxColor < readColor.Blue)
		maxColor = readColor.Blue;

	tmp = 255.0/maxColor;
	readColor.Green *= tmp;
	readColor.Red *= tmp;
	readColor.Blue *= tmp;

	return readColor;



	u32 red_, green_, blue_;

	red_ = readColor.Red;
	green_ = readColor.Green;
	blue_ = readColor.Blue;

	double TMP;



		{
			red_  = red_  * 1.70;
			blue_ = blue_ * 1.35;

			if (red_> green_)
				maxColor = red_;
			else maxColor = green_;

				if (blue_ > maxColor)
					maxColor = blue_;

			if(maxColor > 255)
			{
				TMP = 250.0/maxColor;
				green_	*= TMP;
				red_ 	*= TMP;
				blue_	*= TMP;
			}
		}
		

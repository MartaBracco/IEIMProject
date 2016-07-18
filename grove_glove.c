/*

* grove_haptmotor.c

*

* Created on: 20/giu/2016

* Author: exeless

*/

#include "pmod.h"

#include "grove_glove.h"

int write_hapt(u8 reg, u32 data, u8 bytes, int n)

{
u8 data_buffer[3];

data_buffer[0] = reg;

if(bytes ==2){

data_buffer[1] = data & 0x0f; // Bits 11:8

data_buffer[2] = data & 0xff; // Bits 7:0

}else{

data_buffer[1] = data & 0xff; // Bits 7:0

return iic_write(n, DRV2605_ADDRESS, data_buffer, bytes+1);

};

}

u32 read_hapt(u8 reg){

u8 data_buffer[2];

u32 sample;

data_buffer[0] = reg; // Set the address pointer register

iic_write(2,DRV2605_ADDRESS, data_buffer, 1);
iic_read(2,DRV2605_ADDRESS,data_buffer,2);

iic_write(3,DRV2605_ADDRESS, data_buffer, 1);
iic_read(3,DRV2605_ADDRESS,data_buffer,2);

iic_write(4,DRV2605_ADDRESS, data_buffer, 1);
iic_read(4,DRV2605_ADDRESS,data_buffer,2);

sample = ((data_buffer[0]&0x0f) << 8) | data_buffer[1];

return sample;

}

void init_hapt()

{

// Initialize the default switch at GR1

configureSwitch(1,SCL,GPIO_1,GPIO_2,GPIO_3,SDA,GPIO_5,GPIO_6,GPIO_7);
configureSwitch(2,SCL,GPIO_1,GPIO_2,GPIO_3,SDA,GPIO_5,GPIO_6,GPIO_7);
configureSwitch(3,SCL,GPIO_1,GPIO_2,GPIO_3,SDA,GPIO_5,GPIO_6,GPIO_7);

/*Enter standby mode*/
int n;
for (n=1; n<4; n++) {
write_hapt(MODE_Reg,0x40,1,n);

/*set rated voltage*/

write_hapt(RATED_VOLTAGE_Reg,0x50,1,n);

/*set overdrive voltage*/

write_hapt(OD_CLAMP_Reg,0x89,1,n);
/*Setup feedback and control*/

write_hapt(FB_CON_Reg,0x89,1,n);

write_hapt(CONTRL1_Reg,0x13,1,n);

write_hapt(CONTRL2_Reg,0xF5,1,n);

write_hapt(CONTRL3_Reg,0x80,1,n);

/*Select LRA library*/

write_hapt(LIB_SEL_Reg, 0x06,1,n);

/*Set active mode*/

write_hapt(MODE_Reg, 0x00,1,n);
}
}

void autoCal_hapt(){
int n;
for (n=1; n<4; n++)
{

u8 temp = 0x00;

u8 ACalComp, ACalBEMF,BEMFGain;

/*set rated voltage*/

write_hapt(RATED_VOLTAGE_Reg, 0x50,1,n);

/*set overdrive voltage*/

write_hapt(OD_CLAMP_Reg, 0x89, 1,n);

/*Setup feedback and control*/

write_hapt(FB_CON_Reg,0xB6,1,n);

write_hapt(CONTRL1_Reg,0x93,1,n);

write_hapt(CONTRL2_Reg,0xF5,1,n);

write_hapt(CONTRL3_Reg,0x80,1,n);

/*Set autocalibration mode*/

write_hapt(MODE_Reg,0x07,1,n);

write_hapt(CONTRL4_Reg,0x20,1,n);

/*Begin auto calibration*/

write_hapt(GO_Reg, 0x01,1,n);

while((temp & 0x01) != 0x01)

temp = read_hapt(GO_Reg);

xil_printf("GO autocalibration reg %x¥n",temp);

/*Read status registers*/

temp = read_hapt(STATUS_Reg);

xil_printf("Status reg %x¥n",temp);

if((temp & 0x08) != 0x00)

{

ACalComp = read_hapt(A_CAL_COMP_Reg);

ACalBEMF = read_hapt(A_CAL_BEMF_Reg);

BEMFGain = read_hapt(FB_CON_Reg);
}
}

}

/*void playWave_hapt(u8 ucEffect)

{

write_hapt(MODE_Reg, 0x00, 1);

write_hapt(WAV_SEQ1_Reg, ucEffect, 1);

write_hapt(WAV_SEQ2_Reg, 0x00, 1);

write_hapt(GO_Reg, 0x01, 1);

}*/

void playAmp(u8 amp)

{
int VIB;
int n;
for (n=1; n<4; n++) {
write_hapt(MODE_Reg, 0x05, 1,n);

write_hapt(RTP_INPUT_Reg, amp, 1,n);
}

MB_Sleep (VIB);
for (n=1; n<4; n++)
	write_hapt(MODE_Reg,0x40,1,n);
}

#include <kipr/botball.h>
// Created on Fri May 3 2019

// Replace FILE with your file's name
#ifndef _main_H_
#define _main_H_


		

int create_vacs(int Mainb, int SideB, int Vac)
{
if (Mainb < -127 && Mainb > 127)
	{
		if (Mainb < -127)
			{
				printf("Create_VacsErr: Main Brush Exceeded Min values");
				Mainb = -127;
			}
		if (Mainb > 127)
			{
				printf("Create_VacsErr: Main Brush Exceeded Max values");
				Mainb = 127;
			}		
			
	
	}
if (SideB < -127 && SideB > 127)
	{
		if (SideB < -127)
			{
				printf("Create_VacsErr: Side Brush Exceeded Min values");
				SideB = -127;
			}
		if (SideB > 127)
			{
				printf("Create_VacsErr: Side Brush Exceeded Max values");
				SideB = 127;
			}	
	}
if (Vac < 0 && Vac > 127)
	{		
		if (Vac < 0)
			{
				printf("Create_VacsErr: Vaccum Exceeded Min values");
				Vac = -127;
			}
		if (Vac > 127)
			{
				printf("Create_VacsErr: Vaccum Exceeded Max values");
				Vac = 127;
			}
		
	}
create_write_byte(144);
create_write_byte(Mainb);
create_write_byte(SideB);
create_write_byte(Vac);

}
int create_safe_alt()
{
	create_write_byte(131);
}
int create_full_alt()
{
	create_write_byte(132);
}

int create_led(int mode,int color, int brightness)
{
	if (mode < 0 && mode > 16)
	{
		if (mode < 0)
			{
				printf("Create_LEDErr: Mode Setting Exceeded Min values");
				mode = 0;
			}
		if (mode > 16)
			{
				printf("Create_LEDErr: Mode Setting Exceeded Max values");
				mode = 16;
			}		
	}
	if (color < 0 && color > 255)
	{
		if (color < 0)
			{
				printf("Create_LEDErr: Color Setting Exceeded Min values");
				color = 0;
			}
		if (color > 255)
			{
				printf("Create_LEDErr: Color Setting Exceeded Max values");
				color = 255;
			}		
	}
	if (brightness < 0 && brightness > 255)
	{
		if (brightness < 0)
			{
				printf("Create_LEDErr: Brightness Setting Exceeded Min values");
				brightness = 0;
			}
		if (brightness > 255)
			{
				printf("Create_LEDErr: Brightness Setting Exceeded Max values");
				brightness = 255;
			}		
	}	
create_write_byte(139);
create_write_byte(mode);
create_write_byte(color);
create_write_byte(brightness);


}

/*
/// I'm having trouble writing this code, but I'll come back to it later...
int create_write_char(char first,char second, char third, char fourth)
{
	//printf("%d",(int)first);
	create_write_byte(164);
	create_write_byte(first);
	create_write_byte(second);
	create_write_byte(third);
	create_write_byte(fourth);

}
int create_write_chars(char first)
{
	int a = first;
	printf("%d",a);
}
*/
#endif

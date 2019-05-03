// Created on Fri May 3 2019

// Replace FILE with your file's name
#ifndef _main_H_
#define _main_H_
int Create_Vacs(int Mainb, int SideB, int Vac, int sec)
{

	if (Mainb >= -127 && Mainb <= 127)
		{
			if (SideB >= -127 && SideB <= 127)
			{
				if (Vac >= 0 && Vac <= 127)
					{
						create_write_byte(144);
						create_write_byte(Mainb);
						create_write_byte(SideB);
						create_write_byte(Vac);
						msleep((sec*1000));
					}
			}

		}
else
	{
		printf("Create_VacsErr: Exceeded Max/Min values: Ignoring ");
	}

}
int Create_Safe()
{
	create_write_byte(131);
}
int Create_Full()
{
	create_write_byte(132);
}
int Create_Led(int mode,int color, int brightness)
{
	if (Mainb >= 0 && Mainb <= 16)
	{
		if (color >= 0 && color <= 255)
		{
			if (brightness >= 0 && brightness <= 255)
			{
				create_write_byte(139);
				create_write_byte(mode);
				create_write_byte(color);
				create_write_byte(brightness);
			}
		}
	}
else
{
	printf("Create_LedErr: Exceeded Max/Min values: Ignoring ");
}



}

#endif

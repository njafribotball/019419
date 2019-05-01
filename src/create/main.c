#include <kipr/botball.h>

int main()
{

    create_connect();
    create_drive_direct(250,150);
    msleep(500);
    create_drive_direct(250,-250);
    msleep(3000);
    //Blindly Coding, but this is to drive the Create on the Main Line
    create_drive_straight(300);
    msleep(10000);
    create_drive_direct(250,-250);
    msleep(3000);

}

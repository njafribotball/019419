#include <kipr/botball.h>
void move_forward();
int main()
{
    while(1)
    {
        camera_update();
        if(get_object_count(1)>0) //find yellow
        {
            camera_update();
            move_forward();
            printf("done");
            //Go straight
        }
        else
        {
            camera_update();
            motor(1,-40);
            motor(2,40);
            //Turn Left
        }
    }
    /*while(1);
    {
        while(get_object_count(0)>=1);//detect bot
        {
            camera_update();
            move_forward();
        }
        camera_update();
        motor(1,50);
        motor(2,-50);
    }*/
    return 0;
}
void move_forward()
{
    motor (1, 80);
    motor (2, 80);
}

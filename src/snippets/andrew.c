#include <kipr/botball.h>
void turn_left_90_front_facing();
void turn_right_90();
void follow_black_line(int distance);
int simple_signal();

#define CAMERA_CONFIG "Botball2019"
#define CAMERA_YELLOW_CHANNEL 0


int main()
{
    //wait_for_light(0);
    enable_servos(3);
    set_servo_position(3,1195);
    msleep(500);
    /*
    shut_down_in(119);
    msleep(2000);
    cmpc(3);
    turn_left_90_front_facing();//turns left 90 degrees in of starting box
    cmpc(3);
    while (gmpc(3) >= -4550)// runs straight towards yellow and blue foam block (can be adjusted if needed)
    {
        motor(1,-100);
        motor(3,-100);
    }
    motor(1,0);
    motor(3,0);
    msleep(200);//pauses
    cmpc(3);
    turn_left_90_front_facing();// turns 90 degrees left again so now its facing the pvc board
    cmpc(1);
    while (gmpc(1) <= 3950)//DRIVE forward
    {
        motor(1,99);
        motor(3,100);
    }
    cmpc(3);
    motor(1,0);
    motor(3,0);
    msleep(200);
    while (gmpc(3) <= 1540)//turns so it is on the black line
    {
        motor(3,100);

    }
    motor(1,0);
    motor(3,0);
    msleep(200);
    cmpc(1);

    while (gmpc(1) >= -2000)
    {
        motor(1,-100);
        motor(3,-100);
    }
    set_servo_position(3,115);
    cmpc(3);
    follow_black_line(13000);
    set_servo_position(3,1394);
    cmpc(3);
    turn_left_90_front_facing();
    msleep(100);
    follow_black_line(11000);
    motor(0,0);
    motor(3,0);
    msleep(100);
    */

    int fire_on_right_building = simple_signal();

    if (fire_on_right_building) {
        printf("a\n");
        enable_servo(3);
        set_servo_position(3,130);
        msleep(5000);
        follow_black_line(1000);
    }

    return 0;
}


void follow_black_line(int distance)
{
    while (gmpc(3) <= distance)

    if (analog(2)<3200)
    {
        motor(3,100);
        motor(1,30);
    }
    else
    {
        motor(3,60);
        motor(1,100);
    }
}


void turn_left_90_front_facing()
{    while (gmpc(3) >= -1700)
    {
        motor(3,-100);
    }
}


int simple_signal()
{
    /*
    return 1 for detecting the Yellow-Red signal square else return 0
    */

    int detected = 0;
    if (camera_open () == 0)
    {
        printf ("\tFailed to open camera\n");
        return 0;
    }

    if (camera_load_config (CAMERA_CONFIG) == 0)
    {
        printf ("Failed to load camera configuration\n");
        camera_close();
        msleep(500);

        return 0;
    }

    msleep(1000);

    // update camera 30 times to get more reliable sensor data
    // while loop because for loop decleration not supported in current c compiler mode :(

    printf("\tcamera update\n");
    int i = 0;
    while (i < 10)
    {
    	if (camera_update() == 0) {
            printf("\tFailed to update camera on iteration %d\n", i);
            camera_close();
            msleep(500);

            return 0;
        }
        msleep(50);
        i++;
    }

    // test if yellow and red object exist
    if (get_object_count (CAMERA_YELLOW_CHANNEL) != 0)
        detected = 1;

    camera_close();
    msleep(500);

    printf("\tDetected: %d\n", detected);

    return detected;
}

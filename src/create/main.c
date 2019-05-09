#include <kipr/botball.h>

#define LIGHT_SENSOR_PORT 5
#define ARM_PORT 0
#define SMALL_ARM_PORT 1
#define ARM_DOWN_POSITION 1950
#define ARM_UP_HIGH_POSITION 170
#define ARM_UP_LOW_POSITION 10
#define ARM_CLOSE_POSITION 2046
#define ARM_OPEN_POSITION 1150

#define DONE_STRING "                                            -->  done\n\n"


void init();
void servo(int port, int position, int sleep_time);
void arm_close();
void arm_open();
void arm_up_high();
void arm_up_low();
void arm_down();


int main()
{
    printf("Starting\n");
    create_connect();
    init();

    //wait_for_light();
    //shutdown_in(119);

    arm_up_high();

    create_spin_block(110, 200);

    arm_up_low();
    arm_open();

    create_drive_straight(1000);
    msleep(1600);
    create_stop();
    msleep(100);

    arm_close();
    msleep(1000);
    arm_down();
    msleep(1000);
    arm_open();

    create_disconnect();
    return 0;
}


void init()
{
    printf("Initiating: \n");
    // enable all servo ports
    enable_servos();

    arm_close();
    arm_down();
}


void arm_down()
{
    printf("arm down: \n");

    servo(ARM_PORT, ARM_DOWN_POSITION, 10);

    printf(DONE_STRING);
}


void arm_up_high()
{
    printf("arm up high: \n");

    servo(ARM_PORT, ARM_UP_HIGH_POSITION, 10);

    printf(DONE_STRING);
}


void arm_up_low()
{
    printf("arm up low: \n");

    servo(ARM_PORT, ARM_UP_LOW_POSITION, 10);

    printf(DONE_STRING);
}


void arm_close()
{
    printf("small arm close: \n");

    servo(SMALL_ARM_PORT, ARM_CLOSE_POSITION, 0);

    printf(DONE_STRING);
}


void arm_open()
{
    printf("small arm open: \n");

    servo(SMALL_ARM_PORT, ARM_OPEN_POSITION, 0);

    printf(DONE_STRING);
}


void servo(int port, int position, int sleep_time)
{
    int cur_posi = get_servo_position(port);

    if (cur_posi > position)
    {
        while (cur_posi > position)
        {
            cur_posi -= 10;
            set_servo_position(port, cur_posi);
            msleep(sleep_time);
        }
    } else {
        while (cur_posi < position) {
            cur_posi += 10;
            set_servo_position(port, cur_posi);
            msleep(sleep_time);
        }
    }
}

#include <kipr/botball.h>

#define LIGHT_SENSOR_PORT 5
#define CLAW_BUMP_SENSOR_PORT 0
#define ARM_PORT 0
#define SMALL_ARM_PORT 1
#define ARM_DOWN_POSITION 1950
#define ARM_UP_HIGH_POSITION 330
#define ARM_UP_LOW_POSITION 5
#define ARM_STRAIGHT_UP_POSITION 600
#define ARM_CLOSE_POSITION 2046
#define ARM_OPEN_POSITION 1000

#define DONE_STRING "                                            -->  done\n\n"


void init();
void servo(int port, int position, int sleep_time);
void servo_move_until(int port, int position, int sleep_time, int (*cond)());
void arm_close();
void arm_open();
void arm_up_high();
void arm_up_low();
void arm_down();
void arm_straight_up();
void drive_create(int speed, int time);
void arm_close_until_grabbed();
int grabbed_something();
int empty_func();


int main()
{
    /*
    grab item from 3 skyscrappers (3 -> 1)
    find out which one is on fire by testing if it grabs anything
    grab the big blue cube (water block)
    put on the skyscapper that is on fire
    */
    printf("Starting\n");
    init();

    wait_for_light(LIGHT_SENSOR_PORT);
    shut_down_in(119);
    arm_up_high();
    msleep(60*1000);

    // skyscrapper on fire flag:
    //    0  | Left building
    //    1  | Middle building
    //    2  | Right building
    //    -1 | undetermined
    int skyscrapper_on_fire = -1;

    // go to 3rd (index 2) skyscrapper

    create_spin_block(110, 200);

    arm_up_low();
    arm_open();

    drive_create(1000, 950);

    // grab 3rd (index 2) skyscrapper item
    arm_close_until_grabbed();
    msleep(500);

    arm_straight_up();
    create_spin_CW(100);
    msleep(300);
    create_stop();
    drive_create(-200, 1700);  //changed
    arm_down();
    arm_open();
    drive_create(200, 1700);//changed
    create_spin_CCW(100);
    msleep(300);
    create_stop();

    // move to 2rd (index 1) skyscrapper
    arm_straight_up();
    create_spin_block(60, 200);
    drive_create(500, 600);
    create_spin_block(-60, 200);
    create_stop();

    // grab 2rd (index 1) skyscrapper item
    arm_open();
    arm_up_high();
    drive_create(100, 550);
    arm_close_until_grabbed();
    msleep(500);
    drive_create(-100, 550);

    // drop into starting zone
    arm_straight_up();
    drive_create(-150, 1700); //changed
    arm_down();
    arm_open();
    drive_create(150, 1700); //changed

    // move to 1rd (index 0) skyscrapper
    arm_straight_up();
    create_spin_block(70, 200);
    drive_create(500, 600);
    create_spin_block(-70, 200);

    // grab 1rd (index 0) skyscrapper item
    arm_open();
    arm_up_low();
    arm_close_until_grabbed();
    msleep(500);

    // drop into starting zone
    arm_straight_up();
    create_spin_block(20, 100);
    drive_create(-150, 1700); //changed
    arm_down();
    arm_open();
    drive_create(150, 1700); //changed
    create_spin_block(-20, 100);

    printf("final result: skyscrapper %d (index %d) is on fire\n\n", skyscrapper_on_fire + 1, skyscrapper_on_fire);

    // go to grab the blue cube (water)
    // todo

    create_safe();
    create_disconnect();

    return 0;
}


void init()
{
    printf("Initiating: \n");
    create_connect();
    create_full();
    // enable all servo ports
    enable_servos();

    arm_open();
    arm_down();
}


void drive_create(int speed, int time)
{
    create_drive_straight(speed);
    msleep(time);
    create_stop();
    msleep(100);
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


void arm_straight_up()
{
    printf("arm straight up: \n");
    servo(ARM_PORT, ARM_STRAIGHT_UP_POSITION, 10);
    printf(DONE_STRING);
}


void arm_close_until_grabbed()
{
    printf("small arm close until grabbed something: \n");
    servo_move_until(SMALL_ARM_PORT, ARM_CLOSE_POSITION, 10, grabbed_something);
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
{ servo_move_until(port, position, sleep_time, empty_func); }

void servo_move_until(int port, int position, int sleep_time, int (*cond)())
{
    int cur_posi = get_servo_position(port);

    if (cur_posi > position)
    {
        while (cur_posi > position && !cond())
        {
            cur_posi -= 10;
            if (cur_posi <= 0) cur_posi = 1;
            set_servo_position(port, cur_posi);
            msleep(sleep_time);
        }
    } else {
        while (cur_posi < position && !cond()) {
            cur_posi += 10;
            if (cur_posi >= 2047) cur_posi = 2046;
            set_servo_position(port, cur_posi);
            msleep(sleep_time);
        }
    }

    msleep(200);
}


int grabbed_something() { return digital(CLAW_BUMP_SENSOR_PORT); }

int empty_func() { return 0; }

#include <kipr/botball.h>
#include <math.h>
#include <time.h>


#define ARM_PORT 3
#define LEFT_MOTOR_PORT 3
#define RIGHT_MOTOR_PORT 2
#define ROS_PORT 4
#define LIGHT_SENSOR_PORT 5

#define ARM_DUMP_POSITION 2047
#define ARM_DOWN_POSITION 200

#define LEFT_MOTOR_SPEED_ADJ +2
#define RIGHT_MOTOR_SPEED_ADJ -2
#define TURNING_ANGLE_MULTIPLIER 9.798

#define BLACK_SURFACE_THRESHOLD 3550
#define GREY_SURFACE_THRESHOLD 3000

#define DONE_STRING "                                            -->  done\n\n"

void init();
void dump();
void arm_down();
void drive(int left_motor_speed, int right_motor_speed, int time);
void drive_ticks(int left_motor_ticks, int right_motor_ticks,
                 int left_motor_speed, int right_motor_speed);
void turn(int angle);
void follow_line();
void duration(int seconds, void (*prt) ());

int max(int a, int b);
int min(int a, int b);


int main()
{
    printf("Start\n");

    init();

    printf("running\n");

    // set 10 seconds for running funcion follow_line using callback
    void (*flw_ln) () = &follow_line;
    duration(10, flw_ln);
    return 0;
}


void init()
{
    printf("Initiating\n");
    // enable all servo ports
    enable_servos();

    dump();
}


void duration(int seconds, void (*prt) ())
{
    time_t start_t, cur_t;
    time(&start_t);
    time(&cur_t);

    while (difftime(cur_t, start_t) < seconds) {
        // callback
        (*prt) ();

        time(&cur_t);
    }
}


void follow_line()
{
    if (analog(ROS_PORT) > BLACK_SURFACE_THRESHOLD)
        drive(50, 25, 10);
    else if (GREY_SURFACE_THRESHOLD < analog(ROS_PORT) &&
        analog(ROS_PORT) < BLACK_SURFACE_THRESHOLD)
        drive(25, 50, 10);
}


void turn(int angle)
{
    drive_ticks((int) angle * TURNING_ANGLE_MULTIPLIER,
                (int) -angle * TURNING_ANGLE_MULTIPLIER,
                1000, 1000);
}


void drive(int left_motor_speed, int right_motor_speed, int time)
{
    printf("Driving: \n\tleft motor speed: %d right motor speed: %d\n\ttime: %d\n",
        left_motor_speed, right_motor_speed, time);

    int left_speed = (left_motor_speed / abs(left_motor_speed)) * (abs(left_motor_speed) + LEFT_MOTOR_SPEED_ADJ);
    int right_speed = (right_motor_speed / abs(right_motor_speed)) * (abs(right_motor_speed) + RIGHT_MOTOR_SPEED_ADJ);

    int max_n = (left_speed >= right_speed) ? left_speed: right_speed;
    int min_n = (left_speed <= right_speed) ? left_speed: right_speed;

    int adj = 0;
    if (max_n > 100)
    	adj = 100 - max_n;
   	else if (min_n < -100)
    	adj = -100 - min_n;

    motor(LEFT_MOTOR_PORT, left_speed + adj);
    motor(RIGHT_MOTOR_PORT, right_speed + adj);
    printf("\texecuted left motor speed: %d \n\texecuted right motor speed: %d \n\tOverall speed adjustment: %d",
           left_speed + adj, right_speed + adj, adj);
    msleep(time);

    freeze(LEFT_MOTOR_PORT);
    freeze(RIGHT_MOTOR_PORT);

    printf(DONE_STRING);
}


void drive_ticks(int left_motor_ticks, int right_motor_ticks, int left_motor_speed, int right_motor_speed)
{
    printf("Driving ticks: \n\tleft motor ticks: %d speed: %d\n\tright motor ticks: %d speed: %d\n",
    left_motor_ticks, left_motor_speed,
    right_motor_ticks, right_motor_speed);

    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);

    mtp(LEFT_MOTOR_PORT, left_motor_speed, left_motor_ticks);
    mtp(RIGHT_MOTOR_PORT, right_motor_speed, right_motor_ticks);
    bmd(LEFT_MOTOR_PORT);
    bmd(RIGHT_MOTOR_PORT);

    // adjustments for accomendating errors (motor over-spinned / under-spinned)
    mtp(LEFT_MOTOR_PORT, 100, left_motor_ticks);
    bmd(LEFT_MOTOR_PORT);
    mtp(RIGHT_MOTOR_PORT, 100, right_motor_ticks);
    bmd(RIGHT_MOTOR_PORT);

    freeze(LEFT_MOTOR_PORT);
    freeze(RIGHT_MOTOR_PORT);

    printf("\tError: left: %d right: %d\n",
           gmpc(LEFT_MOTOR_PORT) - left_motor_ticks,
           gmpc(RIGHT_MOTOR_PORT) - right_motor_ticks);

    printf(DONE_STRING);
}


void dump()
{
    printf("arm_dump: \n");

    int tgt_pos = get_servo_position(ARM_PORT);

    while (tgt_pos < ARM_DUMP_POSITION){
        tgt_pos += 10;
        set_servo_position(ARM_PORT, tgt_pos);
        msleep(20);
    }

    printf(DONE_STRING);
}


void arm_down()
{
    printf("arm_down: \n");

    int tgt_pos = get_servo_position(ARM_PORT);

    while (tgt_pos > ARM_DOWN_POSITION){
        tgt_pos -= 10;
        set_servo_position(ARM_PORT, tgt_pos);
    	msleep(20);
    }

    printf("done\n");
}

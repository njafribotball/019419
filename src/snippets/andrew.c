#include <kipr/botball.h>
#include <math.h>
#include <time.h>


#define ARM_PORT 3
#define SMALL_ARM_PORT 3
#define LEFT_MOTOR_PORT 3
#define RIGHT_MOTOR_PORT 1
#define ROS_PORT 4
#define LIGHT_SENSOR_PORT 5

#define ARM_START_POSITION 2045
#define ARM_DUMP_POSITION 1744
#define ARM_DOWN_POSITION 220
#define ARM_HOLDUP_POSITION 1050
#define SMALL_ARM_CLOSE_POSITION 1195
#define SMALL_ARM_OPEN_POSITION 115

#define LEFT_MOTOR_SPEED_ADJ +2
#define RIGHT_MOTOR_SPEED_ADJ -2
#define TURNING_ANGLE_MULTIPLIER 10

#define BLACK_SURFACE_THRESHOLD 3800
#define GREY_SURFACE_THRESHOLD 3000

#define CAMERA_CONFIG "Botball2019"
#define CAMERA_YELLOW_CHANNEL 0
#define CAMERA_RED_CHANNEL 1

#define DONE_STRING "                                            -->  done\n\n"

void init();
void servo(int port, int position, int sleep_time);
void arm_close();
void arm_open();
void drive(int left_motor_speed, int right_motor_speed, int time);
void drive_ticks(int left_motor_ticks, int right_motor_ticks,
                 int left_motor_speed, int right_motor_speed);
void turn(int angle);
void follow_line();
void backward_follow_line();
int signal();
void duration(int seconds, void (*prt) ());
void forward_until_line(int threshold);
void turn_until_line(int direction, int low_threshold, int high_threshold);


int main()
{
    printf("Start\n=====================================================\n");

    init();

    printf("running\n=====================================================\n");

	drive_ticks(2000, 2000, 1400, 1400);
    turn(-35);
    drive_ticks(4000, 4000, 1400, 1400);
    turn(140);

    return 0;
}


void init()
{
    printf("Initiating: \n");
    // enable all servo ports
    enable_servos();

    arm_close();

    // wait_for_light(LIGHT_SENSOR_PORT);
    // shutdown_in(119);
}


void forward_until_line(int threshold)
{
    // drive forward until line detected belowed the threshold
    while (analog(ROS_PORT) < threshold)
    {
        drive(50, 50, 10);
    }
}


void turn_until_line(int direction, int low_threshold, int high_threshold)
{
    while (low_threshold < analog(ROS_PORT) && analog(ROS_PORT) < high_threshold)
    {
        if (direction == 1)
            drive(50, -50, 10);
        else
            drive(-50, 50, 10);
    }
}


void duration(int seconds, void (*prt) ())
{
    // keep calling a function until speed limit exceeded
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
        drive(50, 45, 10);
    else if (GREY_SURFACE_THRESHOLD < analog(ROS_PORT) &&
        analog(ROS_PORT) < BLACK_SURFACE_THRESHOLD)
        drive(25, 45, 10);
}


void turn(int angle)
{
    // negetive for turning to the left
    drive_ticks((int) angle * TURNING_ANGLE_MULTIPLIER,
                (int) -angle * TURNING_ANGLE_MULTIPLIER,
                80 * angle / abs(angle), 80 * -angle / abs(angle));
}


void drive(int left_motor_speed, int right_motor_speed, int time)
{
    printf("Driving: \n\tleft motor speed: %d right motor speed: %d\n\ttime: %d\n",
        left_motor_speed, right_motor_speed, time);

    // adjustment for each motor speed since motor doesn't run at the same speed
    int left_speed = (left_motor_speed / abs(left_motor_speed)) * (abs(left_motor_speed) + LEFT_MOTOR_SPEED_ADJ);
    int right_speed = (right_motor_speed / abs(right_motor_speed)) * (abs(right_motor_speed) + RIGHT_MOTOR_SPEED_ADJ);

    // calculate max and min to acoid exceeding [-100, 100] range
    int max_n = (left_speed >= right_speed) ? left_speed: right_speed;
    int min_n = (left_speed <= right_speed) ? left_speed: right_speed;

    // adjustment for keeping speed in range
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

    // clear motor position counter
    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);

    // move left & right motor while target position not reached
    while (1) {
        int left_finished = 0;

        // absolute value for both moving forward and backward
        if (abs(gmpc(LEFT_MOTOR_PORT)) < abs(left_motor_ticks))
        {
            motor(LEFT_MOTOR_PORT, left_motor_speed);
        }
        else
        {
            freeze(LEFT_MOTOR_PORT);
            left_finished = 1;
        }

        if (abs(gmpc(RIGHT_MOTOR_PORT)) < abs(right_motor_ticks))
        {
            motor(RIGHT_MOTOR_PORT, right_motor_speed);
        }
        else
        {
            freeze(RIGHT_MOTOR_PORT);

            // break when left motor finished job too
            if (left_finished)
                break;
        }
    }

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


void arm_close()
{
    printf("small arm close: \n");

    servo(SMALL_ARM_PORT, SMALL_ARM_CLOSE_POSITION, 0);

    printf(DONE_STRING);
}


void arm_open()
{
    printf("small arm open: \n");

    servo(SMALL_ARM_PORT, SMALL_ARM_OPEN_POSITION, 0);

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


int signal()
{
    /*
    return 1 for detecting the Yellow-Red signal square else return 0
    */

    int detected = 0;
    if (camera_open () == 0)
    {
        printf ("\tFailed to open camera\n");
        printf(DONE_STRING);
        return 0;
    }

    if (camera_load_config (CAMERA_CONFIG) == 0)
    {
        printf ("Failed to load camera configuration\n");
        camera_close();
        msleep(500);
        printf(DONE_STRING);

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

            printf(DONE_STRING);

            return 0;
        }
        msleep(50);
        i++;
    }

    // test if yellow and red object exist
    if (get_object_count (CAMERA_YELLOW_CHANNEL) != 0 && get_object_count (CAMERA_RED_CHANNEL) != 0)
    {
        // test if red square is in yellow square
        struct rectangle yellow_rect = get_object_bbox (CAMERA_YELLOW_CHANNEL, 0);
        struct rectangle red_rect = get_object_bbox (CAMERA_RED_CHANNEL, 0);

        printf("\tYellow Square:\n\t\tx: %d, y: %d, w: %d, h: %d\n", yellow_rect.ulx, yellow_rect.uly, yellow_rect.width, yellow_rect.height);
        printf("\tRed Square:\n\t\tx: %d, y: %d, w: %d, h: %d\n", red_rect.ulx, red_rect.uly, red_rect.width, red_rect.height);

        if ((yellow_rect.ulx <= red_rect.ulx) &&
            (yellow_rect.uly <= red_rect.uly) &&
            (yellow_rect.ulx + yellow_rect.width >= red_rect.ulx + red_rect.width) &&
            (yellow_rect.uly + yellow_rect.height >= red_rect.uly + red_rect.height))
        {
            detected = 1;
        }
    }

    camera_close();
    msleep(500);

    printf("\tDetected: %d\n", detected);
    printf(DONE_STRING);

    return detected;
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
        printf(DONE_STRING);
        return 0;
    }

    if (camera_load_config (CAMERA_CONFIG) == 0)
    {
        printf ("Failed to load camera configuration\n");
        camera_close();
        msleep(500);
        printf(DONE_STRING);

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

            printf(DONE_STRING);

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
    printf(DONE_STRING);

    return detected;
}

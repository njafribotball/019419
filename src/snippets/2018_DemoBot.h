#include <kipr/botball.h>

int FRONT_ARM_PORT = 1;
int BACK_ARM_PORT = 0;
int ET_SENSOR_PORT = 0;

void forward(int speed, int time);
void turn(int left_wheel_speed, int right_wheel_speed, int time);
void simple_turn(int direction, int speed, int time);
void camera_go_to(int channel);
void arm_up();
void arm_down();
void arm_open();
void arm_close(int position);
void init();
int* find_zone();


int main(){
    // wait for light to start
    //wait_for_light(1);
    //shut_down_in(120);
    printf("Start\n");
    init();
    simple_turn(-1, 100, 350);
    forward(100, 1000);
    simple_turn(1, 100, 500);

    // get color zone position
    int *position = find_zone();
    printf("color zone position: %d%d%d\n(left to right  0:red, 1:yellow, 2:green\n)"
           , position[0], position[1], position[2]);

    // grab yellow block
    simple_turn(-1, 100, 300);
    forward(100, 4700);
    simple_turn(-1, 100, 470);
    forward(100, 750);
    arm_close(1350);
    arm_up();
    forward(-100, 1500);
    // putting it aside
    simple_turn(-1, 50, 1400);
    forward(70, 800);
    arm_down();
    arm_open();
    forward(-70, 800);
    simple_turn(1, 50, 1400);

    // grab botguy
    forward(100, 2500);
    arm_close();
    arm_up();
    return 0;
}


void forward(int speed, int time){
    // Speed positive to go forward, negetive to go backward
    // time in miliseconds
    turn(speed, speed, time);
}


void turn(int left_wheel_speed, int right_wheel_speed, int time){
    // Speed positive to go forward, negetive to go backward
    // time in miliseconds
    motor(0, left_wheel_speed);
    motor(1, right_wheel_speed);
    msleep(time);
    freeze(0);
    freeze(1);
}


void simple_turn(int direction, int speed, int time){
    // for turning around in one place
    // direction: 0 to turn left 1 to turn right
    // speed: the speed of the wheel
    // time: miliseconds
    int l = -1;
    int r = 1;
    if (direction == 1){
        l = 1;
        r = -1;
    }
    turn(l*speed, r*speed, time);
}


int * find_zone()
{
	// red is 0, yellow is 1, green is 2
    camera_open();
    int i = 0;
    for (i; i < 5; i++){
        camera_update();
    }
    struct point2 yellow, red, green;
    yellow = get_object_center(0, 0);
    red = get_object_center(1, 0);
    green = get_object_center(2, 0);
    printf("yellow: %d  red: %d  green: %d\n", yellow.x, red.x, green.x);
    int left, mid, right;
    if (yellow.x > red.x)
    {
        if (green.x > yellow.x)
        {
            left = 0;
            mid = 1;
            right = 2;
        }
        else if (green.x < yellow.x && green.x > red.x)
        {
         	left = 0;
            mid = 2;
            right = 1;
        }
        else
        {
         	left = 2;
            mid = 0;
            right = 1;
        }
    }
    else if (yellow.x < red.x)
    {
        if (green.x > yellow.x && green.x < red.x)
        {
         	left = 1;
            mid = 2;
            right = 0;
        }
        else if (green.x > yellow.x)
        {
            left = 1;
            mid = 0;
            right = 2;
        }
        else
        {
         	left = 2;
            mid = 1;
            right = 0;
        }
    }
    int positions[3] = {left, mid, right};
    int *p = NULL;
    p = &positions;
    return p;
}


void camera_go_to(int channel)
{
    // red is 0, yellow is 1, green is 2
    camera_open();
    int first_frame = 1; // test if it is the first frame and jump over it since it is so unreliable
    while (1==1){
        camera_update();
        if (first_frame==1){
            first_frame = 0;
            continue;
        }
        int obj_count = get_object_count(channel);
        if (obj_count == 0 || get_object_area(channel, 0) < 1){
            printf("no obj found\t");
            continue;
        }
        printf("%d\n", get_object_area(channel, 0));

        struct point2 center;
        center = get_object_center(channel, 0);
        if (center.x < 60){
            // turn left
            turn(1, 70 - center.x, 1);
            msleep(50);
        }
        else if (center.x > 100){
            // turn right
            turn(0, center.x - 90, 1);
            msleep(50);
        }
        else {
            forward(100, 1);
            msleep(50);
        }

        if (analog(0) >= 1800)
        {
        	break;
    	}
    }
    camera_close();
}


void init(){
 	enable_servos();
    arm_down();
    arm_open();
}

void arm_up(){
 	set_servo_position(BACKARMPORT, 1350);
    msleep(500);
}

void arm_down(){
 	set_servo_position(BACKARMPORT, 1500);
    msleep(500);
}

void arm_open(){
 	set_servo_position(FRONTARMPORT, 1900);
    msleep(500);
}

void arm_close(int position){
 	set_servo_position(FRONTARMPORT, position);
    msleep(500);
}

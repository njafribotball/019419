#include <kipr/botball.h>


void forward(int speed, int time);
void turn(int left_wheel_speed, int right_wheel_speed, int time);


int main(){
    // wait for light to start
    wait_for_light(1)
    shut_down_in(120);
    printf("Start\n")

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
    ao();
}

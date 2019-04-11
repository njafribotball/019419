#include <kipr/botball.h>

void forward(int speed, int time);
void turn(int direction, int speed, int time);

int main()
{
    while (1){
        if (analog(0) > 2000 && analog(1) > 2000 && analog(2) > 2000) {
            forward(-100, 1000);
        }
        else if (analog(0) > 2000 && analog(1) > 2000){
            turn(1, 100, 1000);
        }
        else if (analog(1) > 2000 && analog(2) > 2000){
            turn(0, 100, 1000);
        }
        else if (analog(0) > 2000){
            turn(1, 100, 100);
        }
        else if (analog(1) > 2000){
            turn(1, 100, 1000);
        }
        else if (analog(2) > 2000){
            turn(0, 100, 100);
        }
        else {
            forward(100, 1);
        }
    }
    return 0;
}


void forward(int speed, int time){
    motor(0, speed);
    motor(1, speed);
    msleep(time);
    ao();
}

void turn(int direction, int speed, int time){
    int l = -1;
    int r = 1;
    if (direction == 1){
        l = 1;
        r = -1;
    }
    motor(0, l * speed);
    motor(1, r * speed);
    msleep(time);
    ao();
}

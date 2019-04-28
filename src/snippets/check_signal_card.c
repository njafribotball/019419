#include <kipr/botball.h>

#define CAMERA_CONFIG "botball2019"
#define CAMERA_YELLOW_CHANNEL 0
#define CAMERA_RED_CHANNEL 1

int signal();

int main()
{
    printf("Hello World\n");
    printf("Detected: %d\n", signal());
    return 0;
}

int signal()
{
    /*
    return 1 for detecting the Yellow-Red signal square else return 0
    */

    int detected = 0;
    if (camera_open () == 0)
    {
        printf ("Failed to open camera\n");
        return 0;
    }

    /*
    if (camera_load_config (CAMERA_CONFIG) == 0)
    {
        printf ("Failed to load camera configuration\n");
        camera_close();
        return 0;
    }
    */

    msleep(1000);

    // update camera 30 times to get more reliable sensor data
    // while loop because for loop decleration not supported in current c compiler mode :(

    printf("camera update\n");
    int i = 0;
    while (i < 10)
    {
    	if (camera_update() == 0) {
            printf("Failed to update camera on iteration %d\n", i);
            camera_close();
            msleep(500);
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

        printf("Yellow Square:\nx: %d, y: %d, width: %d, height: %d\n", yellow_rect.ulx, yellow_rect.uly, yellow_rect.width, yellow_rect.height);
        printf("Red Square:\nx: %d, y: %d, width: %d, height: %d\n", red_rect.ulx, red_rect.uly, red_rect.width, red_rect.height);

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

    return detected;
}

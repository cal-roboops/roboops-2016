#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <drogon/wiringpi/wiringPi/wiringPi.h>
#include <drogon/wiringpi/wiringPi/softServo.h>

int main () {
    if (wiringPiSetup () == -1) {
    fprintf (stdout, "oops: %s\n", strerror (errno)) ;
    return 1 ;
    }

    softServoSetup(0) ;

    softServoWrite (0,  500) ;
/*
    softServoWrite (1, 1000) ;
    softServoWrite (2, 1100) ;
    softServoWrite (3, 1200) ;
    softServoWrite (4, 1300) ;
    softServoWrite (5, 1400) ;
    softServoWrite (6, 1500) ;
    softServoWrite (7, 2200) ;
*/
    // Hold variables
    int mode;
    int value;

    // Servo Control Loop
    while (true) {
        printf("Set value to: ");
        scanf("%d", value);
        softServoWrite(0, value);
        delay(1000);
    }

}
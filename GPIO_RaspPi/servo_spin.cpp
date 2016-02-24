#include <wiringPi.h>

int main (void) {
    int pos;
    wiringPiSetup () ;
    pinMode(15, OUTPUT) ;
    for (pos = 0; pos <= 180; pos++) {
    	digitalWrite(15, 1);
    	delay(15);
    }
    for (pos = 180; pos >= 0; pos--) {
    	digitalWrite(15, -1);
    	delay(15);
    }
    return 0;
}

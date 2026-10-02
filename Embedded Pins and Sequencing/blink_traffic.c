#include <signal.h>
#include <stdio.h>
#include <pigpio.h>


const int REDLED = 21;
const int YELLED = 16;
const int GRELED = 12; 

volatile sig_atomic_t signal_received = 0;

void sigint_handler (int signal) {
signal_received = signal;
}

int main() {
        if (gpioInitialise() == PI_INIT_FAILED) {
                printf("ERROR: Failed to initialize the GPIO interface.\n");
                return 1;
                }
        gpioSetMode(REDLED, PI_OUTPUT);
        gpioSetMode(YELLED, PI_OUTPUT);
        gpioSetMode(GRELED, PI_OUTPUT);
        signal(SIGINT, sigint_handler);
        printf("Press CTRL-C to exit.\n");
        while (!signal_received) {
            for (int i = 0; i < 3 && !signal_received; i++) {
                gpioWrite(REDLED, PI_HIGH);
                time_sleep(1);
                gpioWrite(REDLED, PI_LOW);
                time_sleep(1);
            }
            for (int i = 0; i < 3 && !signal_received; i++) {
                gpioWrite(YELLED, PI_HIGH);
                time_sleep(1);
                gpioWrite(YELLED, PI_LOW);
                time_sleep(1);
            }
            for (int i = 0; i < 3 && !signal_received; i++) {
                gpioWrite(GRELED, PI_HIGH);
                time_sleep(1);
                gpioWrite(GRELED, PI_LOW);
                time_sleep(1);
            }
        }
        gpioSetMode(REDLED, PI_INPUT);
        gpioSetMode(YELLED, PI_INPUT);
        gpioSetMode(GRELED, PI_INPUT);
        gpioTerminate();
        printf("\n");
        return 0;
}



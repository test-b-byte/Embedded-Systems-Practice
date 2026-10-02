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
        printf("ERROR: Failed to intialize the GPIO interface.\n");
        return 1;
    }
    gpioSetMode(REDLED, PI_OUTPUT);
    gpioSetMode(YELLED, PI_OUTPUT);
    gpioSetMode(GRELED, PI_OUTPUT);
    signal(SIGINT, sigint_handler);
    printf("Press CTRL-C to exit.\n");
    while (!signal_received) {
        gpioWrite(REDLED, PI_HIGH);
        for (int i = 0; i < 4 && !signal_received; i++) {
            time_sleep(5);
        }
        gpioWrite(REDLED, PI_LOW);
        gpioWrite(YELLED, PI_HIGH);
        time_sleep(2);
        gpioWrite(YELLED, PI_LOW);
        gpioWrite(GRELED, PI_HIGH);
        for (int i = 0; i < 7 && !signal_received; i++) {
            time_sleep(3);
        }
        gpioWrite(GRELED, PI_LOW);
    }
    gpioSetMode(REDLED, PI_INPUT);
    gpioSetMode(YELLED, PI_INPUT);
    gpioSetMode(GRELED, PI_INPUT);
    gpioTerminate();
    printf("\n");
    return 0;
}

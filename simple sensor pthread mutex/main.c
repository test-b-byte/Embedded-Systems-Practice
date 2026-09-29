#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "fakenews.h"

//mutex technique keys to stop simultaneous read/write bugs
pthread_mutex_t mutex;


int setT = 72;
int currentT = 72;
//off button gate
char sensorOn = 'a';  

// pthread calls up here, to allow user input, parent or UI thread
void * userControl(void *tid) {
    char input;
    while (sensorOn != 'x') {
        scanf(" %c", &input);
        if (input == 'x') {
            sensorOn = 'x';
        }
        else if (input == 't') {
            //create lock
            pthread_mutex_lock(&mutex);
            scanf("%d", &setT);
            printf("Set Temperature: %dF\n", setT);
            //unlock
            pthread_mutex_unlock(&mutex);
        }
        else {
            printf("Error: Press 'x' to exit or 't nn' so set temperature\n");
            //repeat bug was pissing me off so i added this char drain
            while (getchar() != '\n');
        }
    } 
    return NULL;
}
// temperature reports, uses sleep() to keep consistent report and slowly brings temp towards set
void * tempReport(void *tid) {

    while (sensorOn == 'a') {
        sleep(3);
        pthread_mutex_lock(&mutex);
        int lastT = currentT;
        if (setT > currentT) { 
            currentT = fakenews(currentT) + 1;}
        else if (setT < currentT) {
            currentT = fakenews(currentT) - 1;}
        else {
            currentT = fakenews(currentT);
        }
        if (lastT != currentT) {
            printf("Current Temperature: %dF\n", currentT);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


int main (void) {
    printf("Welcome to Den\n");
    printf("Set Temperatur: %dF\n", setT);

    pthread_t uiID;
    pthread_t trepID;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&uiID, NULL, userControl, (void *)&uiID);
    pthread_create(&trepID, NULL, tempReport, (void *)&trepID);
    pthread_exit(NULL);
    return 0;
}


// creating a lock: pthread_mutex_lock(&mutex)
// realease lock: pthread_mutex_unlock(&mutex) 

//Multithreading Using pthreads in C language (Part 1)
//https://www.youtube.com/watch?v=qPhP86HIXgg

//Multithreading in C Using Pthreads (part 2) - Order Violation Bug
//https://www.youtube.com/watch?v=zw8cNzX5ICc

//Mutex Introduction (pthreads) | C Programming Tutorial
//https://www.youtube.com/watch?v=raLCgPK-Igc&t=310s







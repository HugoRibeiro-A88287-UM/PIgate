#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h> 
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>

#include"../inc/utilits.h"
#include "../inc/daemon.h"

pid_t daemonEntriesDB, daemonUpdatePlate, daemonOpenGateDB;

//Thread Priority
static enum mainProcessPrio {captureCutImagePrio = 2, plateRecognitionPrio, textRecognitionPrio, updatePlatePrio, plateValidationPrio};


//Threads Functions Prototypes
void *t_captureCutImage(void *arg);
void *t_plateRecognition(void *arg);
void *t_textRecognition(void *arg);
void *t_updatePlate(void *arg);
void *t_plateValidation(void *arg);

static void signalHandler(int signo)
{		
    switch (signo)
    {
        case SIGINT:

            printf("Killing Deamon \n");
            kill(daemonEntriesDB,SIGTERM);
            kill(daemonUpdatePlate,SIGTERM);
            kill(daemonOpenGateDB,SIGTERM);
            exit(1);        
            break;
    
        default:
            break;
    }

    return;
}


int main(int argc, char *argv[])
{

    daemonEntriesDB = initDaemonEntriesDB();

    printf("Main PID -> %d \n", getpid());
    printf("Daemon PID -> %d \n", daemonEntriesDB);

    daemonUpdatePlate = initDaemonUpdatePlate();

    printf("Main PID -> %d \n", getpid());
    printf("Daemon PID -> %d \n", daemonUpdatePlate);

    daemonOpenGateDB = initDaemonOpenGateDB();

    printf("Main PID -> %d \n", getpid());
    printf("Daemon PID -> %d \n", daemonOpenGateDB);


    signal(SIGINT, signalHandler);



    /*Threads Creation*/

    pthread_attr_t thread_attr;
    struct sched_param thread_param;

    pthread_t captureCutImage_id, plateRecognition_id, textRecognition_id;
    pthread_t updatePlate_id, plateValidation_id;

    pthread_attr_init(&thread_attr);
    pthread_attr_getschedparam(&thread_attr, &thread_param);

    //Setup the thread priority and trys to create it. If successed the program continues. 
    setupThread(captureCutImagePrio, &thread_attr, &thread_param);
    checkFail(  pthread_create(&captureCutImage_id, &thread_attr, t_captureCutImage, NULL)  ); 

    setupThread(plateRecognitionPrio, &thread_attr, &thread_param);
    checkFail(  pthread_create(&plateRecognition_id, &thread_attr, t_plateRecognition, NULL)  );

    setupThread(textRecognitionPrio, &thread_attr, &thread_param);
    checkFail(  pthread_create(&textRecognition_id, &thread_attr, t_textRecognition, NULL)  );

    setupThread(updatePlatePrio, &thread_attr, &thread_param);
    checkFail(  pthread_create(&updatePlate_id, &thread_attr, t_updatePlate, NULL)  );

    setupThread(plateValidationPrio, &thread_attr, &thread_param);
    checkFail(  pthread_create(&plateValidation_id, &thread_attr, t_plateValidation, NULL)  );

    pthread_join(captureCutImage_id,NULL);
    pthread_join(plateRecognition_id,NULL);
    pthread_join(textRecognition_id,NULL);
    pthread_join(updatePlate_id,NULL);
    pthread_join(plateValidation_id,NULL);

    pthread_exit(NULL);

    return 0;

}


void *t_captureCutImage(void *arg)
{

    while (1)
    {
        
        sleep(5);
        printf("I took a photo! \n");


    }
    

}

void *t_plateRecognition(void *arg)
{

    while (1)
    {
        sleep(5);
        printf("OMG! There is a plate \n");
    }
    

}

void *t_textRecognition(void *arg)
{

    while (1)
    {
        sleep(5);
        printf("I detect the plate's text! \n");
    }
    

}

void *t_updatePlate(void *arg)
{

    while (1)
    {
        sleep(5);
        printf("Plates were updated \n");
    }
    

}

void *t_plateValidation(void *arg)
{

    while (1)
    {
        sleep(5);
        printf("Plate is VALID! \n");
    }
    

}
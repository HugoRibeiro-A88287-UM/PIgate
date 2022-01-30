#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

#include"../inc/utilits.h"
#include "../inc/daemon.h"
#include "../inc/firebase.h"


pid_t daemonEntriesDB, daemonUpdatePlate, daemonOpenGateDB;



char whitelistPlates[MAXPLATESLEN][PLATESSIZE];
int PLATESLEN = 0;
pthread_mutex_t updatePlatesMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for whitelistPlates buffer



//SIGALRM EACH MINUTE
struct itimerval itv = {{70,0}, {70,0}};

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
        case SIGTERM:
            kill(daemonEntriesDB,SIGTERM);
            kill(daemonUpdatePlate,SIGTERM);
            kill(daemonOpenGateDB,SIGTERM);
            exit(1);        
            break;

        case SIGUSR1:
            printf("Relay Activate \n");
            printf("Led is Green \n");
            setitimer (ITIMER_REAL, &itv, NULL); //Reset SIGALARM timer
            break;
        
        case SIGUSR2:
            printf("Led is Yellow \n");
            setitimer (ITIMER_REAL, &itv, NULL); //Reset SIGALARM timer
            break;
    
        case SIGALRM:
            printf("Led is IDLE \n");
            break;

        default:
            break;
    }

    return;
}


int main(int count, char *args[])
{

    //Check if PIgate exists!
    int fdPIgateID = open("/etc/PIgateID.txt", O_RDONLY);
    char PIgate_ID[PIGATELEN];

    if(fdPIgateID == -1)
    {
        printf("Cannot Open PIGateID.txt \n");
        exit(-1);
    }

    read(fdPIgateID,PIgate_ID, PIGATELEN);
    close(fdPIgateID);
    
    if( validPIgate(PIgate_ID) == -EXIT_FAILURE)
    {
        printf("Bad Request To Database Or PIgate Doesn't Exists \n");
        exit(-1);
    }

    //Create PIPEs

    if(pipe(recPlatePIPE) < 0)
    {
        printf("Error in recPlatePIPE creation");
        exit(1);
    }

    if(pipe(entriesDBPIPE) < 0)
    {
        printf("Error in entriesDBPIPE creation");
        exit(1);
    }


    daemonEntriesDB = initDaemonEntriesDB();
    daemonUpdatePlate = initDaemonUpdatePlate();
    daemonOpenGateDB = initDaemonOpenGateDB();

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);
    signal(SIGALRM, signalHandler);


    setitimer (ITIMER_REAL, &itv, NULL);

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
    
    printf("I took a photo! \n");
    
    while (1)
    {
        
        sleep(16);
        
        system("ps -A |grep ./daemon");

    }
    

}

void *t_plateRecognition(void *arg)
{

    printf("OMG! There is a plate \n");

    while (1)
    {
        sleep(5);
        
    }
    

}

void *t_textRecognition(void *arg)
{
    printf("I detect the plate's text! \n");

    while (1)
    {
        sleep(5);
        
    }
    

}

void *t_updatePlate(void *arg)
{
    const int inBuffSize = PLATESSIZE+1;
    char inBuff[inBuffSize];

    close(recPlatePIPE[1]); // Close writing end 

    while (1)
    {
        

        while( read(recPlatePIPE[0] , inBuff, inBuffSize) < 1 )
        {}

        pthread_mutex_lock(&updatePlatesMutex);

        PLATESLEN = atoi(inBuff);

        for(int i = 0 ; i < PLATESLEN ; i++)
        {
            read(recPlatePIPE[0] , inBuff, inBuffSize);

            removeHiffen(inBuff, inBuffSize);

            strcpy(whitelistPlates[i] , inBuff);

            printf("Storage Plate: %s \n", whitelistPlates[i] );
        }

        pthread_mutex_unlock(&updatePlatesMutex);
        
        sleep(7);
        
    }
    

}

void *t_plateValidation(void *arg)
{

    close(entriesDBPIPE[0]); // Close reading end
    bool whitelistPlate = false; 

    const char foundedPlate[] = "AA0002";

    while (1)
    {

        pthread_mutex_lock(&updatePlatesMutex);

        for(int i = 0 ; i < PLATESLEN && whitelistPlate == false ; i++)
        {
            whitelistPlate = ( strcmp(foundedPlate,whitelistPlates[i]) == 0);
        }

        pthread_mutex_unlock(&updatePlatesMutex);

        if(whitelistPlate)
        {

            write(entriesDBPIPE[1], foundedPlate, strlen(foundedPlate));

            whitelistPlate = false;

            printf(" GOOD PLATE \n ");
        }
        else
        {
            printf(" BAD PLATE \n ");
        }
        
        sleep(30);

    }
    

}
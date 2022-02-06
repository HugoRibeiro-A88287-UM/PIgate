/**
 * @file main.c
 * @author PIgate
 * @brief PIgateProgram Main Code
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h> /* For mode constants */
#include <linux/types.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h> /* For O_* constants */  
#include <stdint.h>

extern "C" {
#include"../inc/utilits.h"
#include "../inc/daemon.h"
#include "../inc/firebase.h"
#include "../inc/ledRGB.h"
#include "../inc/relay.h"

}

#include "../inc/licensePlate.hpp"
#include "../inc/fifo.hpp"

#include <iostream>

using namespace std;
using namespace cv;


//Daemons PID
pid_t daemonEntriesDB, daemonUpdatePlate, daemonOpenGateDB;

//Shared Memory Variables
char* PIGATE_ID;
uint32_t shmPIgateID;

//Whitelist Plates Variables
char whitelistPlates[MAXPLATESLEN][PLATESSIZE];
int PLATESLEN = 0;
pthread_mutex_t updatePlatesMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for whitelistPlates buffer

//FIFO1 Variables
Mat images[FIFOLEN];
fifoPhoto_t imagesFifo;


//FIFO2 Variables
uint16_t plates[FIFOLEN];
fifo16_t platesFifo;


//FIFO3 Variabels
arrayString plateText[FIFOLEN];
fifoString_t textFifo;

//SIGALRM EACH 30 Seconds
struct itimerval itv = {{30,0}, {30,0}};

//Thread Priority
enum mainProcessPrio {captureCutImagePrio = 2, plateRecognitionPrio, textRecognitionPrio, updatePlatePrio, plateValidationPrio};

//Threads Functions Prototypes
void *t_captureCutImage(void *arg);
void *t_plateRecognition(void *arg);
void *t_textRecognition(void *arg);
void *t_updatePlate(void *arg);
void *t_plateValidation(void *arg);

/**
 * @brief Parent Process Signal Handler
 * 
 * @param signo Received Signal
 */
static void signalHandler(int signo)
{		
    switch (signo)
    {
        case SIGINT:
        case SIGTERM:
            kill(daemonEntriesDB,SIGTERM);
            kill(daemonUpdatePlate,SIGTERM);
            kill(daemonOpenGateDB,SIGTERM);

            munmap(PIGATE_ID, PIGATELEN);
            close(shmPIgateID); /* Close the shared memory object */
            shm_unlink(SHM_PIGATEID_NAME); /* Delete the shared memory object */

            //Remove Device Drivers
            remRelay();
            remLedRGB();

            exit(1);        
            break;

        case SIGUSR1:
            printf("Relay Activate \n");
            openGate();

            printf("Led is Green \n");
            ledRGBStatus(allow);

            setitimer (ITIMER_REAL, &itv, NULL); //Reset SIGALARM timer
            break;
        
        case SIGUSR2:
            printf("Led is Yellow \n");
            ledRGBStatus(warning);

            setitimer (ITIMER_REAL, &itv, NULL); //Reset SIGALARM timer
            break;
    
        case SIGALRM:
            printf("Led is IDLE \n");
            ledRGBStatus(idle);
            break;

        default:
            break;
    }

    return;
}


int main(int count, char *args[])
{
    
    char PIgate_ID[PIGATELEN] = {'\0'};
    uint32_t shmMode;
    //Check if PIgate exists
    int fdPIgateID = open("/etc/PIgate/PIgateID.txt", O_RDONLY);

    installPyrebase();

    if(fdPIgateID == -1)
    {
        perror("Cannot Open PIGateID.txt \n");
        exit(-1);
    }

    
    read(fdPIgateID,PIgate_ID, PIGATELEN-1);  
    close(fdPIgateID);
    
    if( validPIgate(PIgate_ID) == -EXIT_FAILURE)
    {
        perror("Bad Request To Database Or PIgate Doesn't Exists \n");
        exit(-1);
    }

    //IPCs
    //Create PIPEs
    if(pipe(recPlatePIPE) < 0)
    {
        perror("Error in recPlatePIPE creation");
        exit(-1);
    }

    if(pipe(entriesDBPIPE) < 0)
    {
        perror("Error in entriesDBPIPE creation");
        exit(-1);
    }

    //Create Shared Memory
    /* Open the shared memory object */

    shmMode =   S_IRWXU |   //user (file owner) has read, write, and execute permission
                S_IRGRP;    // group has read permission
    if ( (shmPIgateID = shm_open(SHM_PIGATEID_NAME , O_CREAT|O_RDWR|O_TRUNC, shmMode)) == -1 ) 
    {
         perror("PIgateID shm_open failure");
         exit(-1);
    }

    /* Preallocate a shared memory area by determine the current 
    value of a configurable system limit for pagesize*/
    if(ftruncate(shmPIgateID, PIGATELEN) == -1)
    {
        perror("PIgateID ftruncate failure");
        exit(-1);
    }

    if((PIGATE_ID = (char*)mmap(0, PIGATELEN, PROT_WRITE|PROT_READ, MAP_SHARED,shmPIgateID,0)) == (caddr_t) -1) 
    {
        perror("PIgateID mmap failure");
        exit(-1);
    }

    strcpy(PIGATE_ID,PIgate_ID);

    //Daemons Creation

    daemonEntriesDB = initDaemonEntriesDB();
    daemonUpdatePlate = initDaemonUpdatePlate();
    daemonOpenGateDB = initDaemonOpenGateDB();

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);
    signal(SIGALRM, signalHandler);
    setitimer (ITIMER_REAL, &itv, NULL);

    //Adding Devices Drivers

    if( initLedRGB() || initRelay() )
    {
        remLedRGB();
        remRelay();
        perror("Error trying to insert Device Drivers \n");
        exit(-1);
    }

    ledRGBStatus(idle);

    //Create Fifos
    fifoPhoto_init(&imagesFifo,images,FIFOLEN);
    fifo16_init(&platesFifo,plates,FIFOLEN);
    fifoString_init(&textFifo,plateText,FIFOLEN);

    init_cascade();

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
    int gateStatus;
    Mat imageCapture(2,2,CV_8UC3, Scalar(0,0,255));

    printf("t_captureCutImage Thread is Ready \n");

    while (1)
    {
        sleep(1);

        gateStatus = getGateStatus();

        switch (gateStatus)
        {
            case gateOpen:
                /*Do Nothing*/
                break;

            case -EXIT_FAILURE:
                ledRGBStatus(warning);
                break;

            case gateClose:

                if(get_FifoPhotoBuffSize(imagesFifo) == FIFOLEN)
                {
                    /*Ignore, FIFO is FULL*/
                }
                else
                {
                    imageCapture = take_picture();

                    cout << "Pushing Image" << endl;

                    fifoPhoto_push(&imagesFifo,imageCapture);
                }

                break;

            default:
                /*FATAL ERROR*/
                printf("Error Getting gateStatus in t_captureCutImage \n");
                kill(getpid(),SIGTERM);
                break;
        }

        sleep(10);

    }
    

}

void *t_plateRecognition(void *arg)
{
    int gateStatus;
    int detectPlateReturn;
    Mat receivedImage;
    Mat plateImage;

    printf("t_plateRecognition is ready \n");

    while (1)
    {
        
        while(fifoPhoto_pop(&imagesFifo,&receivedImage) == -ENODATA )
        { /*Waits for an image*/
            sleep(1);
        }
        
        gateStatus = getGateStatus();

        switch (gateStatus)
        {
            case gateOpen:
                /*Clears the FIFO since the gate is already open*/
                clear_fifoPhoto(&imagesFifo);
                break;

            case -EXIT_FAILURE:
                ledRGBStatus(warning);
                break;

            case gateClose:

                if(get_Fifo16BuffSize(platesFifo) == FIFOLEN)
                {
                    /*Ignore, FIFO is FULL*/
                }
                else
                {
                    detectPlateReturn = detectPlate(receivedImage, (platesFifo.writeIndex & (platesFifo.buff_len-1) ) );

                    if(detectPlateReturn == -EXIT_FAILURE)
                    {
                        /*ERROR, PLATE NOT FOUNDED*/
                    }
                    else
                    {   
                        cout << "Pushing index photo" << endl;

                        fifo16_push(&platesFifo,detectPlateReturn);
                    }

                }

                break;

            default:
                /*FATAL ERROR*/
                printf("Error Getting gateStatus in t_plateRecognition \n");
                kill(getpid(),SIGTERM);
                break;
        }
        
    }
    

}

void *t_textRecognition(void *arg)
{
    int gateStatus;
    int16_t receivedPlate;
    string plateString;

    printf("t_textRecognition is ready! \n");

    while (1)
    {
        
        while( (receivedPlate = fifo16_pop(&platesFifo) ) == -ENODATA )
        { /*Waits for an image*/
            sleep(1);
        }
        
        gateStatus = getGateStatus();

        switch (gateStatus)
        {
            case gateOpen:
                /*Clears the FIFO since the gate is already open*/
                clear_fifo16(&platesFifo);
                break;

            case -EXIT_FAILURE:
                ledRGBStatus(warning);
                break;

            case gateClose:

                if(get_FifoStringBuffSize(textFifo) == FIFOLEN)
                {
                    /*Ignore, FIFO is FULL*/
                }
                else
                {
                   
                    plateString = read_license_plates(receivedPlate);

                    if(plateString == "ERROR")
                    {
                        /*Plate String not founded*/
                    }
                    else
                    {
                        cout << "Pushing text string" << plateString << endl;
                        fifoString_push(&textFifo,plateString.c_str());
                    }
                }

                break;

            default:
                /*FATAL ERROR*/
                printf("Error Getting gateStatus in t_textRecognition \n");
                kill(getpid(),SIGTERM);
                break;
        }
        
    }
    

}

void *t_updatePlate(void *arg)
{
    char inBuff[PLATESSIZE];
    close(recPlatePIPE[1]); // Close writing end 

    printf("t_updatePlate is ready! \n");

    //Little Delay
    sleep(2);

    while (1)
    {
        

        while( read(recPlatePIPE[0] , inBuff, PLATESSIZE) < 1 )
        {}

        //Convert a string into integer
        PLATESLEN = atoi(inBuff);

        pthread_mutex_lock(&updatePlatesMutex);

        for(int i = 0 ; i < PLATESLEN ; i++)
        {
            read(recPlatePIPE[0] , inBuff, PLATESSIZE);

            strcpy(whitelistPlates[i] , inBuff);

            printf("Storage Plate: %s \n", whitelistPlates[i] );
        }

        pthread_mutex_unlock(&updatePlatesMutex);
        
        //Little delay
        sleep(2);
        
    }
    

}

void *t_plateValidation(void *arg)
{

    char foundedPlate[PLATESSIZE];
    close(entriesDBPIPE[0]); // Close reading end
    bool whitelistPlate = false; 

    printf("t_plateValidation is ready! \n");

    while (1)
    {

        while (fifoString_pop(&textFifo,foundedPlate) == -ENODATA)
        { /*Waiting for need plates*/ }
        

        pthread_mutex_lock(&updatePlatesMutex);

        for(int i = 0 ; i < PLATESLEN && whitelistPlate == false ; i++)
        {
            whitelistPlate = ( strcmp(foundedPlate,whitelistPlates[i]) == 0);
        }

        pthread_mutex_unlock(&updatePlatesMutex);

        if(whitelistPlate)
        {

            write(entriesDBPIPE[1], foundedPlate, strlen(foundedPlate));

            printf(" GOOD PLATE \n ");
            setitimer (ITIMER_REAL, &itv, NULL); //Reset SIGALARM timer
            clear_fifoString(&textFifo);
            ledRGBStatus(allow);
            openGate();
        }
        else
        {
            printf(" BAD PLATE \n ");
            setitimer (ITIMER_REAL, &itv, NULL); //Reset SIGALARM timer
            ledRGBStatus(denied);
        }

        //clean withlistPlate flag
        whitelistPlate = false;
        
        sleep(1);

    }
    

}
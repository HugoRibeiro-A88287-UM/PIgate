 /** @file daemon.c
 * @author PIgate
 * @brief Daemons Module Implementation
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <sys/syslog.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sched.h>
#include <stdbool.h>


#include "../inc/utilits.h"
#include "../inc/firebase.h"
#include "../inc/daemon.h"

pid_t parentPID;
uint32_t shmPIgateID;
char* PIGATE_ID;

/**
 * @brief Opens Shared Memory to get the PIgate_ID from the parents proccess
 * 
 */
static void openSharedMemory(void)
{
    
    /* Open the shared memory object */
   if ( (shmPIgateID = shm_open(SHM_PIGATEID_NAME, O_RDONLY, 0)) == -1 ) 
   {
        syslog(LOG_ERR,"PIGATE_ID DAEMON shm_open failure");
        kill(parentPID,SIGTERM);
   }

   if((PIGATE_ID = mmap(0, PIGATELEN, PROT_READ, MAP_SHARED,shmPIgateID,0)) == (caddr_t) -1)
   {
        syslog(LOG_ERR," PIGATE_ID DAEMON mmap failure");
        kill(parentPID,SIGTERM);
   }

    syslog(LOG_INFO,"PIgate_ID: %s", PIGATE_ID);

}


/**
 * @brief Daemons Signal Handler
 * 
 * @param signo received signal
 */
static void signalHandler(int signo)
{		

    switch (signo)
    {
        case SIGTERM:
            syslog(LOG_INFO, "Deamon dead xP \n");

            munmap(PIGATE_ID, PIGATELEN);
            close(shmPIgateID); /* Close the shared memory object */
            exit(1);        
            break;
    
        default:
            break;
    }

    return;
}


/**
 * @brief EntriesDB Execution Function
 * 
 */
static void entriesDB(void)
{
    const int inBuffSize = PLATESSIZE+1;
    char inBuff[inBuffSize];


    signal(SIGTERM, signalHandler);    
    openSharedMemory();
    close(entriesDBPIPE[1]); // Close writing end 


    syslog(LOG_INFO, "entriesDB is ON \n");

    while (1)
    {


        while( read(entriesDBPIPE[0] , inBuff, inBuffSize) < 1 )
        {}

        insertPlateHiffen(inBuff,inBuffSize);

        if ( sendEntry(PIGATE_ID, inBuff) == -EXIT_FAILURE)
        {
            //Send warning Signal
            kill(parentPID,SIGUSR2);
        }
        else
        {
            syslog(LOG_INFO, "Entry (%s,%s)sent ! :D", PIGATE_ID, inBuff);
        }
        
        
    }
    

}


/**
 * @brief updatePlates Execution Function
 * 
 */
static void updatePlates(void)
{
    signal(SIGTERM, signalHandler);
    openSharedMemory();


    syslog(LOG_INFO, "updatePlate is ON \n");

    while (1)
    {
        
        if( receivePlates() == -EXIT_FAILURE )
        {
            //Send warning Signal
            kill(parentPID,SIGUSR2);

        }

        sleep(60);

    }
}

/**
 * @brief openGateDB Execution Function
 * 
 */
static void openGateDB(void)
{
    int gate = false;


    signal(SIGTERM, signalHandler);
    openSharedMemory();


    syslog(LOG_INFO, "openGateDB is ON \n");
    
    while (1)
    {
        sleep(15);

        gate = isToOpen(PIGATE_ID);

        syslog(LOG_INFO, "isToOpen Recevied: %d \n", gate);

        switch (gate)
        {
        case true:
            kill(parentPID,SIGUSR1);
            break;

        case false:
            break;

        default: //Error getting isToOpen Flag
            kill(parentPID,SIGUSR2);
            break;
        }
        
    }
}

pid_t initDaemonEntriesDB(void)
{
    pid_t pid, sid;
    static int firstTime = 1;

    if(!firstTime)
        return -EACCES;
    
    firstTime = 0;

    parentPID  = getpid();

    // create a new process
    pid = fork(); 

    if (pid < 0)
    { // on error exit
        syslog(LOG_ERR, " Error in fork \n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {   
        //Parent Process
        //Return the Deamons PID
        firstTime = 0;
        return pid;
    }

    //Create Deamon Process

    sid = setsid(); // create a new session

    if (sid < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "setsid");
        exit(EXIT_FAILURE);
    }

    // make '/' the root directory
    if (chdir("/") < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "chdir");
        exit(EXIT_FAILURE);
    }
    umask(0);

    close(STDIN_FILENO);  // close standard input file descriptor
    close(STDOUT_FILENO); // close standard output file descriptor
    close(STDERR_FILENO); // close standard error file descriptor

    entriesDB();

    return -EXIT_FAILURE;
}

pid_t initDaemonUpdatePlate(void)
{
    pid_t pid, sid;
    static int firstTime = 1;

    if(!firstTime)
        return -EACCES;
    
    firstTime = 0;

    parentPID  = getpid();

    // create a new process
    pid = fork(); 

    if (pid < 0)
    { // on error exit
        syslog(LOG_ERR, " Error in fork \n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {   
        //Parent Process
        //Return the Deamons PID
        firstTime = 0;
        return pid;
    }

    //Create Deamon Process

    sid = setsid(); // create a new session

    if (sid < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "setsid");
        exit(EXIT_FAILURE);
    }

    // make '/' the root directory
    if (chdir("/") < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "chdir");
        exit(EXIT_FAILURE);
    }
    umask(0);

    close(STDIN_FILENO);  // close standard input file descriptor
    close(STDOUT_FILENO); // close standard output file descriptor
    close(STDERR_FILENO); // close standard error file descriptor


    updatePlates();

    return -EXIT_FAILURE;

}

pid_t initDaemonOpenGateDB(void)
{
    pid_t pid, sid;
    static int firstTime = 1;

    if(!firstTime)
        return -EACCES;
    
    firstTime = 0;

    parentPID  = getpid();

    // create a new process
    pid = fork(); 

    if (pid < 0)
    { // on error exit
        syslog(LOG_ERR, " Error in fork \n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {   
        //Parent Process
        //Return the Deamons PID
        firstTime = 0;
        return pid;
    }

    //Create Deamon Process

    sid = setsid(); // create a new session

    if (sid < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "setsid");
        exit(EXIT_FAILURE);
    }

    // make '/' the root directory
    if (chdir("/") < 0)
    { // on error exit
        syslog(LOG_ERR, "%s\n", "chdir");
        exit(EXIT_FAILURE);
    }
    umask(0);

    close(STDIN_FILENO);  // close standard input file descriptor
    close(STDOUT_FILENO); // close standard output file descriptor
    close(STDERR_FILENO); // close standard error file descriptor

    openGateDB();

    return -EXIT_FAILURE;
}

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


static void signalHandler(int signo)
{		

    switch (signo)
    {
        case SIGTERM:
            syslog(LOG_INFO, "Deamon dead xP \n");
            exit(1);        
            break;
    
        default:
            break;
    }

    return;
}

static void entriesDB(void)
{
    const int inBuffSize = PLATESSIZE+1;
    char inBuff[inBuffSize];

    signal(SIGTERM, signalHandler);
    syslog(LOG_INFO, "entriesDB is ON \n");

    close(entriesDBPIPE[1]); // Close writing end 

    while (1)
    {


        while( read(entriesDBPIPE[0] , inBuff, inBuffSize) < 1 )
        {}

        insertHiffen(inBuff,inBuffSize);

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

static void updatePlates(void)
{
    signal(SIGTERM, signalHandler);
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

static void openGateDB(void)
{
    signal(SIGTERM, signalHandler);
    syslog(LOG_INFO, "openGateDB is ON \n");
    int gate = false;

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

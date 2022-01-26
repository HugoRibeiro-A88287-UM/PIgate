#include "../inc/relay.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

enum status {ON = 0, OFF};
const char relayOpen = '1';
const char relayClose = '0';


relayStatus_t relayStatus;
char isRelayModuleActive = OFF;

int initRelay()
{
    while (system("insmod relay.ko") != 0 )
    {}

    isRelayModuleActive = ON;

    return EXIT_SUCCESS;
}

void remRelay()
{
    system("rmmod relay.ko");
    isRelayModuleActive = OFF;
    return;
}

int openGate()
{
    if(isRelayModuleActive == ON)
    {
        int fd0 = open("/dev/relay0", O_WRONLY);
        if(fd0 == -1)
        {   /*ERROR OPENING relay0*/
            printf("ERROR: Cannot Open Relay Device Driver \n");
            return -EXIT_FAILURE;
        }   

        if( write(fd0,&relayOpen,1) != 1)
        {
            printf("Failure Open: Write Operation in Relay Device Driver \n");
            return -EXIT_FAILURE;
        }
        sleep(1); /*Little delay */

        if( write(fd0,&relayClose,1) != 1)
        {
            printf("Failure Close: Write Operation in Relay Device Driver \n");
            return -EXIT_FAILURE;
        }

        close(fd0);
        return EXIT_SUCCESS;
    }

    printf("ERROR: Relay module is not active \n");
    return -EXIT_FAILURE;

}

int getGateStatus()
{
    if(isRelayModuleActive == ON)
    {
        int fd0 = open("/dev/relay0", O_RDONLY);
        if(fd0 == -1)
        {   /*ERROR OPENING relay0*/
            printf("ERROR: Cannot Open Relay Device Driver \n");
            return -EXIT_FAILURE;
        }   

        if( read(fd0,&relayStatus,1) != 1)
        {
            printf("Failure Open: Write Operation in Relay Device Driver \n");
            return -EXIT_FAILURE;
        }
        close(fd0);

        return relayStatus;
        
    }

    printf("ERROR: Relay module is not active \n");
    return -EXIT_FAILURE;
}


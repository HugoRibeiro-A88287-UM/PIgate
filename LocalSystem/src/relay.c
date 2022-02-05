/**
 * @file relay.c
 * @author PIgate
 * @brief Relay Module Implementation
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "../inc/relay.h"

//Define the Module Status 
typedef enum moduleStatus {ON = 0, OFF} moduleStatus_t;
const char relayOpen[] = "OPEN";
const char relayClose[] = "CLOSE";

gateStatus_t gateStatus;
moduleStatus_t isRelayModuleActive = OFF;

int initRelay(void)
{
    if( system("insmod relay.ko") == 0 )
    {
        isRelayModuleActive = ON;
        return EXIT_SUCCESS;
    }

    return -EXIT_FAILURE;
    
}

void remRelay(void)
{
    system("rmmod relay.ko");
    isRelayModuleActive = OFF;
    return;
}

int openGate(void)
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
        sleep(1); /*Little delay to generate a pulse */

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

int getGateStatus(void)
{
    char pbuff;

    if(isRelayModuleActive == ON)
    {
        int fd0 = open("/dev/relay0", O_RDONLY);
        if(fd0 == -1)
        {   /*ERROR OPENING relay0*/
            printf("ERROR: Cannot Open Relay Device Driver \n");
            return -EXIT_FAILURE;
        }   

        if( read(fd0, &pbuff,1) != 1)
        {
            printf("Failure Open: Write Operation in Relay Device Driver \n");
            return -EXIT_FAILURE;
        }
        close(fd0);

        if(pbuff == '0')
            gateStatus = gateClose;
        else
            gateStatus = gateOpen;

        return gateStatus;
        
    }

    printf("ERROR: Relay module is not active \n");
    return -EXIT_FAILURE;
}


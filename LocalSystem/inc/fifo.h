/**
 * @file fifo.h
 * @author srhugo Pedro Martins
 * @brief Generic FIFO module
 * @version 0.1
 * @date 2021-05
 * @review 2022-02-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _FIFO_MODULE_H
#define _FIFO_MODULE_H

#include <stdint.h>
#include "utilits.h"

typedef char arrayString[PLATESSIZE];

typedef struct serialFifoString
{
    uint32_t writeIndex;
    uint32_t readIndex;
	uint32_t buff_len;
    arrayString* p_buff;

}fifoString_t;


/**
 * @brief Initializes one fifo with the default indexs and 
 * and receive parameters
 * 
 * @param fifo Specifics the fifo
 * @param array Pointer to a buffer that fifo can storage info
 * @param len Buffer length
 * @return  Return EXIT_SUCESS if all went well, or ENODATA
 *						if an error occurs
 */
int fifoString_init(fifoString_t *fifo, arrayString* array, uint32_t len);


/**
 * @brief Adds the wanted data into the fifo
 * 
 * @param fifo Specifics the fifo
 * @param data What the user want to add in the fifo
 * @return Return EXIT_SUCESS if all went well, or -ENOBUFS
 *						if an error occurs 
 */
int fifoString_push(fifoString_t *fifo,const char* data);


/**
 * @brief This funtion gets a byte from the fifo
 * 
 * @param fifo Specifics the fifo
 * @param return Where the string will be return
 * @return return one position of the buffer or 
 *      If the fifo is empty return -ENODATA
 *      Otherwise error -EXIT_FAILURE
 */
int fifoString_pop(fifoString_t *fifo,char* returnString);


/**
 * @brief Get the FifoBuffSize object
 * 
 * @param fifo Specifics the fifo
 * @return Return the  
 */
uint32_t get_FifoStringBuffSize(fifoString_t fifo);

/**
 * @brief Discarts all the fifo content. No way to reverse
 * 
 * @param fifo Specifics the fifo
 */
void clear_fifoString(fifoString_t *fifo);


#endif


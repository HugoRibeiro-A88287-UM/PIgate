#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "../inc/fifo.h"

int fifoString_init(fifoString_t *fifo, arrayString* array, uint32_t len)
{
	//Checks if array is NULL
	if(array == NULL)
		return -ENODATA; 

    fifo->writeIndex = 0; //Reset indexs
    fifo->readIndex = 0;
	fifo->buff_len = len;
    fifo->p_buff = array;
	
    return EXIT_SUCCESS;
}


int fifoString_push(fifoString_t *fifo,const char* data)
{
	//Checks if there are space available in the buffer
	
	//If not, ERROR FIFO FULL
	if(get_FifoStringBuffSize(*fifo) == fifo->buff_len)
		return -ENOBUFS;



	strcpy( fifo->p_buff[fifo->writeIndex & (fifo->buff_len-1)] , data) ;
	//If yes, storage the byte in position writeIndex

	fifo->writeIndex++;
	return EXIT_SUCCESS;
}

int fifoString_pop(fifoString_t *fifo, char* returnString)
{
	//Checks if any byte to be read
	//If not return -ENODATA
    if(get_FifoStringBuffSize(*fifo) == 0 )
			return -ENODATA;

	//If yes gets the byte from the buffer and return it
    strcpy( returnString , (fifo->p_buff[fifo->readIndex & (fifo->buff_len-1)]));

	fifo->readIndex++;

	return EXIT_SUCCESS;

}

uint32_t get_FifoStringBuffSize(fifoString_t fifo)
{
	return (fifo.writeIndex - fifo.readIndex);
}

void clear_fifoString(fifoString_t *fifo)
{
	fifo->readIndex = fifo->writeIndex;
}

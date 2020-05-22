/* Very simple queue
 * These are FIFO queues which discard the OLD data when full.
 
 * Queue is empty when in == out.
 * If in != out, then 
 *  - items are placed into in before incrementing in
 *  - items are removed from out before incrementing out
 * Queue is full when in == (out-1 + QUEUE_SIZE) % QUEUE_SIZE;
 *
 * WTF is below???
 * The queue will hold QUEUE_ELEMENTS number of items before the
 * calls to QueuePut fail.
 */

#include "dvb_fifo.h"

char Queue[QUEUE_SIZE];
long writePointer, readPointer;


void QueueInit(void)
{
    writePointer = readPointer = 0;
}

char QueuePut(char data)
{
    
    Queue[writePointer] = data;

    writePointer = (writePointer + 1) % QUEUE_SIZE;

    if(writePointer == (( readPointer - 1 + QUEUE_SIZE) % QUEUE_SIZE))
    {
        return -1; // Queue Full
    }

    return 0; // No errors
}

char QueueGet(char *old)
{
    if(writePointer == readPointer)
    {
        return -1; /* Queue Empty - nothing to get*/
    }

    *old = Queue[readPointer];

    readPointer = (readPointer + 1) % QUEUE_SIZE;

    return 0; // No errors
}

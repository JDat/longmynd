#ifndef DVB_FIFO_H_
#define DVB_FIFO_H_
//#include <linux/compiler.h> /* __must_check */

/* Queue structure */
#define QUEUE_ELEMENTS 65536
#define QUEUE_SIZE (QUEUE_ELEMENTS + 1)
extern char Queue[QUEUE_SIZE];
extern long writePointer, readPointer;

void QueueInit(void);
char QueuePut(char data);
char QueueGet(char *old);

#endif //DVB_FIFO_H_

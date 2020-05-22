#ifndef GPIO_IRQ_H_
#define GPIO_IRQ_H_
#include <linux/compiler.h> /* __must_check */

//long *dvbGpioRegister = 0x3f000000;
extern ulong gpioBase;

__must_check int register_irq(void); /* 0 if Ok*/

void unregister_irq(void); 

#endif //GPIO_IRQ_H_

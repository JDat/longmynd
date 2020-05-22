#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/interrupt.h>
#include <linux/gpio.h>

#include "gpio_irq.h"
#include "dvb_fifo.h"

// #define DRIVER_AUTHOR "JDat Deface, inspired from Igor <hardware.coder@gmail.com>"
// #define DRIVER_DESC   "Longmynd TS reader"

// Interrupt pin
#define IRQ_PIN                17

// How much to shift
#define numShiftBits                5

#define IRQF_TRIGGER_MODE           IRQF_TRIGGER_FALLING

// text below will be seen in 'cat /proc/interrupt' command
#define IRQ_PIN_DESCRIPTION           "Some gpio pin description"

// below is optional, used in more complex code, in our case,
// this could be NULL
#define IRQ_DEVICE_DESCRIPTION    "some_device"

ulong gpioBase;

/* Interrupts variables block */
short int irq_any_gpio    = 0;

char data;
ulong gpioData;

/* IRQ handler */
static irqreturn_t irq_handler(int irq, void *dev_id, struct pt_regs *regs) {

   unsigned long flags;

   // disable hard interrupts (remember them in flag 'flags')
   local_irq_save(flags);

   //do your stuff here
   
   //read gpio register
   gpioData = gpioBase;
   
   //shift bits to get 8 bit value
   data = (char)gpioData >> numShiftBits;
   
   //put into FIFO
   QueuePut(data);
   
   // done with stuff

   //printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered !.\n",
   //       irq, (char *) dev_id);

   // restore hard interrupts
   local_irq_restore(flags);

   return IRQ_HANDLED;
}

/* Configures interrupts. */
void r_int_config(void) {

   if (gpio_request(IRQ_PIN, IRQ_PIN_DESCRIPTION)) {
      printk("GPIO request faiure: %s\n", IRQ_PIN_DESCRIPTION);
      return;
   }

   if ( (irq_any_gpio = gpio_to_irq(IRQ_PIN)) < 0 ) {
      printk("GPIO to IRQ mapping failure %s\n", IRQ_PIN_DESCRIPTION);
      return;
   }

   printk(KERN_NOTICE "Mapped int %d\n", irq_any_gpio);

   if (request_irq(irq_any_gpio,
                   (irq_handler_t ) irq_handler,
                   IRQF_TRIGGER_MODE,
                   IRQ_PIN_DESCRIPTION,
                   IRQ_DEVICE_DESCRIPTION)) {
      printk("Irq Request failure\n");
      return;
   }

   return;
}

/* Module init / cleanup block. */
int register_irq(void) {

   printk(KERN_NOTICE "Hello GPIO IRQ!\n");
   r_int_config();

   return 0;
}

void unregister_irq(void) {
   printk(KERN_NOTICE "Goodbye GPIO IRQ\n");
   /* This function releases interrupts. */
   free_irq(irq_any_gpio, IRQ_DEVICE_DESCRIPTION);
   gpio_free(IRQ_PIN);

   return;
}

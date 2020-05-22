TARGET_MODULE:=datv-module

# If we running by kernel building system
ifneq ($(KERNELRELEASE),)
	$(TARGET_MODULE)-objs := main.o device_file.o gpio_irq.o dvb_fifo.o
	obj-m := $(TARGET_MODULE).o

# If we are running without kernel build system
else
	BUILDSYSTEM_DIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)


all : 
# run kernel build system to make module
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules

clean:
# run kernel build system to cleanup in current directory
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean
	rm /dev/simple_driver

load:
	insmod ./$(TARGET_MODULE).ko
	mknod /dev/simple_driver c 241 0

unload:
	rmmod ./$(TARGET_MODULE).ko
	rm /dev/simple_driver

endif



obj-m += chr_drv_ex1.o

PWD := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

install:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install
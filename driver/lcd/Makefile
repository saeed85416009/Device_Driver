obj-m := gpio-sysfs.o
ARCH=arm
CROSS_COMPILE=arm-linux-gnu-
#set KERN_DIR to linux source location 
KERN_DIR = /root/embedded/beaglebone/soft/linux_bbb_5.4 
HOST_KERN_DIR = /lib/modules/`uname -r`/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules
clean:
	make -C $(KERN_DIR) M=$(PWD) clean
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean
	rm -rf *.o *.elf
help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help
host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

copy-dtb: 
	scp ~/workspace/ldd/source/linux_bbb_5.4/arch/arm/boot/dts/am335x-boneblack.dtb debian@192.168.7.2:/home/debian/drivers

copy-drv: 
	scp *.ko *.elf debian@192.168.7.2:/home/debian/drivers

app: lcd_app.o lcd.o gpio.o lcd_app.elf

lcd_app.o: lcd_app.c
	$(CROSS_COMPILE)gcc -c -o $@ $^

lcd.o: lcd.c
	$(CROSS_COMPILE)gcc -c -o $@ $^

gpio.o: gpio.c
	$(CROSS_COMPILE)gcc -c -o $@ $^

lcd_app.elf: lcd_app.o lcd.o gpio.o
	$(CROSS_COMPILE)gcc -o $@ $^






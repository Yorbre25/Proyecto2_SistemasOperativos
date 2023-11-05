obj-m += cdc-acm-pro.o

all: build

build:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	clear

test:
	gcc test.c -o test
	./test


# notes:
# 	ls /dev/dummydriver
# 	sudo chmod 666 /dev/arduino_usb
# 	echo "This is a test" > /dev/arduino_usb
# 	head -n 1 /dev/arduino_usb
# 	sudo dmesg | tail -n 15

# notas2:
# sudo dmesg -w
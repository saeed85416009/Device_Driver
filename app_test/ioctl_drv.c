#include "header.h"

//define ioctl code
#define WR_DATA _IOW('a', 'a', int32_t*)
#define RD_DATA _IOR('a', 'b', int32_t*)


int main(){
	
	int fd;
	int32_t val, num;
	printf("\n IOCTL based character device driver operation from user space");
	fd=open("/dev/my_device",O_RDWR);
	if(fd<0){
		printf("cannot open the device file\n");
		return -1;
	}
	
	printf("Enter the data to send ...");
	scanf("%d", &num);
	printf("writing value to the driver ...\n");
	ioctl(fd,WR_DATA, (int32_t*)&num);
	
	printf("Read from the driver ...");
	ioctl(fd, RD_DATA, (int32_t*)&val);
	printf("read data = %d\n",val);
	
	printf("closing the driver ... \n");
	close(fd);
	
	return 0;
	
	
}


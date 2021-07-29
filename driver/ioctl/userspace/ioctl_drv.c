#include "header.h"

// define ioctl code
#define WR_DATA  _IOW('a', 'a', int32_t*)
#define RD_DATA  _IOR('b', 'b', int32_t*)

int main(){

	int fd;
	int32_t val, num;
	printf("\n IOCT based character device driver operation from user space");
	fd = open("/dev/pcd", O_RDWR);
		if(fd<0){
			printf("cannot open the device file");
			return -1;
		}
	printf("Enter the data to send ... : \n");
	scanf("%d", &num);
	printf("write to the Driver ... : \n");
	ioctl(fd, WR_DATA, (int32_t*) &num);

	printf("Read from the Driver ... : \n");
	ioctl(fd, RD_DATA, (int32_t*) &val);
	printf("read data = %d \n", val);
	close(fd);

	return 0;


}

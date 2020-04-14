#include <header.h>

#define BASEPORT 0x70
int main (){

// get access to the port
if(ioperm(BASEPORT,4,1)){
	perror("ioperm");
	exit(1);
}
	
}
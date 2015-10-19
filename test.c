#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

main()
{
	int fd;
	fd = open("test.c",O_RDONLY);
	if(fd == -1)
		printf("\n\tError");
		
	printf("\n\tHello\n");
	
	
}

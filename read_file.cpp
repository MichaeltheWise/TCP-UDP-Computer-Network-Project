#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

int main() {
	FILE *file = fopen("nums.csv", "r");
	if (file == NULL) 
	{
		printf("Can't open!\n");
		exit(1);
	}
	int buffer = 0;
	int count = 0;
	while (fscanf(file,"%d",&buffer) != EOF)
	{
		count++;
		// printf("Number is = %d\n", buffer);
	}
	// printf("Count: %d\n", count);
	fclose(file);

	FILE *ifp = fopen("nums.csv", "r");
	int i = 0;
	int buf[count];
	int num = 0;
	while (fscanf(ifp,"%d",&num) != EOF)
	{
		buf[i] = num;
		i = i + 1;
	}

	int* pt = buf;

	char * str [5];
	int leng = (int)( sizeof(str) / sizeof(str[0]));
	printf("Size is %d\n", leng);

	// printf("Number is = %d\n", buf[1]);
	// printf("Number is = %d\n", buf[264]);
	// printf("Number is = %d\n", pt[2]);
	

}
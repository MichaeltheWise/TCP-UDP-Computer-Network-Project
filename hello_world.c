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


int main()
{
	int sock, cli;
	struct sockaddr_in server, client; 
	unsigned int len;
	int sent;
	char msg[] = "Hello World!\n";

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket: ");
		exit(-1); 
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(10000);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero,8);

	len = sizeof(struct sockaddr_in);

	if ((bind(sock, (struct sockaddr*)&server, len)) == -1)
	{
		perror("bind");
		exit(-1);
	}

	if ((listen(sock, 5)) == -1)
	{
		perror("listen");
		exit(-1);
	}

	while(1)
	{
		if ((cli = accept(sock, (struct sockaddr*)&client, &len)) == -1)
		{
			perror("accpet");
			exit(-1);
		}

		sent = send(cli, msg, strlen(msg), 0);
		printf("Sent %d bytes to client: %s\n", sent, inet_ntoa(client.sin_addr));

		close(cli);


	}

}
/*
EE 450 Session 3
Socket Programming
Written by Po-Hsuan (Michael) Lin
*/

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
#include <string>
#include <iostream>
#include <fstream>
#include "client.h"
using namespace std;

/* Only TCP connection for the client */ 

// Define descriptors
int TCP_descriptor;

// Define TCP sockaddr_in
// TCP here is used to connect aws to client
struct sockaddr_in TCP_client_socket_addr;
struct sockaddr_in TCP_aws_socket_addr; 

int main(int argc, char* argv[]) 
{
	cout << "The client is up and running." << endl;
	int* pt = read_csv();
	int count = read_file();
	// printf ("Number: %d\n",pt[2]);
	setup_TCP_socket();
	connect_TCP_socket();
	char instr[4];
	strcpy(instr, argv[1]);
	send_length();
	send_TCP_socket_instr(instr,count);
	cout << "The client has sent the reduction type " << instr << " to AWS." << endl;
	send_TCP_socket(pt,count);
	int answer = receive_reduction_answer();
	cout << "The AWS has received reduction " << instr << ": " << answer << endl;
	// close_TCP_socket();
}

// Read in the number file into array
int* read_csv()
{
	// Find the file size
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
	}
	fclose(file);

	int i = 0;
	string buf;
	int* number_array = new int[count];
	ifstream ifp ("nums.csv");
	// Create an array and store all the data into that array
	while (getline(ifp,buf))
	{
		number_array[i] = atoi(buf.c_str());
		i = i + 1;
		// cout << number_array[i] << endl;
	}
	// cout << i << endl;
	/*
	for (int j = 0; j < i; j++)
	{
		cout << number_array[j] << endl;
	}
	*/
	return number_array;
}

/* TCP CONNECTION */
// Set up TCP connection
void setup_TCP_socket()
{
	TCP_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (TCP_descriptor < 0)
	{
		perror("Error creating TCP socket");
		exit(-1);
	}
}

// Connect TCP connection
void connect_TCP_socket() 
{
	// struct hostent *server_IP_host_address = resolve_host_name("nunki.usc.edu");
	// struct in_addr **server_IP_address = (struct in_addr **) server_IP_host_address->h_addr_list;
	TCP_aws_socket_addr.sin_family = AF_INET;
	// TCP_aws_socket_addr.sin_addr = **server_IP_address;
	TCP_aws_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	TCP_aws_socket_addr.sin_port = htons(25823);
	bzero(&TCP_aws_socket_addr.sin_zero,8);// &TCP_aws_socket_addr.sin_zero,8);

	// Check for errors
	if ((connect(TCP_descriptor, (struct sockaddr*) &TCP_aws_socket_addr, sizeof(TCP_aws_socket_addr))) < 0)
	{
		perror("Error connecting TCP socket");
		exit(-1);
	}
}

// Send data over TCP connection
void send_TCP_socket(int* buffer, int count)
{	
	// printf ("Number: %d\n",buffer[2]);
	int num_len = count * sizeof(int);
	if (send(TCP_descriptor,buffer,num_len,0) < 0)
	{
		perror("Error sending data");
	}
}

void send_length()
{
	// Find the file size
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
	}
	fclose(file);
	int len = htons(count);
	if (send(TCP_descriptor,(const char*)&len,sizeof(len),0) < 0)
	{
		perror("Error sending the length");
	}
	cout << "The client has sent " << count << " numbers to AWS." << endl;
}

// Send instruction over TCP connection
void send_TCP_socket_instr(char* instr, int count)
{
	// int len = sizeof(instr);
	if (send(TCP_descriptor,instr,4,0) < 0)
	{
		perror("Error sending instruction");
	}
	//printf("Instr sent %s\n",instr);
}

int receive_reduction_answer()
{
	int receive;
	int buffer;
	receive = recv(TCP_descriptor, &buffer, sizeof(buffer), 0);
	if (receive < 0)
	{
		perror("Error receiving the length");
		exit(-1);
	}
	return buffer;
	// return buffer;
}

// Close TCP socket
void close_TCP_socket()
{
	if (close(TCP_descriptor) < 0)
	{
		perror("Error closing TCP socket");
	}
}

int read_file()
{
	// Find the file size
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
	}
	return count;
}
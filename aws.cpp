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
#include "aws.h"
using namespace std;

/* TCP connection with client and UDP connection with back servers */

// Define descriptors
int UDP_descriptor;
int TCP_descriptor;
int TCP_client_descriptor;

// Define UDP sockaddr_in
// UDP here is used to connect aws to back servers
struct sockaddr_in UDP_aws_socket_addr;
struct sockaddr_in UDP_server_A_socket_addr; 
struct sockaddr_in UDP_server_B_socket_addr;
struct sockaddr_in UDP_server_C_socket_addr;

// Define TCP sockaddr_in
// TCP here is used to connect aws to client
struct sockaddr_in TCP_aws_socket_addr;
struct sockaddr_in TCP_client_socket_addr; 

int main (int argc, char* argv[])
{
	cout << "The AWS is up and running." << endl;
	setup_TCP_socket();
	bind_TCP_socket();
	setup_UDP_socket();
	bind_UDP_socket();
	int length;
	int dup_length;
	char* instr;
	int* buf;
	int* dup_buf;
	int ansA;
	int ansB;
	int ansC;
	int answer;
	listen_to_TCP_socket();
	while (1)
	{
		accept_TCP_socket();
		length = receive_length();
		dup_length = length;
		cout << "The AWS has received " << length << " numbers from the client using TCP over port 25823" << endl;
		// cout << length << endl;
		instr = receive_TCP_socket_instr();
		buf = receive_TCP_socket(dup_length);
		dup_buf = buf;
		// cout << buf[0] << endl;

		send_length_A(&dup_length);
		cout << "The AWS sent " << length / 3 << " numbers to Backend-Server A" << endl;
		send_UDP_socket_A(dup_buf,dup_length);
		send_UDP_socket_instr_A(instr);
	
		send_length_B(&dup_length);
		cout << "The AWS sent " << length / 3 << " numbers to Backend-Server B" << endl;
		send_UDP_socket_B(dup_buf,dup_length);
		send_UDP_socket_instr_B(instr);
	
		send_length_C(&dup_length);
		cout << "The AWS sent " << length / 3 << " numbers to Backend-Server C" << endl;
		send_UDP_socket_C(dup_buf,dup_length);
		send_UDP_socket_instr_C(instr);
	
		ansA = receive_UDP_socket_A();
		cout << "The AWS received reduction result of " << instr << " from Bankend-Server A using UDP over port 24823 and it is " << ansA << endl;
		ansB = receive_UDP_socket_B();
		cout << "The AWS received reduction result of " << instr << " from Bankend-Server B using UDP over port 24823 and it is " << ansB << endl;
		ansC = receive_UDP_socket_C();
		cout << "The AWS received reduction result of " << instr << " from Bankend-Server C using UDP over port 24823 and it is " << ansC << endl;
	
		answer = calculation(instr, ansA, ansB, ansC);
		// cout << answer << endl;

		send_reduction_answer(&answer);
	}

	// close_TCP_socket();
}

// OTHER FUNCTION AWS REDUCE FUNCTIOn
int calculation(char* instr, int A, int B, int C) 
{
	string s_instr = instr;
	int min = A;
	int max = A;
	int sum = 0;
	int sos = 0;
	if (s_instr == "MIN")
	{
		if (B > C)
		{
			if (C > A)
				min = A;
			else
				min = C;
		}
		else
		{
			if (B > A)
				min = A;
			else
				min = B;
		}
		return min;
	}
	if (s_instr == "MAX")
	{
		if (B > C)
		{
			if (B < A)
				max = A;
			else
				max = B;
		}
		else
		{
			if (C < A)
				max = A;
			else
				max = C;
		}
		return max;
	}
	if (s_instr == "SUM")
	{
		sum = A + B + C;
		return sum;
	}
	if (s_instr == "SOS")
	{
		sos = A + B + C;
		return sos;
	}
}

// UDP CONNECTION 
// Set up UDP connection
void setup_UDP_socket() 
{
	UDP_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
	if (UDP_descriptor < 0)
	{
		perror("Error creating UDP socket");
		exit(-1);
	}
}

// Bind UDP connection
void bind_UDP_socket()
{
	// struct hostent *nunki_IP_host_address = resolve_host_name("nunki.usc.edu");
	// struct in_addr **nunki_IP_address = (struct in_addr **) nunki_IP_host_address->h_addr_list;
	UDP_aws_socket_addr.sin_family = AF_INET;
	UDP_aws_socket_addr.sin_port = htons(24823);
	UDP_aws_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //**nunki_IP_address;
	bzero(&UDP_aws_socket_addr.sin_zero,8);

	// Check for errors
	if ((bind(UDP_descriptor, (struct sockaddr*) &UDP_aws_socket_addr, sizeof(UDP_aws_socket_addr))) < 0)
	{
		perror("Error binding UDP socket");
		exit(-1);
	}

	// Update socket
	// update_socket(UDP_descriptor, &UDP_aws_socket_addr);
}

/* Send partial number sets to back up servers section */
// Send data over UDP connection
// SERVER A
void send_UDP_socket_A(int* buffer, int length)
{	
	// Connect to server A
	UDP_server_A_socket_addr.sin_family = AF_INET;
	UDP_server_A_socket_addr.sin_port = htons(21823);
	UDP_server_A_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_server_A_socket_addr.sin_zero,8);

	int num_len = length * sizeof(int);
	if (sendto(UDP_descriptor, buffer, num_len, 0, (struct sockaddr*)&UDP_server_A_socket_addr, sizeof(UDP_server_A_socket_addr)) < 0)
	{
		perror("Error UDP data");
	}
}

// Send length over UDP connection
void send_length_A(int* length)
{
	// Connect to server A
	UDP_server_A_socket_addr.sin_family = AF_INET;
	UDP_server_A_socket_addr.sin_port = htons(21823);
	UDP_server_A_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_server_A_socket_addr.sin_zero,8);

	int num_len = sizeof(length);
	if (sendto(UDP_descriptor, length, num_len, 0, (struct sockaddr*)&UDP_server_A_socket_addr, sizeof(UDP_server_A_socket_addr)) < 0)
	{
		perror("Error UDP data");
	}
}

// Send instruction over UDP connection
void send_UDP_socket_instr_A(char* instr)
{
	// Connect to server A
	UDP_server_A_socket_addr.sin_family = AF_INET;
	UDP_server_A_socket_addr.sin_port = htons(21823);
	UDP_server_A_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_server_A_socket_addr.sin_zero,8);

	if (sendto(UDP_descriptor, instr, 4, 0, (struct sockaddr*)&UDP_server_A_socket_addr, sizeof(UDP_server_A_socket_addr)) < 0)
	{
		perror("Error sending instruction");
	}
}

// Receive data back from server 
int receive_UDP_socket_A()
{
	unsigned int serverAlen;
	int buffer;
	if (recvfrom(UDP_descriptor, &buffer, sizeof(buffer), 0, (struct sockaddr*)&UDP_server_A_socket_addr, (socklen_t*)&serverAlen) < 0)
	{
		perror("Error receiving UDP data");
	}
	return buffer;
}

// SERVER B
void send_UDP_socket_B(int* buffer, int length)
{	
	// Connect to server B
	UDP_server_B_socket_addr.sin_family = AF_INET;
	UDP_server_B_socket_addr.sin_port = htons(22823);
	UDP_server_B_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_server_B_socket_addr.sin_zero,8);

	int num_len = length * sizeof(int);
	if (sendto(UDP_descriptor, buffer, num_len, 0, (struct sockaddr*)&UDP_server_B_socket_addr, sizeof(UDP_server_B_socket_addr)) < 0)
	{
		perror("Error UDP data");
	}
}

// Send length over UDP connection
void send_length_B(int* length)
{
	// Connect to server B
	UDP_server_B_socket_addr.sin_family = AF_INET;
	UDP_server_B_socket_addr.sin_port = htons(22823);
	UDP_server_B_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_server_B_socket_addr.sin_zero,8);

	int num_len = sizeof(length);
	if (sendto(UDP_descriptor, length, num_len, 0, (struct sockaddr*)&UDP_server_B_socket_addr, sizeof(UDP_server_B_socket_addr)) < 0)
	{
		perror("Error UDP data");
	}
}

// Send instruction over UDP connection
void send_UDP_socket_instr_B(char* instr)
{
	// Connect to server B
	UDP_server_B_socket_addr.sin_family = AF_INET;
	UDP_server_B_socket_addr.sin_port = htons(22823);
	UDP_server_B_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_server_B_socket_addr.sin_zero,8);

	if (sendto(UDP_descriptor, instr, 4, 0, (struct sockaddr*)&UDP_server_B_socket_addr, sizeof(UDP_server_B_socket_addr)) < 0)
	{
		perror("Error sending instruction");
	}
}

// Receive data back from server 
int receive_UDP_socket_B()
{
	unsigned int serverBlen;
	int buffer;
	if (recvfrom(UDP_descriptor, &buffer, sizeof(buffer), 0, (struct sockaddr*)&UDP_server_B_socket_addr, (socklen_t*)&serverBlen) < 0)
	{
		perror("Error receiving UDP data");
	}
	return buffer;
}

// SERVER C
void send_UDP_socket_C(int* buffer, int length)
{	
	// Connect to server C
	UDP_server_C_socket_addr.sin_family = AF_INET;
	UDP_server_C_socket_addr.sin_port = htons(23823);
	UDP_server_C_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_server_C_socket_addr.sin_zero,8);

	int num_len = length * sizeof(int);
	if (sendto(UDP_descriptor, buffer, num_len, 0, (struct sockaddr*)&UDP_server_C_socket_addr, sizeof(UDP_server_C_socket_addr)) < 0)
	{
		perror("Error UDP data");
	}
}

// Send length over UDP connection
void send_length_C(int* length)
{
	// Connect to server C
	UDP_server_C_socket_addr.sin_family = AF_INET;
	UDP_server_C_socket_addr.sin_port = htons(23823);
	UDP_server_C_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_server_C_socket_addr.sin_zero,8);

	int num_len = sizeof(length);
	if (sendto(UDP_descriptor, length, num_len, 0, (struct sockaddr*)&UDP_server_C_socket_addr, sizeof(UDP_server_C_socket_addr)) < 0)
	{
		perror("Error UDP data");
	}
}

// Send instruction over UDP connection
void send_UDP_socket_instr_C(char* instr)
{
	// Connect to server C
	UDP_server_C_socket_addr.sin_family = AF_INET;
	UDP_server_C_socket_addr.sin_port = htons(23823);
	UDP_server_C_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_server_A_socket_addr.sin_zero,8);

	if (sendto(UDP_descriptor, instr, 4, 0, (struct sockaddr*)&UDP_server_C_socket_addr, sizeof(UDP_server_C_socket_addr)) < 0)
	{
		perror("Error sending instruction");
	}
}

// Receive data back from server 
int receive_UDP_socket_C()
{
	unsigned int serverClen;
	int buffer;
	if (recvfrom(UDP_descriptor, &buffer, sizeof(buffer), 0, (struct sockaddr*)&UDP_server_C_socket_addr, (socklen_t*)&serverClen) < 0)
	{
		perror("Error receiving UDP data");
	}
	return buffer;
}

// Close UDP socket
void close_UDP_socket()
{
	if (close(UDP_descriptor) < 0)
	{
		perror("Error closing UDP socket");
	}
}

// TCP CONNECTION
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

// Bind TCP connection
void bind_TCP_socket()
{
	// struct hostent *nunki_IP_host_address = resolve_host_name("nunki.usc.edu");
	// struct in_addr **nunki_IP_address = (struct in_addr **) nunki_IP_host_address->h_addr_list;
	TCP_aws_socket_addr.sin_family = AF_INET;
	TCP_aws_socket_addr.sin_port = htons(25823);
	// TCP_aws_socket_addr.sin_addr = **nunki_IP_address;
	TCP_aws_socket_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&TCP_aws_socket_addr.sin_zero,8);

	unsigned int len = sizeof(struct sockaddr_in);
	// Check for errors
	if ((bind(TCP_descriptor, (struct sockaddr*) &TCP_aws_socket_addr, len)) < 0)
	{
		perror("Error binding TCP socket");
		exit(-1);
	}
}

// Listen for requests
void listen_to_TCP_socket()
{
	if (listen(TCP_descriptor, 5) < 0)
	{
		perror("Error listening TCP socket");
		exit(-1);
	}
}

// Accept instruction and numbers from client
void accept_TCP_socket() 
{
	unsigned int len = sizeof(struct sockaddr_in);
	TCP_client_descriptor = accept(TCP_descriptor, (struct sockaddr*)&TCP_client_socket_addr, &len);
	if (TCP_client_descriptor < 0)
	{
		perror("Error accepting TCP socket");
		exit(-1);
	}
}

// Receive numbers from client
int* receive_TCP_socket(int count)
{
	int receive;
	int buf[count];
	int len = count * sizeof(int);
	receive = recv(TCP_client_descriptor, buf, len, 0);
	if (receive < 0)
	{
		perror("Error receiving TCP socket");
		exit(-1);
	}
	// printf("Receive %d bytes from client: %s\n", receive, inet_ntoa(TCP_client_socket_addr.sin_addr));
	// cout << buf[0] << endl;
	// cout << buf[298] << endl;
	// cout << buf[299] << endl;
	int* pt = new int [count];
	for (int i = 0; i < count; i++)
	{
		pt[i] = buf[i];
	}
	return pt;
}

// Receive length
int receive_length()
{
	int receive;
	int buffer;
	receive = recv(TCP_client_descriptor, &buffer, sizeof(buffer), 0);
	int len = ntohs(buffer);
	if (receive < 0)
	{
		perror("Error receiving the length");
		exit(-1);
	}
	cout << "The AWS has received " << len << " numbers from the client using TCP over port 25823" << endl;
	return len;
	// return buffer;
}

// Receive instruction from client
char* receive_TCP_socket_instr()
{
	int receive;
	char buffer[4];
 	receive = recv(TCP_client_descriptor, buffer, 4, 0);
 	if (receive < 0)
 	{
 		perror("Error receiving TCP instr");
 		exit(-1);
 	}
 	// cout << buffer << endl;
 	char* pt = new char[4];
 	strcpy(pt,buffer);
 	return pt;
}

void send_reduction_answer(int* answer) 
{
	int len = sizeof(answer);
	if (send(TCP_client_descriptor,answer,len,0) < 0)
	{
		perror("Error sending the length");
	}
}

// Close TCP socket
void close_TCP_socket()
{
	if (close(TCP_descriptor) < 0)
	{
		perror("Error closing TCP socket");
	}
}

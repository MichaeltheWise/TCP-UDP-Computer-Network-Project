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
#include "serverB.h"
using namespace std;

/* TCP connection with client and UDP connection with back servers */

// Define descriptors
int UDP_descriptor;

// Define UDP sockaddr_in
// UDP here is used to connect aws to back servers
struct sockaddr_in UDP_aws_socket_addr;
struct sockaddr_in UDP_server_socket_addr; 

int main ()
{
	setup_UDP_socket();
	bind_UDP_socket();
	cout << "The server B is up and running using UDP on port 22823" << endl;
	int length;
	int* buf;
	char* instr;
	int answer;
	while (1)
	{
		length = receive_length();
		cout << "The server B has received " << length / 3 << " numbers" << endl;
		// cout << length << endl;
		buf = receive_UDP_socket(length);
		// cout << buf[0] << endl;
		// cout << buf[length-1] << endl;
		instr = receive_UDP_socket_instr();
		// cout << instr << endl;
		answer = calculationB(instr, length, buf);
		cout << "The server B has successfully finished the reduction " << instr << ": " << answer << endl;
		// cout << answer << endl;
		send_UDP_socket(&answer);
		cout << "The server B has successfully finished sending the reduction value to AWS server" << endl;
	}	
	close_UDP_socket();
	return 0;
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
	UDP_server_socket_addr.sin_family = AF_INET;
	UDP_server_socket_addr.sin_port = htons(22823);
	UDP_server_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //**nunki_IP_address;
	bzero(&UDP_server_socket_addr.sin_zero,8);

	// Check for errors
	if ((bind(UDP_descriptor, (struct sockaddr*) &UDP_server_socket_addr, sizeof(UDP_server_socket_addr))) < 0)
	{
		perror("Error binding UDP socket");
		exit(-1);
	}

	// Update socket
	// update_socket(UDP_descriptor, &UDP_aws_socket_addr);
}

int calculationB(char* instr, int length, int* data) 
{
	int actual_size = length / 3;
	string s_instr = instr;
	int min = data[actual_size];
	int max = data[actual_size];
	int sum = 0;
	int sos = 0;
	if (s_instr == "MIN")
	{
		for (int i = actual_size; i < (2*actual_size); i++)
		{
			if (data[i] < min)
			{
				min = data[i];
			}
		}
		return min;
	}
	else if (s_instr == "MAX")
	{
		for (int i = actual_size; i < (2*actual_size); i++)
		{
			if (data[i] > max)
			{
				max = data[i];
			}
		}
		return max;
	}
	else if (s_instr == "SUM")
	{
		for (int i = actual_size; i < (2*actual_size); i++)
		{
			sum = sum + data[i];
		}
		return sum;
	}
	else if (s_instr == "SOS")
	{
		for (int i = actual_size; i < (2*actual_size); i++)
		{
			sos = sos + (data[i] * data[i]);
		}
		return sos;
	}
}

int receive_length()
{
	int buffer;
	unsigned int awslen;
	if (recvfrom(UDP_descriptor, &buffer, sizeof(buffer), 0, (struct sockaddr*)&UDP_aws_socket_addr, (socklen_t*)&awslen) < 0)
	{
		perror("Error receiving UDP data length");
	}
	// int len = htons(buffer);
	// cout << buffer << endl;
	return buffer;
}

// Receive data from aws 
int* receive_UDP_socket(int length)
{
	int buf[length];
	int len = length * sizeof(int);
	unsigned int awslen = sizeof(UDP_server_socket_addr);
	if (recvfrom(UDP_descriptor, buf, len, 0, (struct sockaddr*)&UDP_aws_socket_addr, (socklen_t*)&awslen) < 0)
	{
		perror("Error receiving UDP data");
	}
	int* pt = new int [length];
	for (int i = 0; i < length; i++)
	{
		pt[i] = buf[i];
	}
	return pt;
}

// Receive instruction from aws
char* receive_UDP_socket_instr()
{
	char instr[4];
	unsigned int awslen = sizeof(UDP_server_socket_addr);
	if (recvfrom(UDP_descriptor, instr, 4, 0, (struct sockaddr*)&UDP_aws_socket_addr, (socklen_t*)&awslen) < 0)
	{
		perror("Error receiving UDP instruction");
	}
	char* pt = new char[4];
	strcpy(pt,instr);
	return pt;
}

// Send answer to aws
void send_UDP_socket(int* answer) 
{
	UDP_aws_socket_addr.sin_family = AF_INET;
	UDP_aws_socket_addr.sin_port = htons(24823);
	UDP_aws_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&UDP_aws_socket_addr.sin_zero,8);

	int num_len = sizeof(answer);
	if (sendto(UDP_descriptor, answer, num_len, 0, (struct sockaddr*)&UDP_aws_socket_addr, sizeof(UDP_aws_socket_addr)) < 0)
	{
		perror("Error sending answer");
	}
}

// Close UDP socket
void close_UDP_socket()
{
	if (close(UDP_descriptor) < 0)
	{
		perror("Error closing UDP socket");
	}
}
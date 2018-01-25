/*
EE 450 Session 3
Socket Programming
Written by Po-Hsuan (Michael) Lin
*/

#ifndef EE450_SERVERB_HEADER_FILE
#define EE450_SERVERB_HEADER_FILE

// Functions
void setup_UDP_socket();
void bind_UDP_socket();
int calculationB(char* instr, int length, int* data);
int receive_length();
int* receive_UDP_socket(int length);
char* receive_UDP_socket_instr();
void send_UDP_socket(int* answer);
void close_UDP_socket();

#endif
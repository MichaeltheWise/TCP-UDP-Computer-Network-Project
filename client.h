/*
EE 450 Session 3
Socket Programming
Written by Po-Hsuan (Michael) Lin
*/

#ifndef EE450_CLIENT_HEADER_FILE
#define EE450_CLIENT_HEADER_FILE

// Functions
int* read_csv();
void setup_TCP_socket();
void connect_TCP_socket();
void send_TCP_socket(int *pt, int count);
void send_TCP_socket_instr(char* instr, int count);
void send_length();
void close_TCP_socket();
int read_file();
int receive_reduction_answer();

#endif
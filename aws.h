/*
EE 450 Session 3
Socket Programming
Written by Po-Hsuan (Michael) Lin
*/

#ifndef EE450_AWS_HEADER_FILE
#define EE450_AWS_HEADER_FILE

// Functions
void setup_TCP_socket();
void bind_TCP_socket();
void listen_to_TCP_socket();
void accept_TCP_socket();
int* receive_TCP_socket(int length);
char* receive_TCP_socket_instr();
int receive_length();
void close_TCP_socket();

void setup_UDP_socket();
void bind_UDP_socket();

void send_length_A(int* length);
void send_UDP_socket_A(int* buf, int length);
void send_UDP_socket_instr_A(char* instr);
int receive_UDP_socket_A();

void send_length_B(int* length);
void send_UDP_socket_B(int* buf, int length);
void send_UDP_socket_instr_B(char* instr);
int receive_UDP_socket_B();

void send_length_C(int* length);
void send_UDP_socket_C(int* buf, int length);
void send_UDP_socket_instr_C(char* instr);
int receive_UDP_socket_C();

void close_UDP_socket();
int calculation(char* instr, int ansA, int ansB, int ansC);
void send_reduction_answer(int* answer);

#endif
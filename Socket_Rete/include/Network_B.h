// Wireless communicator - Client part

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define sign(X) (X>=0.0)?48:49

#define PORT_B 3491     // the port client will be connecting to
#define MAXDATASIZE_B 100 // max number of bytes we can get at once
using namespace std;

extern const char* hostname_B;
static int sockfd_B;
static int numbytes_B;
extern char buf_B[MAXDATASIZE_B];
static sockaddr_in server_B;
static int len_B;

extern short int cmd_B;
extern bool connected_B;

void* get_in_addr_B(sockaddr* sa);    // get sockaddr IPv4 or IPv6
int initialize_connection_B();
void close_connection_B();

void set_recv_timeout_B();
void receive_data_B();
void send_data_B(char*);


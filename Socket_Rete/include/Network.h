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

#define PORT 3490     // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once
using namespace std;

extern const char* hostname;
static int sockfd;
static int numbytes;
extern char buf[MAXDATASIZE];
static sockaddr_in server;
static int len;

extern short int cmd;
extern bool connected;

void* get_in_addr(sockaddr* sa);    // get sockaddr IPv4 or IPv6
int initialize_connection();
void close_connection();

void set_recv_timeout();
void receive_data();
void send_data(char*);


// Wireless communicator - Client part

#include "Network.h"

int initialize_connection(){
    hostent* host;
    host = gethostbyname(hostname);
    len = sizeof(struct sockaddr_in);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
            perror("client: socket");
            exit(1);
    }

    memset((char *)&server, 0, sizeof(server)); 
    server.sin_family = AF_INET; 
    server.sin_addr = *((struct in_addr*) host->h_addr);
    server.sin_port = htons(PORT); 

    cout << "client: connecting to " << inet_ntoa(server.sin_addr) << endl;

    sockaddr_in myaddr;
    memset((char *)&myaddr, 0, sizeof(myaddr)); 
    myaddr.sin_family = AF_INET; 
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    myaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) { 
    	perror("client: bind failed"); 
    	exit(1);
    }

    return 0;
}

void set_recv_timeout(){
    timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (timeval*)&timeout, sizeof(timeval));
}

void close_connection(){
    cmd = 9;
    sleep(3);		// Time to send disconnection command to server
    connected = false;
    close(sockfd);
}

void receive_data(){
    memset(&buf, 0, sizeof(buf));
    // Receive data
    numbytes = recvfrom(sockfd, buf, 27, MSG_WAITALL, (sockaddr*)&server, (socklen_t*)&len);
    std::cout << "numbytes: " << numbytes << std::endl;
    if (numbytes == -1 || numbytes < 27){
        // perror("recv");
        numbytes = recvfrom(sockfd, buf, 27, MSG_WAITALL, (sockaddr*)&server, (socklen_t*)&len);
	    // exit(1);
    }
    buf[numbytes] = '\0';
}

void send_data(char* var){
    int ret = sendto(sockfd, (void*)var, 5, MSG_WAITALL, (sockaddr*)&server, len);
    if (ret == -1 || ret < 5){
		perror("send");
		exit(1);
	}
}


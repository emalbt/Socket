// Wireless communicator - Client part

#include "Network_B.h"

int initialize_connection_B(){
    hostent* host_B;
    host_B = gethostbyname(hostname_B);
    len_B = sizeof(struct sockaddr_in);

    if ((sockfd_B = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
            perror("client: socket");
            exit(1);
    }

    memset((char *)&server_B, 0, sizeof(server_B)); 
    server_B.sin_family = AF_INET; 
    server_B.sin_addr = *((struct in_addr*) host_B->h_addr);
    server_B.sin_port = htons(PORT_B); 

    cout << "client: connecting to " << inet_ntoa(server_B.sin_addr) << endl;

    sockaddr_in myaddr;
    memset((char *)&myaddr, 0, sizeof(myaddr)); 
    myaddr.sin_family = AF_INET; 
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    myaddr.sin_port = htons(PORT_B);

    if (bind(sockfd_B, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) { 
    	perror("client: bind failed"); 
    	exit(1);
    }

    return 0;
}

void set_recv_timeout_B(){
    timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;
    setsockopt(sockfd_B, SOL_SOCKET, SO_RCVTIMEO, (timeval*)&timeout, sizeof(timeval));
}

void close_connection_B(){
    cmd_B = 9;
    sleep(3);		// Time to send disconnection command to server_B
    connected_B = false;
    close(sockfd_B);
}

void receive_data_B(){
    memset(&buf_B, 0, sizeof(buf_B));
    // Receive data
    numbytes_B = recvfrom(sockfd_B, buf_B, 27, MSG_WAITALL, (sockaddr*)&server_B, (socklen_t*)&len_B);
    std::cout << "numbytes: " << numbytes_B << std::endl;
    if (numbytes_B == -1 || numbytes_B < 27){
        // perror("recv");
        numbytes_B = recvfrom(sockfd_B, buf_B, 27, MSG_WAITALL, (sockaddr*)&server_B, (socklen_t*)&len_B);
	    // exit(1);
    }
    buf_B[numbytes_B] = '\0';
}

void send_data_B(char* var){
    int ret = sendto(sockfd_B, (void*)var, 5, MSG_WAITALL, (sockaddr*)&server_B, len_B);
    if (ret == -1 || ret < 5){
		perror("send");
		exit(1);
	}
}


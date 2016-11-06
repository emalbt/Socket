// Wireless communicator - Client part

#include "wireless_communicator.h"

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                        initialize_connection
//------------------------------------------------------------------------------------------------------------------------------------------
int initialize_connection(char* clientname){
    struct sockaddr_in myaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            perror("server: socket");
            exit(1);
    }

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    //myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(PORT);

    memset((char *)&other, 0, sizeof(other));
    other.sin_family = AF_INET;
    inet_aton(clientname, &other.sin_addr);
    other.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
    	perror("server: bind");
    	exit(1);
    }

    cout << "Ready to accept connections on port " << PORT << endl;

    connected = true;

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                              close_connection
//------------------------------------------------------------------------------------------------------------------------------------------
void close_connection(){
    connected = false;
    connected_main = false;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                                    close_network
//------------------------------------------------------------------------------------------------------------------------------------------
void close_network(){
    cout << "Closing connections ...";
    close_connection();
    close(sockfd);
    cout << "Done" << endl;
}


//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                                       receive_data
//------------------------------------------------------------------------------------------------------------------------------------------
void receive_data(){
   memset(&buf, 0, sizeof(buf));
    numbytes = recvfrom(sockfd, buf, 5, MSG_WAITALL, (sockaddr*) &other, (socklen_t*)&len);
    if (numbytes == -1 || numbytes < 5) {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';
}


//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                             receive_wifi_event
//------------------------------------------------------------------------------------------------------------------------------------------
bool receive_wifi_event(){
   memset(&buf, 0, sizeof(buf));
    numbytes = recvfrom(sockfd, buf, 1, MSG_DONTWAIT, (sockaddr*) &other, (socklen_t*)&len);
    if (numbytes == -1 || numbytes < 1) 
        return 0;
    else
    	return 1;
    //buf[numbytes] = '\0';
}


//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                                           send_data
//------------------------------------------------------------------------------------------------------------------------------------------
void send_data(char* var){
    int ret = sendto(sockfd, (void*)var, 27, MSG_WAITALL, (sockaddr*)&other, len);
    //std::cout << "ret: " << ret << std::endl;
    if (ret == -1 || ret < 27) {
        perror("send");
        exit(1);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                            create_packet_imu
//------------------------------------------------------------------------------------------------------------------------------------------
void create_packet_imu(char* str, double* val){
	char app[MAXDATASIZE] = ":";
	char s[MAXDATASIZE] = "";
	for (int i=0; i<6; i++){
		sprintf(s, "%03d", (short int)val[i]);
		strcat(app, s);
		strcat(app, ":");
	}
	strcat(app, "!");
	strcpy(str, app);
}


//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                        	       create_packet
//------------------------------------------------------------------------------------------------------------------------------------------
void create_packet(char* str, double* val){
	char app[MAXDATASIZE] = "?";
	char s[MAXDATASIZE] = "";
	for (int i=0; i<4; i++){
		sprintf(s, "%c'%03d", (sign(val[i])), abs((short int)val[i]));
		strcat(app, s);
		if (i < 3) strcat(app, ":");
	}
	strcat(app, ".\n");
	strcpy(str, app);
}

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                                        	              wifi_event
//------------------------------------------------------------------------------------------------------------------------------------------
bool wifi_event()
{
	if (listen(sockfd, 5) == -1)
	{
		std::cout << "ZERO\n";
		return 0;
	}	
	else
	{	
		std::cout << "UNO\n";	
		return 1;
	}
}



// Wireless communicator - Client part

#include "wireless_communicator.h"

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

void close_connection(){
    connected = false;
    connected_main = false;
}

void close_network(){
    cout << "Closing connections ...";
    close_connection();
    close(sockfd);
    cout << "Done" << endl;
}

void receive_data(){
   memset(&buf, 0, sizeof(buf));
    numbytes = recvfrom(sockfd, buf, 5, MSG_WAITALL, (sockaddr*) &other, (socklen_t*)&len);
    if (numbytes == -1 || numbytes < 5) {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';
}

void send_data(char* var){
    int ret = sendto(sockfd, (void*)var, 27, MSG_WAITALL, (sockaddr*)&other, len);
    //std::cout << "ret: " << ret << std::endl;
    if (ret == -1 || ret < 27) {
        perror("send");
        exit(1);
    }
}

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

void send_image(char* var, unsigned int mex_len){
    int ret;
    char s[10];
    sprintf(s, "%06d", mex_len);
    ret = sendto(sockfd, s, 6, 0, (sockaddr*)&other, len);
    if (ret == -1 || ret < 6) {
        perror("send");
        exit(1);
    }

    ret = sendto(sockfd, (void*)var, mex_len, 0, (sockaddr*)&other, len);
    if (ret == -1 || ret < (int)mex_len) {
        perror("send");
        exit(1);
    }
}

void send_info(char* var, char* oth){
    int ret;
    char s[10];
    unsigned int mex_len = strlen(var);
    sprintf(s, "%05d", mex_len);
    ret = sendto(sockfd, s, 5, 0, (sockaddr*)&other, len);
    if (ret == -1 || ret < 5) {
        perror("send");
        exit(1);
    }

    ret = sendto(sockfd, (void*)var, mex_len, 0, (sockaddr*)&other, len);
    if (ret == -1 || ret < (int)mex_len) {
        perror("send");
        exit(1);
    }

    ret = sendto(sockfd, (void*)oth, 20, 0, (sockaddr*)&other, len);
    if (ret == -1 || ret < 20) {
        perror("send");
        exit(1);
    }
}

bool parse_request(){
   /* // Check if the packet is valid
    if (buf[0] != '?')
        return false;
    char btn;
    char signs[7] = {0,0,0,0,0,0,0};
    short int args[7] = {0,0,0,0,0,0,0};
	char s0, s1;
    int ret = sscanf(buf, "?%c,", &signs[0]);
    if (ret != 1)
        return false;
    cmd = (short int)(signs[0]-48);

    char* pbuf = &buf[3];
    ret = sscanf(pbuf, "%c'%05hd:%c'%05hd:%c'%05hd:%c,%c'%05hd:%c'%05hd:%c'%05hd:%c'%05hd.", \
		 &s0, &args[0], &s1, &args[1], &signs[2], &args[2],  \
		 &btn, &signs[3], &args[3], &signs[4], &args[4], &signs[5], &args[5], &signs[6], &args[6]);
    signs[0] = s0;
    signs[1] = s1;

    if (ret != 15)
	return false;
    else {
	for (int i=0; i<7; i++){
		if (signs[i] == '1')
			args[i] = -args[i];
		else if (signs[i] != '0')
			return false;
	}
    }

    accX = args[0];
    accY = args[1];
    accZ = args[2];
    button = (short int)(btn-48);
    posX = args[3];
    posY = args[4];
    posZ = args[5];

    robot_stiffness = (double)(args[6]) /10000.0;

    // Parse command
    switch (cmd){
        case 0:
            return true;
        case 1:
            increase_kvib = -1;
            return true;
        case 2:
            increase_kvib = 1;
            return true;
        case 3:
            dft321 = true;
            return true;
        case 4:
            dft321 = false;
            return true;
        case 5:
            stop_flag = true;
            send_info_flag = true;
            return true;
        case 6:
            stop_flag = false;
            return true;
        case 7:
            send_img = 1;
            return true;
        case 8:
            send_img = 0;
            return true;
        case 9:
            close_network();
            return true;
        case 10:
            change_motor_ref = true;
            return true;
        default:
            return false;
    }*/
}

bool receive_packet(){
    receive_data();
    return parse_request();
}


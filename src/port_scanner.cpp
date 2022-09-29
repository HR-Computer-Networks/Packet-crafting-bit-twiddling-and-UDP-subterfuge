#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

// Setup socket address structure for connection struct
// has members sin_family, sin_port, sin_addr...
struct sockaddr_in serv_addr;
char buffer[1024];

bool port_is_open(int portno) {
    // Sets target port number to big-endian storage
	serv_addr.sin_port = htons( portno );

    // Create the UDP socket
    int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Send a message via UDP to watch for a response
    for (int i=0; i<3; i++) {
	    int res = sendto(sock_fd, "test", sizeof("test")-1, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    }

    int recVal = 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sock_fd, &rfds);

    // Delay
    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = 1.0;

    recVal = select(sock_fd + 1, &rfds, NULL, NULL, &tv);
    if (recVal == 0) {
        return false;
    }
    else if (recVal == -1) {
        return false;
    }
    else {
        return true;
    }

}

int main(int argc, char **argv) {
    // Read the parameters passed from terminal
	if(argc < 4) { 
		printf("Usage: server <serverip> <low_port> <high_port>\n"); 
		exit(1);
	}

    // Set block of memory
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET ;

    // Assigns the IP and the ports passed as parameters from the terminal
	char *server_ip = argv[1];
	int low_portno = atoi(argv[2]);
    int high_portno = atoi(argv[3]);

    // Need to know the IP address of the server we are connecting to,
	// stores the IP address in binary form in serv_addr.sin_addr
	if( inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
		perror(" failed to set socket address");
		exit(0);
	}

    // Checks which ports are open inside the selected range
    for (int i=low_portno; i<=high_portno; i++) {
        printf("Port %d is %d \n", i, port_is_open(i));
    }
}

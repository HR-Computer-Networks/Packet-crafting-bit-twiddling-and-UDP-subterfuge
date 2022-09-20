// string& passes by reference to a function: the actual memory store
// string passes by value to a function

// int* n1 creates a pointer variable (will store the address of another var)
// int n2 creates a regular variable
// n1 = &n2 passes the memory store / address of n2 to n1
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

bool port_is_open(int portno) {

    // sets target port number to big-endian storage
	serv_addr.sin_port = htons( portno );

    int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP

	int res = sendto(sock_fd, "test", sizeof("test")-1, 0, (sockaddr*)&serv_addr, sizeof(serv_addr));


    int recVal = 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sock_fd, &rfds);

    // We wait 0.001s
    struct timeval tv;
    tv.tv_usec = 300000;
    tv.tv_sec = 0;

    recVal = select(sock_fd + 1, &rfds, NULL, NULL, &tv);
    if (recVal == 0) {
        return false;
    }
    else {
        cout << portno << endl;
        return true;
    }
}

int main(int argc, char **argv) {
	// should be given 2 arguments exactly: IP address, port
	// all other arguments ignored
	if(argc<4) { 
		printf("usage: server <serverip> <low_port> <high_port>\n"); 
		exit(1);
	}

    int sock = 0;

    // set block of memory
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET ;

    // only works with ip address, not with hostname
	char *server_ip = argv[1];
	int low_portno = atoi(argv[2]);
    int high_portno = atoi(argv[3]);

    // Need to know the IP address of the server we are connecting t
	// stores the IP address in binary form in serv_addr.sin_addr
	if( inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
		perror(" failed to set socket address");
		exit(0);
	}

    cout << "Open ports:" << endl;
    for (int i=low_portno; i<=high_portno; i++) {
        port_is_open(i);
    }
}


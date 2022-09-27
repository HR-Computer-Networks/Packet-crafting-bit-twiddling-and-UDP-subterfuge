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

string get_udp_response(int portno, char* msg) {

    char buffer[2000];

    // sets target port number to big-endian storage
	serv_addr.sin_port = htons( portno );

    // create a socket data structure
    int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP

    // send a message via UDP to watch for a response
	int res = sendto(sock_fd, msg, sizeof(msg)-1, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("sending to port %i: %s\n", portno, msg);
    socklen_t length = sizeof(serv_addr);
    // receive from
    // int nread = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_addr, &length);
    // printf("from %s,%d: %s\n", inet_ntoa(serv_addr.sin_addr), serv_addr.sin_port, buffer);

    int recVal = 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sock_fd, &rfds);

    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = 10.0;

    recVal = select(sock_fd + 1, &rfds, NULL, NULL, &tv);
    if (recVal == 0) {
        cout << "Timeout" << endl;
    }
    else if (recVal == -1) {
        cout << "Error" << endl;
    }
    else {
        if(FD_ISSET(sock_fd, &rfds)){ 
            int n = read(sock_fd, buffer, sizeof(buffer)-1);
        }//close else if statement
    }

    return buffer;
}

int main(int argc, char **argv) {
	// should be given 2 arguments exactly: IP address, port
	// all other arguments ignored
	if(argc<4) { 
		printf("usage: server <serverip> <port_1> <port_2> <port_3> <port_4>\n"); 
		exit(1);
	}
    int ports[] = { atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]) };
    // only works with ip address, not with hostname
	char *server_ip = argv[1];

    // set block of memory
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET ;


    // Need to know the IP address of the server we are connecting t
	// stores the IP address in binary form in serv_addr.sin_addr
	if( inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
		perror(" failed to set socket address");
		exit(0);
	}

    printf("Relevant ports: %i, %i, %i, %i \n", ports[0], ports[1], ports[2], ports[3]);
    char probe_msg[] = "test";
    // get_udp_response(ports[0], probe_msg);

    char group_msg[] = "$group_50$";
    cout << get_udp_response(ports[0], group_msg);

    cout << get_udp_response(ports[1], probe_msg) << endl;
    cout << get_udp_response(ports[2], probe_msg) << endl;
    cout << get_udp_response(ports[3], probe_msg) << endl;

}

#include<iostream>
#include<netdb.h>
#include<sys/types.h>

using namespace std;


string get_udp_response(...) {

}

string send_udp_message(...) {

}

// checksum and wrapsum are available online
// its fine to use those
// github.com link!!
// https://github.com/openbsd/src/blob/master/sbin/dhclient/packet.c
uint32_t checksum(...) {

}

uint32_t wrapsum(...) {

}

// solving the evil bit puzzle

int solve_evil_port(int sockfd, struct hostent* host, int port) {
    // make a UDP header and an IP with right checksums

    char datagram[4096];
    memset(datagram, 0, sizeof(datagram));
    int datagram_length;
    struct ip* ip = (struct ip*) datagram;

    ip-> ip_v 
    ip-> ip_hl
    ip-> ip_tos
    ip -> ip_len
    ip -> ip_id
    ip -> ip_off
    ip->ip_ttl
    ip->ip_p
    ip->ip_sum

    // we'll get our own IP address and port by looking at
    // the src address of the socket that we used to
    // talk to the server

    struct sockaddr_in own_addr;
    socklen_t own_addr_len = sizeof(own_addr);

    // use getsockname function

    ip->ip_src = own_addr.sin_addr;
    ip->ip_dst.s_addr = (uint32_t*)host->h_addr;
}

int find_right_port(...) {

}

// solving the checksum port puzzle
string solve_checksum_port() {

}

// talk to the oracle
vector<int> solve_oracle() {
    ...
}

// knocking
string knock() {
    ...
}


int main(int argc, char* argv[]) {

    // Setup socket address structure for connection struct
    // has members sin_family, sin_port, sin_addr...
    struct sockaddr_in serv_addr;

    const char* hostname;
    int ports[4];

    struct hostent* host = NULL;
    int sockfd;
    struct timeval tv;

    // should be given 2 arguments exactly: IP address, port
	// all other arguments ignored
	if(argc<4) { 
		printf("usage: server <serverip> <port_1> <port_2> <port_3> <port_4>\n"); 
		exit(1);
	}

    int sock = 0;

    // set block of memory
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET ;

    // only works with ip address, not with hostname
	char *server_ip = argv[1];
	int ports[] = { atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]) };

    // set timeout for socket operations
    // use the timeval tv to set values you desire

    // solve puzzleports

    // talk to oracle
    // call solve_oracle, need to write

    // knock on hidden ports 
    // call a knock function, need to write
}

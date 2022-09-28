#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <netinet/ip.h>

using namespace std;

// Setup socket address structure for connection struct
// has members sin_family, sin_port, sin_addr...
struct sockaddr_in serv_addr;
char buffer[2048];

//send a udp message where the payload is a valid UDP IPv4 packet,
//that has a valid UDP checksum of 0x4171, and with the source address being 5.105.90.126!


void get_udp_response(int portno, char* msg) {

    memset(buffer, 0, 2048);
    fflush(stdout);
    int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP


    // sets target port number to big-endian storage
	serv_addr.sin_port = htons( portno );

    // create a socket data structure

    // send a message via UDP to watch for a response
	int res = sendto(sock_fd, msg, strlen(msg), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
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
            cout << buffer << endl;
        }
    }

    close(sock_fd);

}



// retrieved from
// https://github.com/openbsd/src/blob/master/sbin/dhclient/packet.c
uint32_t
checksum(unsigned char *buf, uint32_t nbytes, uint32_t sum)
{
	unsigned int	 i;

	/* Checksum all the pairs of bytes first. */
	for (i = 0; i < (nbytes & ~1U); i += 2) {
		sum += (uint16_t)ntohs(*((uint16_t *)(buf + i)));
		if (sum > 0xFFFF)
			sum -= 0xFFFF;
	}

	/*
	 * If there's a single byte left over, checksum it, too.
	 * Network byte order is big-endian, so the remaining byte is
	 * the high byte.
	 */
	if (i < nbytes) {
		sum += buf[i] << 8;
		if (sum > 0xFFFF)
			sum -= 0xFFFF;
	}

	return sum;
}

uint32_t
wrapsum(uint32_t sum)
{
	sum = ~sum & 0xFFFF;
	return htons(sum);
}

int solve_group_msg(int sockfd, unsigned short checksum, in_addr* s_addr) {
    
    char *data = "Hello";

    char datagram[4096];
    memset(datagram, 0, sizeof(datagram));
    struct ip *ip = (struct ip*) datagram;
    struct udp *udp = (struct udp*) (datagram + sizeof(struct ip));

    ip->ip_hl = 5;
    ip->ip_v = 4;

    ip->ip_tos = 0;
    ip->ip_len = ;
    ip->ip_id = ;
    ip->ip_off = 0;
    ip->ip_ttl = 255;
    ip->ip_p = IPPROTO_UDP;
    ip->ip_sum = checksum;
    ip->ip_src = *s_addr;
    ip->ip_dst = serv_addr.sin_addr;

    sendto(sockfd, datagram, (sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data)), ROUTER_IP);

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

    // The first port wants me to send a message with "group_50"
    char group_msg[] = "$group_50$";
    get_udp_response(ports[0], group_msg);

    // The last 6 bytes contain the relevant information in byte order
    // get the last 6 bytes
    char * pch;
    pch=strrchr(buffer,')');
    // we want to start from the next character
    pch = pch + 1;

    unsigned short* given_checksum = new unsigned short;
    in_addr* given_address = new in_addr;

    memcpy(given_checksum, pch, sizeof(unsigned short));
    pch = pch + 2;
    memcpy(given_address, pch, sizeof(in_addr));



    // [][][][][][][][]
    // ^        ^ udph (udphdr*)
    // dg (char*)
    // ^ iph (iphdr*)

}
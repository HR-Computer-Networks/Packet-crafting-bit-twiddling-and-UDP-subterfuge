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
#include <netinet/udp.h>

using namespace std;

// Setup socket address structure for connection struct
// has members sin_family, sin_port, sin_addr...
struct sockaddr_in serv_addr;
char buffer[2048];
const char ip[] = "20.213.80.229";

//send a udp message where the payload is a valid UDP IPv4 packet,
//that has a valid UDP checksum of 0x4171, and with the source address being 5.105.90.126!


void get_udp_response(int sock_fd, int portno, char* msg) {

    memset(buffer, 0, 2048);
    fflush(stdout);

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

void solve_group_msg(int sockfd, int portno, unsigned short* checksum, in_addr* s_addr) {
    
    char datagram[4096] , *data;

    memset(datagram, 0, sizeof(datagram));
    struct ip *iph = (struct ip*) datagram;
    struct udphdr *udp = (struct udphdr*) (datagram + sizeof(struct ip));

    data = datagram + sizeof(struct ip) + sizeof(struct udphdr);
	strcpy(data , "Hello");

    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    // cout << sizeof(struct ip)
    iph->ip_len = sizeof(struct ip) + sizeof(struct udphdr) + strlen(data);
    iph->ip_id = htonl (54321);
    iph->ip_off = 0;
    iph->ip_ttl = 255;
    iph->ip_p = IPPROTO_UDP;
    iph->ip_sum = *checksum;
    iph->ip_src = *s_addr;
    iph->ip_dst = serv_addr.sin_addr;

    udp->uh_sport = 6666;
    udp->uh_dport = portno);
    udp->uh_ulen = 8 + strlen(data);
    udp->uh_sum = *checksum;
    

    sendto(sockfd, datagram, sizeof(struct ip*) + sizeof(struct udphdr*) + strlen(data), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    memset(buffer, 0, 2048);
    fflush(stdout);

    int recVal = 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);

    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = 10.0;

    recVal = select(sockfd + 1, &rfds, NULL, NULL, &tv);
    if (recVal == 0) {
        cout << "Timeout" << endl;
    }
    else if (recVal == -1) {
        cout << "Error" << endl;
    }
    else {
        if(FD_ISSET(sockfd, &rfds)){ 
            int n = read(sockfd, buffer, sizeof(buffer)-1);
            cout << buffer << endl;
        }
    }

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

    int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP

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
    get_udp_response(sock_fd, ports[0], group_msg);

    // The last 6 bytes contain the relevant information in byte order
    // get the last 6 bytes
    char * pch;
    pch=strrchr(buffer,')');
    // we want to start from the next character
    pch = pch + 1;

    unsigned short* given_checksum = new unsigned short;
    struct in_addr* given_address = new in_addr;

    memcpy(given_checksum, pch, sizeof(unsigned short));
    pch = pch + 2;
    memcpy(given_address, pch, sizeof(in_addr));


    // [][][][][][][][]
    // ^        ^ udph (udphdr*)
    // dg (char*)
    // ^ iph (iphdr*)

    solve_group_msg(sock_fd, ports[0], given_checksum, given_address);
}
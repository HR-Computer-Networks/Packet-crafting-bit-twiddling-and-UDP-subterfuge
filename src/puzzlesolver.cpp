// boss port: 4071
// oracle: 4021
// hidden1: 4028
// hidden2: 4014
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

struct pseudo_header
{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t udp_length;
};

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
    tv.tv_sec = 5.0;

    recVal = select(sock_fd + 1, &rfds, NULL, NULL, &tv);
    if (recVal == 0) {
        cout << "Timeout" << endl;
    }
    else if (recVal == -1) {
        cout << "Error" << endl;
        exit(0);
    }
    else {
        if(FD_ISSET(sock_fd, &rfds)){ 
            int n = read(sock_fd, buffer, sizeof(buffer)-1);
            cout << buffer << endl;
        }
    }

}

unsigned short checksum2(const char *buf, unsigned size)
{
	unsigned long long sum = 0;
	const unsigned long long *b = (unsigned long long *) buf;

	unsigned t1, t2;
	unsigned short t3, t4;

	/* Main loop - 8 bytes at a time */
	while (size >= sizeof(unsigned long long))
	{
		unsigned long long s = *b++;
		sum += s;
		if (sum < s) sum++;
		size -= 8;
	}

	/* Handle tail less than 8-bytes long */
	buf = (const char *) b;
	if (size & 4)
	{
		unsigned s = *(unsigned *)buf;
		sum += s;
		if (sum < s) sum++;
		buf += 4;
	}

	if (size & 2)
	{
		unsigned short s = *(unsigned short *) buf;
		sum += s;
		if (sum < s) sum++;
		buf += 2;
	}

	if (size)
	{
		unsigned char s = *(unsigned char *) buf;
		sum += s;
		if (sum < s) sum++;
	}

	/* Fold down to 16 bits */
	t1 = sum;
	t2 = sum >> 32;
	t1 += t2;
	if (t1 < t2) t1++;
	t3 = t1;
	t4 = t1 >> 16;
	t3 += t4;
	if (t3 < t4) t3++;

	return ~t3;
}

uint16_t udp_checksum(const void *buffer, size_t length, in_addr_t src_addr, in_addr_t dest_addr)
{
    const uint16_t *buf = (const uint16_t *) buffer; /* treat input as bunch of uint16_t's */
    uint16_t *src_ip = (uint16_t *) &src_addr; 
    uint16_t *dest_ip = (uint16_t *)&dest_addr;
    uint32_t sum;
    size_t len = length;

    sum = 0; 

    /* fold the carry bits for the buffer */
    while (length > 1) {
        sum += *buf++;
        if (sum & 0x80000000)
            sum = (sum & 0xFFFF) + (sum >> 16); /* fold  carries */
        length -= 2;
    }

    if(length & 1)
        sum += *((uint8_t *)buf); // add the padding if packet length is odd */

    /* inject checksum of the pseudo-header */
    sum += *(src_ip++);
    sum += *(src_ip);

    sum += *(dest_ip++);
    sum += *(dest_ip);

    sum += htons(IPPROTO_UDP); /* protocol info */
    sum += htons(len); /* original length! */

    /* fold any carry bits created by adding header sums */
    while(sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return (uint16_t)(~sum);
}

// generates random string of length len
// used for finding UDP checksum
string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}


void solve_group_msg(int sockfd, int portno, unsigned short* checksum, in_addr* s_addr) {
    
    char datagram[4096] , *data, *pseudogram;

    memset(datagram, 0, sizeof(datagram));
    struct ip *iph = (struct ip*) datagram;
    struct udphdr *udp = (struct udphdr*) (datagram + sizeof(struct ip));

    data = datagram + sizeof(struct ip) + sizeof(struct udphdr);
	strcpy(data , "Hello");

    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    iph->ip_len = htons(sizeof(struct ip) + sizeof(struct udphdr) + strlen(data));
    iph->ip_id = 54321;
    iph->ip_off = 0;
    iph->ip_ttl = 255;
    iph->ip_p = IPPROTO_UDP;

    // calculate the IP header checksum
    iph->ip_sum = 0;
    // iph->ip_sum = checksum2(datagram, sizeof (struct ip));
    iph->ip_src = *s_addr;
    iph->ip_dst = serv_addr.sin_addr;

    udp->uh_sport = htons(6666);
    udp->uh_dport = htons(portno);
    udp->uh_ulen = htons(sizeof(struct udphdr) + strlen(data));

    // we need to create data such that the checksums will match
    udp->uh_sum = *checksum;

    // --------------------------------
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // We were not able to get finding the right checksum to work
    // so we simply send the given checksum (incorrect)
    // It will say checksums did not match
    // ---------------------------------


    // using the pseudo header
    // struct pseudo_header psh;

	// psh.source_address = s_addr->s_addr;
	// psh.dest_address = serv_addr.sin_addr.s_addr;
	// psh.placeholder = 0;
	// psh.protocol = IPPROTO_UDP;
	// psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );


    // int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
	// memset(pseudogram, 0, psize);
	
	// memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
	// memcpy(pseudogram + sizeof(struct pseudo_header), udp, sizeof(struct udphdr) + strlen(data));

    // while (udp_sum != *checksum) {
    //     data = datagram + sizeof(struct ip) + sizeof(struct udphdr);
	//     strcpy(data , gen_random(8).c_str());

    //     psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );


    //     int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
    //     memset(pseudogram, 0, psize);
        
    //     memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
    //     memcpy(pseudogram + sizeof(struct pseudo_header), udp, sizeof(struct udphdr) + strlen(data));

    //     udp_sum = checksum2(pseudogram, psize);
    // }

    sendto(sockfd, datagram, sizeof(struct ip) + sizeof(struct udphdr) + strlen(data), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
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

void solve_evil_bit(int sockfd, int portno) {

    //Create a raw socket
	int s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
	
	if(s == -1) {
	    perror("Failed to create socket");
		exit(1);
	}

    // data will be the payload
    // pseudogram for UDP checksum calculation
    char datagram[4096] , *data, *pseudogram;

    memset(datagram, 0, sizeof(datagram));

    // ip header pointer
    struct ip *iph = (struct ip*) datagram;

    //udp header comes after ip header
    struct udphdr *udp = (struct udphdr*) (datagram + sizeof(struct ip));

    data = datagram + sizeof(struct ip) + sizeof(struct udphdr);
	strcpy(data , "$group_50$");

    // get own ip address and port into own_addr
    struct sockaddr_in own_addr;
    socklen_t own_addr_len = sizeof(own_addr);
    if (getsockname(sockfd, (struct sockaddr *) &own_addr, &own_addr_len) != 0) {
        printf("Couldn't get own IP and port");
        exit(0);
    }

    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    iph->ip_len = sizeof(struct ip) + sizeof(struct udphdr) + strlen(data);
    iph->ip_id = 54321;

    // this should be changed to evil bit
    // the first bit should be set to 1 for this
    iph->ip_off = 1000000000000000;
    iph->ip_ttl = 255;
    iph->ip_p = IPPROTO_UDP;

    // we need to calculate the checksum
    iph->ip_sum = 0;

    iph->ip_src = own_addr.sin_addr;
    iph->ip_dst = serv_addr.sin_addr;

    iph->ip_sum = checksum2 (datagram, sizeof (struct ip));

    udp->uh_sport = own_addr.sin_port;
    udp->uh_dport = htons(portno);
    udp->uh_ulen = htons(sizeof(struct udphdr) + strlen(data));

    // we need to calculate the checksum using the pseudogram after this
    udp->uh_sum = 0;
    
    // using the pseudo header
    struct pseudo_header psh;

	psh.source_address = own_addr.sin_addr.s_addr;
	psh.dest_address = serv_addr.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );


    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
	pseudogram = (char *) malloc(psize);
	
	memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
	memcpy(pseudogram + sizeof(struct pseudo_header), udp, sizeof(struct udphdr) + strlen(data));


    // calculate checksum using pseudo header
    udp->uh_sum = htons(udp_checksum(udp, sizeof(struct udphdr), own_addr.sin_addr.s_addr, serv_addr.sin_addr.s_addr));
	
    // for MacOS from Piazza
    int optVal = 1;
    int status = setsockopt(s, IPPROTO_IP, IP_HDRINCL, &optVal, sizeof(optVal));
    if (status != 0) {
        perror("Can't set IP_HDRINCL option on a socket");
    }

    // set the port we want to send tp
    serv_addr.sin_port = htons(portno);

    if ( sendto(s, datagram, sizeof(struct ip) + sizeof(struct udphdr) + strlen(data), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0 ) {
		perror("Evil sendto failed");
	}
    else {
        printf("Evil packet sent\n");
    }

    memset(buffer, 0, 2048);
    fflush(stdout);

    int recVal = 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);

    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = 10.0;

    recVal = select(s+1, &rfds, NULL, NULL, &tv);
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

	// all other arguments ignored
	if(argc < 5) { 
		printf("usage: <serverip> <port_1> <port_2> <port_3> <port_4>\n"); 
        printf("alternate usage: <IP address> <oracleport> <hiddenport1> <hiddenport2> <secretphrase>");
		exit(1);
	}

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

    // check if last argument is integer
    char *endptr;
    const long int argument = strtol (argv[5], &endptr, 10);

    // Secret phrase was passed
    if (endptr[0] != '\0') {
        int oracleport = atoi(argv[2]);
        int hidden1 = atoi(argv[3]);
        int hidden2 = atoi(argv[4]);

        char* secret_phrase = argv[5];
        // cout << secret_phrase << endl;

        string oracle_msg = to_string(hidden1) + ", " + to_string(hidden2);
        char *c_msg = new char[oracle_msg.length()+1];
        strcpy(c_msg, oracle_msg.c_str());
        get_udp_response(sock_fd, oracleport, c_msg);

        get_udp_response(sock_fd, 4014, secret_phrase);
        get_udp_response(sock_fd, 4028, secret_phrase);
        get_udp_response(sock_fd, 4028, secret_phrase);
        get_udp_response(sock_fd, 4028, secret_phrase);
        get_udp_response(sock_fd, 4014, secret_phrase);
    }

    // 4 puzzle ports passed
    else {
        int ports[] = { atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]) };

        // ports[0] should be the boss problem
        // ports[1] the evil bit problem
        // ports[2] the checksum problem
        // ports[3] the oracle
        printf("Relevant ports: %i, %i, %i, %i \n", ports[0], ports[1], ports[2], ports[3]);
        char probe_msg[] = "test";

        // The first passed port contains the hidden port with message about the employee and the boss
        get_udp_response(sock_fd, ports[0], probe_msg);
        char hiddenport1[4];
        memset(hiddenport1, '\0', 4);

        char* p_port1;
        p_port1 = strrchr(buffer, 's');
        p_port1 = p_port1 + 2;

        memcpy(hiddenport1, p_port1, 4);
        printf("Hidden port number one is: %s \n", hiddenport1);


        // second passed port is the evil bit problem
        printf("\nSolving evil bit port... \n");
        get_udp_response(sock_fd, ports[1], probe_msg);
        solve_evil_bit(sock_fd, ports[1]);


        // The third passed port is the checksum problem

        printf("\nSolving checksum problem port... \n");
        char group_msg[] = "$group_50$";
        get_udp_response(sock_fd, ports[2], group_msg);

        // The last 6 bytes contain the relevant information in byte order
        // get the last 6 bytes by finding the end of the message
        // which is closing parenthesis
        char * pch;
        pch=strrchr(buffer,')');

        // we want to start from the next character
        pch = pch + 1;

        unsigned short* given_checksum = new unsigned short;
        struct in_addr* given_address = new in_addr;

        memcpy(given_checksum, pch, sizeof(unsigned short));
        pch = pch + sizeof(unsigned short);
        memcpy(given_address, pch, sizeof(in_addr));

        printf("\n Probing the oracle \n");
        solve_group_msg(sock_fd, ports[2], given_checksum, &serv_addr.sin_addr);


        // This probes the oracle
        get_udp_response(sock_fd, ports[3], probe_msg);
    }

}
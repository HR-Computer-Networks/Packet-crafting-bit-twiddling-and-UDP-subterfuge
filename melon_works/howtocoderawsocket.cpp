#include<stdio.h>
#include<sys/socket.h> 

int main(int argc , char *argv[])
{
    // create a socket
	int socket_desc;
	socket_desc = socket(AF_INET , SOCK_STREAM , 0); //SOCK_STREAM for TCP, SOCK_DGRAM for UDP
	
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}

    // -------------------------------------------------
    // Connect socket to a server

    struct in_addr {
        unsigned long s_addr;          // load with inet_pton()
    };
    
    struct sockaddr_in {
        short            sin_family;   // e.g. AF_INET, AF_INET6
        unsigned short   sin_port;     // e.g. htons(3490)
        struct in_addr   sin_addr;     // see struct in_addr, below
        char             sin_zero[8];  // zero this if you want to
    };

    struct sockaddr {
        unsigned short    sa_family;    // address family, AF_xxx
        char              sa_data[14];  // 14 bytes of protocol address
    };

	
	return 0;
}
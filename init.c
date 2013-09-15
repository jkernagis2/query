#include "init.h"
#include "shared.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <getopt.h>
#include <arpa/inet.h>



struct sockaddr_in servaddr;
struct sockaddr_in servaddr1;
struct sockaddr_in servaddr2;
struct sockaddr_in servaddr3;
int my_id;
int sockfd;
char myIP[NI_MAXHOST];
pthread_t receive_thread;

void *receive_thread_main(void *discard) {
    struct sockaddr_in fromaddr;
    socklen_t len;
    int nbytes;
	char rIP[NI_MAXHOST];
    char buf[1000];
	char *temp;
    mess_s buff;
    int i;
    for (;;) {
        int source;
        len = sizeof(fromaddr);
        nbytes = recvfrom(sockfd,&buff,sizeof(mess_s),0,(struct sockaddr *)&fromaddr,&len);
        strcpy(buf, buff.words);
        if (nbytes < 0) {
            if (errno == EINTR)
                continue;
            perror("recvfrom");
            exit(1);
        }
        int i;

        source = ntohs(fromaddr.sin_port);

        if (nbytes == 0) {

        }
        else {
			temp = inet_ntoa(fromaddr.sin_addr); // return the IP
        	printf("<%s> %s\n", temp,buf);
            if(strncmp(buf,"/test",5) == 0){
            
                gen_logs(my_id);
            
            }else if(strncmp(buf,"grep",4) == 0){
                d_grep(buf,my_id);
                
                // Send resulting file, result<MYID>.tmp to whoever sent us the grep
            }
        }
    }
}
void getIP(void) {
	struct ifaddrs *ifaddr, *ifa;
	int family, s;


	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		family = ifa->ifa_addr->sa_family;

		if (family == AF_INET) {
			s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), myIP, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
					printf("getnameinfo() failed: %s\n", gai_strerror(s));
					exit(EXIT_FAILURE);
			}
			if(strncmp(ifa->ifa_name, "eth0", 4) == 0){
				printf("My IP: %s\n", myIP);
			}
		}
	}
}
void init_others(void){


	struct hostent *other;
	char buffer[NI_MAXHOST];
	char *temp;
	int i;

	printf("\nPlease Enter 1st IP ");
	fgets(buffer, sizeof(buffer), stdin);
    int len = strlen(buffer);
     /* trim newline */
      if (buffer[len-1] == '\n') {
            buffer[len-1] = 0;
      }
	memset(&servaddr1,0,sizeof(servaddr1));
	servaddr1.sin_family = AF_INET;
	inet_aton(buffer, &servaddr1.sin_addr);
	servaddr1.sin_port = htons(9000); //This is the port for all communicatio


	printf("\nPlease Enter 2nd IP ");
	fgets(buffer, sizeof(buffer), stdin);
    len = strlen(buffer);
     /* trim newline */
      if (buffer[len-1] == '\n') {
            buffer[len-1] = 0;
      }
	memset(&servaddr2,0,sizeof(servaddr2));
	servaddr2.sin_family = AF_INET;
	inet_aton(buffer, &servaddr2.sin_addr);
	servaddr2.sin_port = htons(9000); //This is the port for all communication


	printf("\nPlease Enter 3rd IP ");
	fgets(buffer, sizeof(buffer), stdin);
    len = strlen(buffer);
     /* trim newline */
      if (buffer[len-1] == '\n') {
            buffer[len-1] = 0;
      }
	memset(&servaddr3,0,sizeof(servaddr3));
	servaddr3.sin_family = AF_INET;
	inet_aton(buffer, &servaddr3.sin_addr);
	servaddr1.sin_port = htons(9000); //This is the port for all communication

}
void init(void) {
    //int sockfd;
    int i,n;
	int sockoptval = 1;
	char *temp;
    char trashinput;
	getIP();
    printf("What is my machine ID? ::>  ");
    scanf("%d%c",&my_id,&trashinput); //gets machine id and throws away the return character
    /* set up UDP listening socket */
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }
 	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(myIP);
	servaddr.sin_port = htons(9000); //This is the port for all communication
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
       exit(1);
    }

    /* start receiver thread to make sure we obtain announcements from anyone who tries to contact us */
    if (pthread_create(&receive_thread, NULL, &receive_thread_main, NULL) != 0) {
        fprintf(stderr, "Error in pthread_create\n");
        exit(1);
    }

	init_others();

}
void multicast(const char *message) {
    int i;
	char *temp;
    mess_s value;
    strcpy(value.words, message);

    sendto(sockfd, &value, sizeof(mess_s), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
	sendto(sockfd, &value, sizeof(mess_s), 0, (struct sockaddr *) &servaddr1, sizeof(servaddr1));
	sendto(sockfd, &value, sizeof(mess_s), 0, (struct sockaddr *) &servaddr2, sizeof(servaddr2));
	sendto(sockfd, &value, sizeof(mess_s), 0, (struct sockaddr *) &servaddr3, sizeof(servaddr3));
    if(strncmp(message,"/test",5)==0){
        // We sent the /test command to the others so they generated log files
        // Now we need to do a few greps and verify that the results we get back are correct
    }
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              request.c                                  //
//  Function definitions for gossiping membership lists + heartbeats,   //
//  receiving incoming messages, key-value store ring management, etc;  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// Our files
#include "request.h"


// Socket stuff
struct sockaddr_in myaddr;
struct sockaddr_in lservaddr;

// Socket Descriptors
int reqfd;

// Global machine ID and IP variables
int my_id;
char myc_id;
char myIP[NI_MAXHOST];


// Threads
pthread_t recv_thread;

//Global Flag
int nd_flag;

void send_req(char * request, int key, char * value){
    int i,n;
    int sockoptval = 1;
    getIP();
    /* set up UDP listening socket */
    reqfd = socket(AF_INET,SOCK_DGRAM,0);

    if (reqfd < 0) {
        perror("socket");
        exit(1);
    }
    setsockopt(reqfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));


    memset(&myaddr,0,sizeof(myaddr));
    memset(&lservaddr,0,sizeof(lservaddr));
    myaddr.sin_family = AF_INET;
    lservaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr(myIP);
    lservaddr.sin_addr.s_addr = inet_addr(myIP);
    myaddr.sin_port = htons(9092); //Request port is 9092
    lservaddr.sin_port = htons(9090);

    if (bind(reqfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind");
       exit(1);
    }
    nd_flag = 0;
    if(strncmp(request,"s_lookup",8) == 0){
        if (pthread_create(&recv_thread, NULL, &recv_thread_main, NULL) != 0) {
            fprintf(stderr, "Error in pthread_create\n");
            exit(1);
        }
        nd_flag = 1;
    }
    /*Sending the request here*/
    mess_s req;
    memset(&req,'\0',sizeof(mess_s)); 
    strcpy(req.command, request);
    if(value != NULL)
    {
        strcpy(req.message, value);
    }
    req.nid = key;
    sendto(reqfd, &req, sizeof(mess_s), 0,(struct sockaddr *) &lservaddr, sizeof(lservaddr));
    while(nd_flag){}
}
void getIP(){
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
				printf("My IP: %s\n", myIP); //remove
			}
		}
	}
}

void *recv_thread_main(void *discard){
    struct sockaddr_in fromaddr;
    socklen_t len;
    int nbytes;
    char rIP[NI_MAXHOST];
    char buf[1000];
	char *temp;
    mess_s buff;
    char rbuff[1024];
    for (;;) {
        int source;
        len = sizeof(fromaddr);
	nbytes = recvfrom(reqfd,&buff,sizeof(mess_s),0,(struct sockaddr *)&fromaddr,&len);
        strcpy(buf, buff.command);
        if (nbytes < 0){
            if (errno == EINTR)
                continue;
            perror("recvfrom");
            exit(1);
        }
        source = ntohs(fromaddr.sin_port);

        if(nbytes == 0){}
        else{
            //temp = inet_ntoa(fromaddr.sin_addr);    // return the IP
            //printf("<%s> %s\n", temp,buf);          // Prints incoming message
            if(strncmp(buf,"r_lookup",9) == 0){        // Check if we are doing a test
                printf("Key %d has value: %s\n",buff.nid,buff.message);               // If so, generate logs based on our ID
            nd_flag = 0;
            }else if(strncmp(buf,"dnf_lookup",11) == 0){
               printf("Key %d was not found.\n",buff.nid);
               nd_flag = 0;
            }
        }
    }
}


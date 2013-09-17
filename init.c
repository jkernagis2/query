#include "init.h"
#include "d_grep.h"
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
FILE* fp1;
FILE* fp2;
FILE* fp3;
FILE* fp4;
int flag,t_flag;
char myc_id;
int sockfd;
//int my_port;
char myIP[NI_MAXHOST];
pthread_t receive_thread;
char lf[11] = "resulti.tmp";
int status[4];

void *receive_thread_main(void *discard) {
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
        nbytes = recvfrom(sockfd,&buff,sizeof(mess_s),0,(struct sockaddr *)&fromaddr,&len);
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
                temp = inet_ntoa(fromaddr.sin_addr);    // return the IP
                printf("<%s> %s\n", temp,buf);          // Prints incoming message
                if(strncmp(buf,"/test",5) == 0){        // Check if we are doing a test
                    gen_logs(my_id);                    // If so, generate logs based on our ID
                }
                else if(strncmp(buf,"reply",5) == 0){   // Otherwise check if this is a reply
                    t_flag = 0;
                    char lft[11];                       // Set up a buffer for the file name we need to create
                    strcpy(lft, lf);                    // Copy base filename string into this buffer "resulti.tmp"
                    lft[6] = buff.id;                   // Set id number for file
                    int b_read;
                    
                    switch(buff.id){
                        case '1':
                            fwrite(buff.message, buff.bytes_sent, 1, fp1); // write the number of bytes sent in the message
                            break;
                        case '2':
                            fwrite(buff.message, buff.bytes_sent, 1, fp2); // write the number of bytes sent in the message
                            break;
                        case '3':
                            fwrite(buff.message, buff.bytes_sent, 1, fp3); // write the number of bytes sent in the message
                            break;
                        case '4':
                            fwrite(buff.message, buff.bytes_sent, 1, fp4); // write the number of bytes sent in the message
                            break;
                        default:
                            break;
                    }
                }
                else if(strncmp(buf,"done",4) == 0){    // If done, set the relevant flag
                    status[buff.nid - 1] = 1;
                    flag = 1;
                }
                else if(strncmp(buf,"grep",4) == 0){    // If the command recieved is grep
                    d_grep(buf,my_id);                  // Call the d_grep function
                    mess_s ret;                         // make a mess_s struction
                    memset(&ret,'\0',sizeof(mess_s));   // Clear the message structure
                    if(fromaddr.sin_addr.s_addr != servaddr.sin_addr.s_addr || fromaddr.sin_port != servaddr.sin_port){
                        char lft[11];
                        strcpy(lft, lf);
                        lft[6] = myc_id;
                        FILE *file_ptr;
                        int b_read;
                        file_ptr = fopen(lft, "r");
                        strcpy(ret.command, "reply");
                        ret.id = myc_id;
                        int i = 0;
                        while(!feof(file_ptr))
                        {
                            b_read = fread(rbuff, 1, 50000, file_ptr);
                            //if(b_read < 1024){sleep(1);}
                            strcpy(ret.message, rbuff);
                            ret.bytes_sent = b_read;
                            usleep(100000);
                            //if(i == 20){sleep(3);}
                            sendto(sockfd, &ret, sizeof(mess_s), 0, (struct sockaddr *) &fromaddr, sizeof(fromaddr));
                            i++;
                        }
                        fclose(file_ptr);
                        char remove_buff[14] = "rm resulti.tmp";
                        remove_buff[9] = myc_id;
                        system(remove_buff);
                    }
                    ret.nid = my_id;
                    strcpy(ret.command, "done");
                    sendto(sockfd, &ret, sizeof(mess_s), 0, (struct sockaddr *) &fromaddr, sizeof(fromaddr));
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
	//int tport;					/*For the case of 1 computer with multiple processe*/
	//char trashinput;				/*For the case of 1 computer with multiple processe*/
	
	
	printf("\nPlease Enter 1st IP. ::> ");
	//printf("\nPlease Enter 1st Port ID. ::> ");	/*For the case of 1 computer with multiple processe*/
	fgets(buffer, sizeof(buffer), stdin);
	int len = strlen(buffer);
	/* trim newline */
	if (buffer[len-1] == '\n') {
		buffer[len-1] = 0;
	}
	//scanf("%d%c", &tport,&trashinput); 				/*For the case of 1 computer with multiple processe*/
	memset(&servaddr1,0,sizeof(servaddr1));
	servaddr1.sin_family = AF_INET;
	inet_aton(buffer, &servaddr1.sin_addr);
	//servaddr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /*Default ip,For the case of 1 computer with multiple processe */
	servaddr1.sin_port = htons(9000); //This is the port for all communication
	//servaddr1.sin_port = htons(tport);		/*For the case of 1 computer with multiple processe*/

	printf("\nPlease Enter 2nd IP. ::> ");
	//printf("\nPlease Enter 2nd Port ID. ::> ");	/*For the case of 1 computer with multiple processe*/
	fgets(buffer, sizeof(buffer), stdin);
	len = strlen(buffer);
	/* trim newline */
	if (buffer[len-1] == '\n') {
		 buffer[len-1] = 0;
	}
	//scanf("%d%c", &tport,&trashinput); 				/*For the case of 1 computer with multiple processe*/
	memset(&servaddr2,0,sizeof(servaddr2));
	servaddr2.sin_family = AF_INET;
	inet_aton(buffer, &servaddr2.sin_addr);
	//servaddr2.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /*Default ip,For the case of 1 computer with multiple processe */
	servaddr2.sin_port = htons(9000); //This is the port for all communication
	//servaddr2.sin_port = htons(tport);		/*For the case of 1 computer with multiple processe*/

	printf("\nPlease Enter 3rd IP. ::> ");
	//printf("\nPlease Enter 3rd Port ID. ::> ");	/*For the case of 1 computer with multiple processe*/
	fgets(buffer, sizeof(buffer), stdin);
	len = strlen(buffer);
	/* trim newline */
	if (buffer[len-1] == '\n') {
		buffer[len-1] = 0;
	}
	//scanf("%d%c", &tport,&trashinput); 				/*For the case of 1 computer with multiple processe*/
	memset(&servaddr3,0,sizeof(servaddr3));
	servaddr3.sin_family = AF_INET;
	inet_aton(buffer, &servaddr3.sin_addr);
	//servaddr3.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /*Default ip,For the case of 1 computer with multiple processe */
	servaddr3.sin_port = htons(9000); //This is the port for all communication
	//servaddr3.sin_port = htons(tport);		/*For the case of 1 computer with multiple processe*/
}
void init(void) {
   int i,n;
	int sockoptval = 1;
	char *temp;
    char trashinput;
    getIP();
    printf("What is my machine ID? ::>  ");
    scanf("%d%c",&my_id,&trashinput); //gets machine id and throws away the return character
    /* set up UDP listening socket */
    myc_id = (char)(((int)'0')+my_id);
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(myIP);
    //servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /*Default ip,For the case of 1 computer with multiple processe */
    servaddr.sin_port = htons(9000); //This is the port for all communication
    //printf("\nPlease Enter Port ID. ::> ");	/*For the case of 1 computer with multiple processe*/
    //scanf("%d%c",&my_port,&trashinput); 
    //servaddr.sin_port=htons(my_port);     /* let the operating system choose a port for us. For the case of 1 computer with multiple processes */
    //fprintf(stderr, "My port number: %d\n", my_port);	/*For the case of 1 computer with multiple processe*/
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
    mess_s value;
    strcpy(value.command, message);
    if(strncmp(message,"grep",4)==0){
    	for(i = 0; i<4; i++){
    		status[i]=0;
    	}
        flag = 0;
        fp1 = fopen("result1.tmp","w");
        fp2 = fopen("result2.tmp","w");
        fp3 = fopen("result3.tmp","w");
        fp4 = fopen("result4.tmp","w");
        sendto(sockfd, &value, sizeof(mess_s), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
        t_flag = 0;
        while(1){
            sleep(1);
            t_flag += 1;
            if(flag == 1){
                flag = 0;
                break;
            }else if(t_flag == 10){
                break;
            }
        }
        sendto(sockfd, &value, sizeof(mess_s), 0, (struct sockaddr *) &servaddr1, sizeof(servaddr1));
        t_flag = 0;
        while(1){
            sleep(1);
            t_flag += 1;
            if(flag == 1){
                flag = 0;
                break;
            }else if(t_flag == 10){
                break;
            }
        }
        sendto(sockfd, &value, sizeof(mess_s), 0, (struct sockaddr *) &servaddr2, sizeof(servaddr2));
        t_flag = 0;
        while(1){
            sleep(1);
            t_flag += 1;
            if(flag == 1){
                flag = 0;
                break;
            }else if(t_flag == 10){
                break;
            }
        }
        sendto(sockfd, &value, sizeof(mess_s), 0, (struct sockaddr *) &servaddr3, sizeof(servaddr3));
        t_flag = 0;
        while(1){
            sleep(1);
            t_flag += 1;
            if(flag == 1){
                flag = 0;
                break;
            }else if(t_flag == 10){
                break;
            }
        }

        if(status[0]==0){printf("Machine 1 has failed.\n");}
        if(status[1]==0){printf("Machine 2 has failed.\n");}
        if(status[2]==0){printf("Machine 3 has failed.\n");}
        if(status[3]==0){printf("Machine 4 has failed.\n");}
        fclose(fp1);                                   // close the filepointer
        fclose(fp2);                                   // close the filepointer
        fclose(fp3);                                   // close the filepointer
        fclose(fp4);                                   // close the filepointer

        // Combine the .tmp results files into grep.output and delete the .tmp files
        combine();
        printf("Results recieved. Output located in grep.output.\n");

    }else if(strncmp(message,"/test",5)==0){
        // We sent the /test command to the others so they generated log files
        // Now we need to do a few greps and verify that the results we get back are correct
        int check = 0;
        multicast("grep -k ^INFO");
        check += verify_logs(1);
        multicast("grep -k ^QUERY");
        check += verify_logs(2);
        multicast("grep -v D$");
        check += verify_logs(3);
        multicast("grep -v ^USER_3");
        check += verify_logs(4);
        if(check == 0){printf("Test success!\n");}
        else{printf("Test failure.\n");}
    }
}

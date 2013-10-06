//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            network.h                                 //
//  Function definitions for initializing and maintaining the network   //
//   interface.                                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "network.h"

/************************************************************************/
/*                                                                      */
/*  Various variables used globally within this file. Mainly relating   */
/*    to machine ID storage and such.                                   */
/*                                                                      */
/************************************************************************/

/*New Code*/
struct sockaddr_in myaddr;
struct sockaddr_in mygaddr;

gossip_s* gossip_list;   //Needs to be initialized in init
int num_machines;               //Needs to be initialized in init
int max_machines;               //Needs to be initialized in init


/*End*/

int my_id;
char myc_id;

char myIP[NI_MAXHOST];

sem_t gossip_lock;

FILE* fp1;
FILE* fp2;
FILE* fp3;
FILE* fp4;

int flag,t_flag;
int grepfd;
int gossfd;
int status[4];

pthread_t grep_recv_thread;
pthread_t goss_recv_thread;
pthread_t gossip_thread;
pthread_t monitor_thread;

char lf[11] = "resulti.tmp";

void init(int type, char * servIP) {
    //int grepfd;
    int i,n;
    int sockoptval = 1;
    char *temp;
    char trashinput;
    srand(time(NULL));
    printf("What is my machine ID? ::>  ");
    scanf("%d%c",&my_id,&trashinput); //gets machine id and throws away the return character
    /* set up UDP listening socket */
    myc_id = (char)(((int)'0')+my_id);
    /*Setting up sending/reciveing sockets*/
    grepfd = socket(AF_INET,SOCK_DGRAM,0);
    if (grepfd  < 0) {
        perror("socket");
        exit(1);
    }
    gossfd = socket(AF_INET,SOCK_DGRAM,0);
    if (gossfd < 0) {
        perror("socket");
        exit(1);
    }
     setsockopt(grepfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));
    setsockopt(gossfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));
    memset(&myaddr,0,sizeof(myaddr));
    memset(&myaddr,0,sizeof(mygaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr(myIP);
    myaddr.sin_port = htons(9000); //This is the port for grep communication

    if (bind(grepfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind");
       exit(1);
    }
    mygaddr = myaddr;
    mygaddr.sin_port = htons(9173); //This is the port for grep communication
    if (bind(gossfd, (struct sockaddr *)&mygaddr, sizeof(mygaddr)) < 0) {
        perror("bind");
        exit(1);
    }

    /* Initialize the semaphore */
    sem_init(gossip_lock,0,1);
    
    
    /* start receiver thread to make sure we recieve grep call */
    if (pthread_create(&grep_recv_thread, NULL, &grep_recv_thread_main, NULL) != 0) {
        fprintf(stderr, "Error in pthread_create\n");
        exit(1);
    }

     /* start receiver thread to make sure we recieve gossips */
    if (pthread_create(&goss_recv_thread, NULL, &goss_recv_thread_main, NULL) != 0) {
        fprintf(stderr, "Error in pthread_create\n");
        exit(1);
    }
    /*Setting Up Gossip List*/
    num_machines = 0;
    max_machines = 10;
    gossip_list = malloc(10*sizeof(gossip_s));

    /*Add self to the gossip list*/
    getIP();
    inet_aton(myIP, &gossip_list[0].addr);
    gossip_list[0].counter = 0;
    gossip_list[0].time = (int)time(NULL);
    sprintf(gossip_list[0].id,"%d-%d",my_id,(int)time(NULL));
    gossip_list[0].p_crashed = 0;
    gossip_list[0].has_left = 0;

    /*Add the conecting machine to the gossip list*/
    if(type == 0)
    { 
        inet_aton(servIP, &gossip_list[1].addr);
        gossip_list[1].counter = 0;
        gossip_list[1].time = (int)time(NULL);
        sprintf(gossip_list[1].id,"%d-%d",my_id,(int)time(NULL));
        gossip_list[1].p_crashed = 0;
        gossip_list[1].has_left = 0;

    }
    else{
        printf("My IP: %s\n", myIP);
    }
    num_machines = 1;
    /*Starting Gossip thread*/
    if (pthread_create(&gossip_thread, NULL, &gossip_thread_main, NULL) != 0) {
        fprintf(stderr, "Error in pthread_create\n");
        exit(1);
    }
    if (pthread_create(&monitor_thread, NULL, &monitor_thread_main, NULL) != 0) {
        fprintf(stderr, "Error in pthread_create\n");
        exit(1);
    }
}
void multicast(const char *message) {

    int i;
    mess_s value;
    struct sockaddr_in sendaddr;
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

        memset(&sendaddr, 0, sizeof(sendaddr));
        sendaddr.sin_family = AF_INET;
        sendaddr.sin_port = htons(9000); //This is the port for grep communication


        sem_wait(gossip_lock);//lock
        for(i =0; i<num_machines+1; i++)
        {
            sendaddr.sin_addr.s_addr = gossip_list[i].addr.s_addr;
            sendto(grepfd, &value, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr)) ;
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
        }
        sem_post(gossip_lock);//end lock

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

    }
}
void join(gossip_s* new_gossip){
    sem_wait(gossip_lock);//Lock
    num_machines += 1;

    /*Checks the size of list and makes it bigger if needed*/        
    if(num_machines == max_machines){
        max_machines *= 2;      
        gossip_list = realloc(gossip_list, max_machines*sizeof(gossip_s));   //Doubles the size of the list
    }
    /*Added the new gossip to the list*/
    memcpy(&gossip_list[num_machines], new_gossip, sizeof(gossip_s));
    gossip_list[num_machines].time = (int)time(NULL);
    //File IO saying someone joined
    log_event(my_id,num_machines,"join",gossip_list);
    
    sem_post(gossip_lock);//End Lock
    

}
void leave(int index, int type){

    /*Erase the addrs in the list and shifts that array down*/
    memmove(&gossip_list[index], &gossip_list[index+1], sizeof(gossip_s)*(num_machines-index));
    num_machines--;
    
    //file IO saying someone left
    if(type == 1){
        //Crashed Machine
        log_event(my_id,num_machines,"crash",gossip_list);
    }
    else{
        log_event(my_id,num_machines,"leave",gossip_list);
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
            }
        }
    }
}
void *grep_recv_thread_main(void *discard) {
    struct sockaddr_in fromaddr;
    socklen_t len;
    int nbytes;
    char rIP[NI_MAXHOST];
    char buf[1000];
    char *temp;
    mess_s buff;
    char rbuff[1024];
    while(1) {
        int source;
        len = sizeof(fromaddr);
        nbytes = recvfrom(grepfd,&buff,sizeof(mess_s),0,(struct sockaddr *)&fromaddr,&len);
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
                if(strncmp(buf,"reply",5) == 0){   // Otherwise check if this is a reply
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
                    if(fromaddr.sin_addr.s_addr != gossip_list[0].addr.s_addr){
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
                            strcpy(ret.message, rbuff);
                            ret.bytes_sent = b_read;
                            usleep(100000);
                            sendto(grepfd, &ret, sizeof(mess_s), 0, (struct sockaddr *) &fromaddr, sizeof(fromaddr));
                            i++;
                        }
                        fclose(file_ptr);
                        char remove_buff[14] = "rm resulti.tmp";
                        remove_buff[9] = myc_id;
                        system(remove_buff);
                    }
                    ret.nid = my_id;
                    strcpy(ret.command, "done");
                    sendto(grepfd, &ret, sizeof(mess_s), 0, (struct sockaddr *) &fromaddr, sizeof(fromaddr));
                }
            }
    }
}
void *goss_recv_thread_main(void *discard) {
    int i, j, nbytes;
    struct sockaddr_in fromaddr;
    socklen_t len;
    gossip_s rbuff[10];

    while(1) {
        int source;
        len = sizeof(fromaddr);
        gossip_m_s rbuff;
        nbytes = recvfrom(gossfd,&rbuff,sizeof(gossip_m_s),0,(struct sockaddr *)&fromaddr,&len);
        if (nbytes < 0){
            if (errno == EINTR)
                continue;
            perror("recvfrom");
            exit(1);
        }

        sem_wait(gossip_lock);//lock
        /*Compare gossip to local gossip struct */
        for(i = 0; i<rbuff.num_gossip; i++)
        {
            for(j = 0; j<(num_machines +1); j++)
            {
                if(rbuff.gossips[i].addr.s_addr == gossip_list[j].addr.s_addr)
                {
                    /*Updates List*/
                    if(rbuff.gossips[i].counter > gossip_list[j].counter)
                    {
                        memcpy(&gossip_list[j], &rbuff.gossips[i],sizeof(gossip_s));
                        gossip_list[j].time =(int)time(NULL);
                        continue;
                    }
                }
                else if(j == num_machines)
                {
                    if(rbuff.gossips[i].has_left == 0 && rbuff.gossips[i].p_crashed == 0)
                    {
                         join(&rbuff.gossips[i]);    //adds the new gossip if its not there.
                    }
                }

            }
        }
       
        sem_post(gossip_lock);//endlock
    }
}
void *gossip_thread_main(void *discard) {
    int i, j,k;
    int index;
    int * index_array;
    int * gossipees;
    int num_gossip;
    int  tnm;
    gossip_m_s sendg;


    struct sockaddr_in sendaddr;
    memset(&sendaddr, 0, sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(9173);

    while(1)
    {
        sem_wait(gossip_lock);//lock
        gossip_list[0].counter++;       //increase heartbeat
        tnm = num_machines;
        index_array = malloc(num_machines*sizeof(int)+1);
        for(i =0; i<num_machines; i++)
        {
            index_array[i] = i;
        }
        index_array[num_machines] = 0;
        num_gossip = log(num_machines)/log(2);
        if(num_gossip == 0)
        {
            num_gossip =1;
        }
        gossipees = malloc(num_gossip*sizeof(int));
        for(i = 0; i< num_gossip; i++)
        {
            index = rand()%tnm;
            sendaddr.sin_addr = gossip_list[index_array[index]].addr;
            for(j=0; j < num_machines; j += 10)
            {
                for(k =0; k<(num_machines-j); k++)
                {
                    sendg.gossips[k] = gossip_list[k+j];
                }
                sendg.num_gossip = k;
                sendto(gossfd, &sendg, sizeof(gossip_m_s), 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr)) ;
            }
            memmove(&index_array[index], &index_array[index+1], sizeof(int)*(tnm-index));
            tnm--;
        }
        sem_post(gossip_lock);//end lock
        //wait
    }   
}
void *monitor_thread_main(void *discard) {
    int i;
    int tempt = (int)time(NULL);
    sem_wait(gossip_lock);//lock
    while(1)
    {
        for(i = 1; i<=num_machines; i++)
        {
            if(gossip_list[i].has_left == 1)
            {
                if((tempt-gossip_list[i].time) > 1/*NEED CHANGE*/)
                {
                    leave(i, 0);
                }
                continue;
            }
            else if(gossip_list[i].p_crashed == 1)
            {
                if((tempt-gossip_list[i].time) > 1/*NEED CHANGE*/)
                {
                    leave(i, 1);
                }
                continue;
            }
            else if((tempt-gossip_list[i].time) > 1/*NEED CHANGE*/)
            {
                gossip_list[i].p_crashed =1;
            }
        }
        //wait
    }

    sem_post(gossip_lock);//end lock
}

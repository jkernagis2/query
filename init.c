//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              init.c                                  //
//  Function definitions for gossiping membership lists + heartbeats,   //
//  receiving incoming messages, key-value store ring management, etc;  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// Our files
#include "init.h"

// Externs
extern keyval* mykv;
extern ring_n* myring;

// Socket stuff
struct sockaddr_in myaddr;
struct sockaddr_in mygaddr;

struct sockaddr_in retaddr;

// Membership list and counter variables
gossip_s* gossip_list;
volatile int num_machines;
volatile int max_machines;

// Int flags and semaphore
volatile int leaving_group_flag = 0;
volatile int flag;
volatile int t_flag;
volatile int server_flag;
volatile int status[4];
sem_t gossip_lock;
sem_t test_lock;

// Socket Descriptors
int grepfd;
int gossfd;

// Global machine ID and IP variables
int my_id;
char myc_id;
char myIP[NI_MAXHOST];

// Old file pointers from MP1
FILE* fp1;
FILE* fp2;
FILE* fp3;
FILE* fp4;

// Threads
pthread_t grep_recv_thread;
pthread_t goss_recv_thread;
pthread_t gossip_thread;
pthread_t monitor_thread;

// Static file name array used in MP1
char lf[11] = "resulti.tmp";
void delete_ring();
void init_ring();

int replicas;

void init(int type, char * servIP){
    int i,n;
    int sockoptval = 1;
    char *temp;
    char trashinput;
    server_flag = type;
    getIP();
    printf("What is my machine ID? ::>  ");
    scanf("%d%c",&my_id,&trashinput); //gets machine id and throws away the return character
    /* set up UDP listening socket */
    myc_id = (char)(((int)'0')+my_id);
    grepfd = socket(AF_INET,SOCK_DGRAM,0);
    gossfd = socket(AF_INET,SOCK_DGRAM,0);

    if (grepfd < 0) {
        perror("socket");
        exit(1);
    }
    if (gossfd < 0) {
        perror("socket");
        exit(1);
    }
    setsockopt(grepfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));
    setsockopt(gossfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));

    memset(&myaddr,0,sizeof(myaddr));
    memset(&mygaddr,0,sizeof(mygaddr));
    myaddr.sin_family = AF_INET;
    mygaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr(myIP);
    mygaddr.sin_addr.s_addr = inet_addr(myIP);
    myaddr.sin_port = htons(9090);
    mygaddr.sin_port = htons(9091);

    if (bind(grepfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind");
       exit(1);
    }
    if (bind(gossfd, (struct sockaddr *)&mygaddr, sizeof(mygaddr)) < 0) {
        perror("bind");
        exit(1);
    }

    /* Initialize the semaphore */
    sem_init(&gossip_lock,0,1);
    sem_init(&test_lock,0,1);

    /*Setting Up Gossip List*/
    num_machines = 0;
    max_machines = 10;
    gossip_list = malloc(10*sizeof(gossip_s));

    /*Add self to the gossip list*/
    gossip_list[0].addr.s_addr = myaddr.sin_addr.s_addr;
    gossip_list[0].counter = 0;
    gossip_list[0].time = (int)time(NULL);
    sprintf(gossip_list[0].id,"%d-%d",my_id,(int)time(NULL));
    gossip_list[0].p_crashed = 0;
    gossip_list[0].has_left = 0;
    gossip_list[0].has_left = 0;
    /*MP3*/
    gossip_list[0].ring_id = get_hashed_id();
    if(type == 0)
    {
        gossip_list[1].addr.s_addr = inet_addr(servIP);
        gossip_list[1].counter = 0;
        gossip_list[1].time = (int)time(NULL);
        sprintf(gossip_list[1].id,"%d-%d",my_id,(int)time(NULL));
        gossip_list[1].p_crashed = 0;
        gossip_list[1].has_left = 0;
        num_machines = 1;
    }
    else{
        printf("My IP: %s\n", myIP);
    }

    if (pthread_create(&grep_recv_thread, NULL, &grep_recv_thread_main, NULL) != 0) {
        fprintf(stderr, "Error in pthread_create\n");
        exit(1);
    }
    if (pthread_create(&goss_recv_thread, NULL, &goss_recv_thread_main, NULL) != 0) {
        fprintf(stderr, "Error in pthread_create\n");
        exit(1);
    }
    /*Starting Gossip thread*/
    if (pthread_create(&gossip_thread, NULL, &gossip_thread_main, NULL) != 0) {
        fprintf(stderr, "Error in pthread_create\n");
        exit(1);
    }
    if (pthread_create(&monitor_thread, NULL, &monitor_thread_main, NULL) != 0) {
        fprintf(stderr, "Error in pthread_create\n");
        exit(1);
    }

    /*Ring Initialization*/
    init_ring();
    replicas = 0;

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
void multicast(const char *message){

    int i;
    mess_s value;
    strcpy(value.command, message);
    struct sockaddr_in sendaddr;
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
        sendaddr.sin_port = htons(9090); //This is the port for grep communication

        //sem_wait(&gossip_lock);//lock
        for(i =0; i<=num_machines; i++)
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
        //sem_post(&gossip_lock);//end lock



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
void join(gossip_s* new_gossip){
    printf("New Machine\n");
    add_to_ring(new_gossip->ring_id, new_gossip->addr);
    //shift_keys();
	fix_keys();
    if((new_gossip->addr.s_addr == gossip_list[1].addr.s_addr) && (server_flag == 0))
    {
        gossip_list[1].addr = new_gossip->addr;
        gossip_list[1].counter = new_gossip->counter;
        gossip_list[1].p_crashed = new_gossip->p_crashed;
        gossip_list[1].has_left = new_gossip->has_left;
        strncpy(gossip_list[1].id, new_gossip->id, 50);
       gossip_list[1].time =(int)time(NULL);
       gossip_list[1].ring_id = new_gossip->ring_id;
       //File IO saying someone joined
       log_event(my_id,num_machines,"join",gossip_list);
    }
    else
    {
        num_machines ++;

        /*Checks the size of list and makes it bigger if needed*/
        if(num_machines == max_machines){
            max_machines *= 2;
            gossip_list = realloc(gossip_list, max_machines*sizeof(gossip_s)); //Doubles the size of the list
        }
        /*Added the new gossip to the list*/
        //memcpy(&gossip_list[num_machines], new_gossip, sizeof(gossip_s));
        gossip_list[num_machines].addr = new_gossip->addr;
        gossip_list[num_machines].counter = new_gossip->counter;
        gossip_list[num_machines].p_crashed = new_gossip->p_crashed;
        gossip_list[num_machines].has_left = new_gossip->has_left;
        strncpy(gossip_list[num_machines].id, new_gossip->id, 50);
       gossip_list[num_machines].time =(int)time(NULL);
       gossip_list[num_machines].ring_id = new_gossip->ring_id;
       //File IO saying someone joined
       log_event(my_id,num_machines,"join",gossip_list);
    }
}
void leave(int index, int type){

    //file IO saying someone left
    remove_from_ring(gossip_list[index].ring_id, 0);
    if(type == 1){
        //Crashed Machine
        if((index != 1) || (server_flag == 1)){
            /*Erase the addrs in the list and shifts that array down*/
            memmove(&gossip_list[index], &gossip_list[index+1], sizeof(gossip_s)*(num_machines-index));
            num_machines--;
            log_event(my_id,num_machines,"crash",gossip_list);
        }
        else{
            gossip_list[1].p_crashed = 2;
            log_event(my_id,num_machines,"hostcrash",gossip_list);
        }
    }
    else{
    	if((index != 1) || (server_flag == 1)){
            /*Erase the addrs in the list and shifts that array down*/
            memmove(&gossip_list[index], &gossip_list[index+1], sizeof(gossip_s)*(num_machines-index));
            num_machines--;
            log_event(my_id,num_machines,"leave",gossip_list);
        }
        else{
            gossip_list[1].p_crashed = 2;
            log_event(my_id,num_machines,"hostleave",gossip_list);
        }
    }
}
void set_leave(){
    sem_wait(&gossip_lock);//lock
	leaving_group_flag = 1;
    /*Ring Management*/
    remove_from_ring(gossip_list[0].ring_id, 0);
    delete_ring();
    
    sem_post(&gossip_lock);//endlock
	// Clear local membership list from [2] to [END]
	// IF(WE ARE SERVER){ clear membership list[1];}

	return;
}
void rejoin(){
	// Generate new ID and stuff
    sem_wait(&gossip_lock);//lock
        init_ring();

	    gossip_list[0].counter = 0;
	    gossip_list[0].time = (int)time(NULL);
	    sprintf(gossip_list[0].id,"%d-%d",my_id,(int)time(NULL));
	    gossip_list[0].p_crashed = 0;
	    gossip_list[0].has_left = 0;
	    leaving_group_flag = 0;
        if(server_flag == 0)
        {
            gossip_list[1].counter = 0;
            gossip_list[1].time = (int)time(NULL);
            sprintf(gossip_list[1].id,"%d-%d",my_id,(int)time(NULL));
            gossip_list[1].p_crashed = 0;
            gossip_list[1].has_left = 0;
            num_machines = 1;
        }
    sem_post(&gossip_lock);//endlock
	return;
}
void init_ring(){
    myring = malloc(sizeof(ring_n));
    myring->value = gossip_list[0].ring_id;
    myring->next = NULL;
    myring->prev = NULL;
    myring->addr.s_addr = myaddr.sin_addr.s_addr;

}
void add_to_ring(int newid, struct in_addr new_addr){

    ring_n* n_node;
    ring_n* prev = NULL;
    ring_n* current = myring;

    n_node = malloc(sizeof(ring_n));
    n_node->value = newid;
    n_node->addr.s_addr = new_addr.s_addr;

    while(current->value < newid){
        prev = current;
        current = current->next;
        if(current == NULL){
            break;
        }
    }
    n_node->prev = prev;
    n_node->next = current;

    // Set prev's next pointer if prev exists
    if(prev != NULL){
        prev->next = n_node;
    }

    // Set current's prev pointer, if current exists
    if(current != NULL){
        current->prev = n_node;
    }

    if(myring == n_node->next){
        myring = n_node;
    }
    if(n_node->next !=NULL)
    {
        if(my_id == (n_node->next)->value)
        {
            shift_keys();
        }
    }
    else{
        if(my_id == myring->value)
        {
            shift_keys();
        }
    }

}
void remove_from_ring(int id, int type){
    
    ring_n* temp = myring;
	ring_n* prev;
	ring_n* next;
    while(temp != NULL)
    {
		prev = temp->prev;
		next = temp->next;
        if(temp->value == id)
        {
            if(temp == myring)
            {
                myring = temp->next;
            }
           break;
        }
        temp = temp->next;
    }
    if(temp != NULL){
	
		if(id = gossip_list[0].ring_id)
		{
			leave_shift(temp);
		}
		else if(type == 1){
			if(gossip_list[0].ring_id == gpn(temp)->value)
			{
				crash_shift(temp, 0);
			}
			else if(gossip_list[0].ring_id  == gnn(temp)->value){
				crash_shift(temp, 1);
			}
		}
		
        if(temp->next !=NULL)
        {
            (temp->next)->prev = temp->prev;
        }
        if(temp->prev !=NULL)
        {
            (temp->prev)->next = temp->next;
        }
        free(temp);
    }
}
void delete_ring(){
    ring_n* temp = myring;
    ring_n* temp_next;
    while(temp != NULL)
    {
        temp_next = temp->next;
        free(temp);
        temp = temp_next;
    }
    myring = NULL;
}
void *grep_recv_thread_main(void *discard){
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
			    if(leaving_group_flag != 0){continue;}
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
                else if(strncmp(buf,"insert",6) == 0){
                    local_insert(buff.nid, buff.message,1);
                    mess_s ret;
                    memset(&ret,'\0',sizeof(mess_s));   // Clear the message structure
                    ret.nid = buff.nid;
                    strcpy(ret.command, "i_done");

                    sendto(grepfd, &ret, sizeof(mess_s), 0, (struct sockaddr *) &fromaddr, sizeof(fromaddr));
                }
                else if(strncmp(buf,"replicate",9) == 0){
                    local_insert(buff.nid, buff.message,0);
                    replicas++;
                    /*mess_s ret;
                    memset(&ret,'\0',sizeof(mess_s));   // Clear the message structure
                    ret.nid = buff.nid;
                    strcpy(ret.command, "i_done");

                    sendto(grepfd, &ret, sizeof(mess_s), 0, (struct sockaddr *) &fromaddr, sizeof(fromaddr));*/
                }
                else if(strncmp(buf,"lookup",6) == 0){
                    mess_s ret;
                    memset(&ret,'\0',sizeof(mess_s));   // Clear the message structure
                    ret.nid = buff.nid;
                    char* looked_up = local_lookup(buff.nid);
                    if(looked_up != NULL){
                        strcpy(ret.command, "r_lookup");
                        strcpy(ret.message,looked_up);
                    }
                    else{
                        strcpy(ret.command, "dnf_lookup");
                    }

                    sendto(grepfd, &ret, sizeof(mess_s), 0, (struct sockaddr *) &fromaddr, sizeof(fromaddr));
                }
                else if(strncmp(buf,"update",6) == 0){
                    local_update(buff.nid, buff.message,1);
                }
				else if(strncmp(buf,"rep_update",10) == 0){
                    local_update(buff.nid, buff.message,0);
				}
                else if(strncmp(buf,"delete",6) == 0){
                    local_delete(buff.nid, 1);
                }
				else if(strncmp(buf,"rep_delete",10) == 0){
                    local_delete(buff.nid, 0);
					replicas--;
                }
                else if(strncmp(buf,"r_lookup",8) == 0){
                   //sendto(grepfd, &buff, sizeof(mess_s), 0, (struct sockaddr *) &retaddr, sizeof(retaddr));
                   printf("Key %d has value: %s\n",buff.nid,buff.message);
                   sem_post(&test_lock);
                }else if(strncmp(buf,"dnf_lookup",10) == 0){
                   //sendto(grepfd, &buff, sizeof(mess_s), 0, (struct sockaddr *) &retaddr, sizeof(retaddr));
                   printf("Key %d was not found.\n",buff.nid);
                   sem_post(&test_lock);
                }else if(strncmp(buf,"i_done",6) == 0){
                   printf("Key insert finished.\n");
                   sem_post(&test_lock);
                }


                /*External Calls*/
                else if(strncmp(buf,"s_insert",8) == 0){
                    insert(buff.nid, buff.message);
                }else if(strncmp(buf,"s_lookup",8) == 0){
                    lookup(buff.nid);
                    memset(&retaddr, 0, sizeof(retaddr));
                    memcpy(&retaddr, &fromaddr, sizeof(struct sockaddr_in));
                }else if(strncmp(buf,"s_update",8) == 0){
                    update(buff.nid, buff.message);
                }else if(strncmp(buf,"s_delete",8) == 0){
                    delete_k(buff.nid);
                }
            }
    }
}
void *goss_recv_thread_main(void *discard){
    struct sockaddr_in fromaddr;
    socklen_t len;
    int nbytes,i,j;
    char rIP[NI_MAXHOST];
    char *temp;
    char *temp1;
    char *temp2;
    //mess_s buff;
    gossip_m_s buff;
    char rbuff[1024];
    for (;;) {

        int source;
        len = sizeof(fromaddr);
        nbytes = recvfrom(gossfd,&buff,sizeof(gossip_m_s),0,(struct sockaddr *)&fromaddr,&len);
        if (nbytes < 0){
            if (errno == EINTR)
                continue;
            perror("recvfrom");
            exit(1);
        }
        source = ntohs(fromaddr.sin_port);

        if(nbytes == 0){}
        else{
            if(leaving_group_flag != 0){continue;}
            temp = inet_ntoa(fromaddr.sin_addr);    // return the IP
            //printf("<%s>:Sent Gossip\n", temp);         // Prints incoming message

            sem_wait(&gossip_lock);//lock
            /*Compare gossip to local gossip struct */
            for(i = 0; i<buff.num_gossip; i++)
            {
            temp2 = inet_ntoa(buff.gossips[0].addr);
                for(j = 0; j<(num_machines+1); j++)
                {

                    if(buff.gossips[i].addr.s_addr != gossip_list[0].addr.s_addr){
                        if(strncmp(buff.gossips[i].id,gossip_list[j].id,50) == 0)
                        {
                            /*Updates List*/
                            if(buff.gossips[i].counter > gossip_list[j].counter)
                            {
                                gossip_list[j].counter = buff.gossips[i].counter;
                                gossip_list[j].p_crashed = buff.gossips[i].p_crashed;
                                gossip_list[j].has_left = buff.gossips[i].has_left;
                                gossip_list[j].time =(int32_t)time(NULL);
                                strncpy(gossip_list[j].id, buff.gossips[i].id, 50);
                            }
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                if(j > num_machines)
                {
                    if(buff.gossips[i].has_left == 0 && buff.gossips[i].p_crashed == 0)
                    {
                        join(&buff.gossips[i]); //adds the new gossip if its not there.
                    }
                }

            }

        sem_post(&gossip_lock);//endlock
        }
    }
}
void *gossip_thread_main(void *discard){
    int i, j,k;
    int index;
    int * index_array;
    int num_gossip;
    int tnm;
    gossip_m_s sendg;


    struct sockaddr_in sendaddr;
    memset(&sendaddr, 0, sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(9091);

    for(;;)
    {
        sem_wait(&gossip_lock);//lock

    	if(leaving_group_flag == 1)
    	{
    		leaving_group_flag++;
    		gossip_list[0].has_left = 1;
    	}
    	else if(leaving_group_flag == 2)
    	{
	        if(server_flag)
	        {
		        num_machines = 0;
	        }
	        else
	        {
		        num_machines = 1;
	        }
            sem_post(&gossip_lock);//end lock
    	    continue;
    	}

        gossip_list[0].counter++; //increase heartbeat
        tnm = num_machines;
        index_array = malloc(num_machines*sizeof(int));
        for(i =0; i<=num_machines; i++)
        {
            index_array[i] = i;
        }
        num_gossip = log(num_machines)/log(2);
        if(num_gossip == 0)
        {
            num_gossip =1;
        }
        for(i = 0; i< num_gossip; i++)
        {
            index = (rand()%tnm) +1;
            sendaddr.sin_addr.s_addr = gossip_list[index_array[index]].addr.s_addr;
            //for(j=0; j <= num_machines; j += 10)
            //{
                for(k =0; k<=num_machines; k++)
                {
                    memcpy(&sendg.gossips[k], &gossip_list[k], sizeof(gossip_s));
                }
                sendg.num_gossip = k;
                sendto(gossfd, &sendg, sizeof(gossip_m_s), 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr)) ;
            //}
            memmove(&index_array[index], &index_array[index+1], sizeof(int)*(tnm-index));
            tnm--;
        }
        sem_post(&gossip_lock);//end lock
        usleep(200000);//wait
    }
}
void *monitor_thread_main(void *discard){
    int i;
    int tempt;

    while(1)
    {
        sem_wait(&gossip_lock);//lock
    	if(leaving_group_flag != 0){sem_post(&gossip_lock); continue;}
        tempt = (int)time(NULL);
        for(i = 1; i<=num_machines; i++)
        {
            //printf("Checking\n");
            if((i == 1) && (gossip_list[1].p_crashed == 2)){continue;}
            if(gossip_list[i].has_left == 1)
            {
                if((int)(tempt-gossip_list[i].time) > 3/*NEED CHANGE*/)
                {
                    leave(i, 0);
                }
                continue;
            }
            else if(gossip_list[i].p_crashed == 1)
            {
                //printf("Starting to Crash\n");
                if((int)(tempt-gossip_list[i].time) > 4/*NEED CHANGE*/)
                {
                    if(TEST_MODE != 1){
                        printf("Crashing\n");
                        leave(i, 1);
                    }
                }
                continue;
            }
            else if((int)(tempt-gossip_list[i].time) > 2/*NEED CHANGE*/)
            {
                if(TEST_MODE != 1){
                    gossip_list[i].p_crashed =1;
                }
            }
            //printf("Time diff = %d   i = %d\n",(int)(tempt-gossip_list[i].time), i);
        }
        sem_post(&gossip_lock);//end lock
        sleep(1);//wait
    }


}







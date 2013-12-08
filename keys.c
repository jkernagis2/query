//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              keys.c                                  //
//  Function definitions for the handling of ring based key-value store.//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "keys.h"
#define NUMTEST  20
volatile int lookup_requests = 0;

void local_insert(int key, char* value, int type){
    // Allocate new keyval structure and linked list traversing pointers
    keyval* new_key = malloc(sizeof(keyval));
    keyval* current = mykv;
    keyval* prev = NULL;

    // Initialize the new keyval structure
    new_key->key = key;
    new_key->value = malloc(strlen(value));
    strcpy(new_key->value,value);
    new_key->next = NULL;

    // Base case for first keyval
    if(mykv == NULL){
        // We are the first, so there is no previous keyval element
        new_key->prev = NULL;
        // Set the linked list start to be this keyval
        mykv = new_key;
    }
    // If not first keyval
    else{
        // Traverse the linked list until we find that current's key is greater then the key to be inserted
        // Or we hit the end of the list
        while(current->key < key){
            prev = current;
            current = current->next;
            if(current == NULL){
                break;
            }
        }
        if(current != NULL){
            if(current->key == key){
                free(new_key);
				if(type == 1){
					int i;
					struct sockaddr_in sendaddr;
					memset(&sendaddr, '\0', sizeof(sendaddr));
					sendaddr.sin_family = AF_INET;
					sendaddr.sin_port = htons(9090);


					mess_s message;
					memset(&message,'\0',sizeof(message));
					message.nid = key;
					strncpy(message.command,"replicate",9);
					strncpy(message.message,value,strlen(value));
					struct in_addr address[3];
					get_rep_addr(key, address);
					for(i = 0; i < 2; i++){
						sendaddr.sin_addr = address[i+1];
						sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr));
					}
				}
                return;
            }
        }
        // New key's previous pointer = prev, next pointer = current
        new_key->prev = prev;
        new_key->next = current;

        // Set prev's next pointer if prev exists
        if(prev != NULL){
            prev->next = new_key;
        }

        // Set current's prev pointer, if current exists
        if(current != NULL){
            current->prev = new_key;
        }
        if(mykv == new_key->next){
            mykv = new_key;
        }
    }
    /*MP4*/
    if(type == 1){
        int i;
        struct sockaddr_in sendaddr;
        memset(&sendaddr, '\0', sizeof(sendaddr));
        sendaddr.sin_family = AF_INET;
        sendaddr.sin_port = htons(9090);


        mess_s message;
        memset(&message,'\0',sizeof(message));
        message.nid = key;
        strncpy(message.command,"replicate",9);
        strncpy(message.message,value,strlen(value));
        struct in_addr address[3];
        get_rep_addr(key, address);
        for(i = 0; i < 2; i++){
            sendaddr.sin_addr = address[i+1];
            sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr));
        }
    }
}
char* local_lookup(int key){
    keyval* current = mykv;
    lookup_requests++;
    while(current != NULL){
        if(current->key == key){
            return current->value;
        }
        else{
            current = current->next;
        }
    }
    return NULL;
}
void local_update(int key, char* new_val, int type){
    keyval* current = mykv;
    while(current != NULL){
        if(current->key == key){
            free(current->value);
            current->value = malloc(strlen(new_val));
            strcpy(current->value,new_val);
			if(type == 1){
				int i;
				struct sockaddr_in sendaddr;
				memset(&sendaddr, '\0', sizeof(sendaddr));
				sendaddr.sin_family = AF_INET;
				sendaddr.sin_port = htons(9090);


				mess_s message;
				memset(&message,'\0',sizeof(message));
				message.nid = key;
				strncpy(message.command,"rep_update",9);
				strncpy(message.message,new_val,strlen(new_val));
				struct in_addr address[3];
				get_rep_addr(key, address);
				for(i = 0; i < 2; i++){
					sendaddr.sin_addr = address[i+1];
					sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr));
				}
			}
            return;
        }
        else{
            current = current->next;
        }
    }
    return;
}
void local_delete(int key, int type){
    keyval* current = mykv;
    keyval* temp = NULL;
    while(current != NULL){
        if(current->key == key){
            if(current->prev != NULL){
                current->prev->next = current->next;
            }
            if(current->next != NULL){
                current->next->prev = current->prev;
            }
            if(mykv == current){
                mykv = current->next;
            }
            free(current->value);
            free(current);
			if(type == 1){
				int i;
				struct sockaddr_in sendaddr;
				memset(&sendaddr, '\0', sizeof(sendaddr));
				sendaddr.sin_family = AF_INET;
				sendaddr.sin_port = htons(9090);


				mess_s message;
				memset(&message,'\0',sizeof(message));
				message.nid = key;
				strncpy(message.command,"rep_delete",10);
				struct in_addr address[3];
				get_rep_addr(key, address);
				for(i = 0; i < 2; i++){
					sendaddr.sin_addr = address[i+1];
					sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr));
				}
			}
            return;
        }
        else{
            current = current->next;
        }
    }
    return;
}
void local_show(){
    keyval* current = mykv;
    int counter = 0;
    // Print local keys stored here
    printf("Local Keys:\n");
    while(current != NULL){
        printf("%d, ",current->key);
        counter++;
        current = current->next;
    }
    printf("\nNumber Keys: %d\nLocal Membership List:\n",counter);

    ring_n* current_r = myring;
    while(current_r != NULL){
        printf("%d, ", current_r->value);
        current_r = current_r->next;
    }
    printf("\nLookup Requests Recieved: %d\n",lookup_requests);

}
void insert(int key, char* val){
    struct sockaddr_in sendaddr;
    memset(&sendaddr, '\0', sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(9090);
    sendaddr.sin_addr = get_addr(key);


    mess_s message;
    memset(&message,'\0',sizeof(message));
    message.nid = key;
    strncpy(message.command,"insert",6);
    strncpy(message.message,val,strlen(val));

    sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr)) ;
}
void lookup(int key){
    struct sockaddr_in sendaddr;
    memset(&sendaddr, '\0', sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(9090);
    sendaddr.sin_addr = get_addr(key);


    mess_s message;
    memset(&message,'\0',sizeof(message));
    message.nid = key;
    strncpy(message.command,"lookup",6);

    sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr)) ;
}
void update(int key, char* new_val){
    struct sockaddr_in sendaddr;
    memset(&sendaddr, '\0', sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(9090);
    sendaddr.sin_addr = get_addr(key);


    mess_s message;
    memset(&message,'\0',sizeof(message));
    message.nid = key;
    strncpy(message.command,"update",6);
    strncpy(message.message,new_val,strlen(new_val));

    sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr)) ;
}
void delete_k(int key){
    struct sockaddr_in sendaddr;
    memset(&sendaddr, '\0', sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(9090);
    sendaddr.sin_addr = get_addr(key);


    mess_s message;
    memset(&message,'\0',sizeof(message));
    message.nid = key;
    strncpy(message.command,"delete",6);

    sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr)) ;
}
void shift_keys(ring_n* ring_pos, int pn){
    struct sockaddr_in sendaddr;
    memset(&sendaddr, '\0', sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(9090);

    mess_s message;
    memset(&message,'\0',sizeof(message));
    strncpy(message.command,"replicate",9);
    
    struct in_addr address[3];
    keyval* temp = mykv;
    ring_n*  next_p;

    while(temp != NULL)
    {
        message.nid = temp->key;
        strncpy(message.message,temp->value,strlen(temp->value));
        get_rep_addr(temp->key, address);
        if(address[0].s_addr == ring_pos->addr.s_addr && pn  == 2){
            next_p = ring_pos;
        }
        else if(address[1].s_addr == ring_pos->addr.s_addr && pn == 1)
        {
            next_p = ring_pos;

        }
        else if(address[2].s_addr == ring_pos->addr.s_addr && pn == 0)
        {
            next_p = ring_pos;
        }
        else{
            temp = temp->next; 
            continue;
        }
        sendaddr.sin_addr = next_p->addr;
        sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr));
        local_delete(temp->key, 0);

        temp = temp->next;
    }
}
void move_keys(){
    keyval* temp = mykv;
    keyval* temp_next;
	mykv == NULL;
    while(temp != NULL)
    {
        temp_next = temp->next;
        insert(temp->key, temp->value);
        local_delete(temp->key, 0);
        temp = temp_next;
    }
}
void fix_keys(){
	keyval* temp = mykv;
    keyval* temp_next;
	mykv = NULL;
    while(temp != NULL)
    {
        temp_next = temp->next;
        insert(temp->key, temp->value);
		free(temp->value);
		free(temp);
        temp = temp_next;
    }
}
void leave_shift(ring_n* ring_pos){
	struct sockaddr_in sendaddr;
    memset(&sendaddr, '\0', sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(9090);

	mess_s message;
	memset(&message,'\0',sizeof(message));
	strncpy(message.command,"replicate",9);
	
	struct in_addr address[3];
	ring_n*  next_p;
	while(mykv != NULL)
	{
		message.nid = mykv->key;
		strncpy(message.message,mykv->value,strlen(mykv->value));
		get_rep_addr(mykv->key, address);
		if(address[0].s_addr == ring_pos->addr.s_addr){
			next_p = gnn(gnn(gnn(ring_pos)));
		}
		else if(address[1].s_addr == ring_pos->addr.s_addr)
		{
			next_p = gnn(gnn(ring_pos));

		}
		else if(address[2].s_addr == ring_pos->addr.s_addr)
		{
			next_p = gnn(ring_pos);
		}
		else{ 
			printf("WTF-Leave Shift in keys\n"); 
			while(1){}
		}
		sendaddr.sin_addr = next_p->addr;
		sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr));
		local_delete(mykv->key, 0);
		
	}
}
void crash_shift(ring_n* ring_pos, int pn){

	struct sockaddr_in sendaddr;
    memset(&sendaddr, '\0', sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(9090);

	mess_s message;
	memset(&message,'\0',sizeof(message));
	strncpy(message.command,"replicate",9);
	
	struct in_addr address[3];
	keyval* temp = mykv;
	ring_n*  next_p;

	while(temp != NULL)
	{
		message.nid = temp->key;
		strncpy(message.message,temp->value,strlen(temp->value));
		get_rep_addr(temp->key, address);
		if(address[0].s_addr == ring_pos->addr.s_addr && pn  == 1){
			next_p = gnn(gnn(gnn(ring_pos)));
		}
		else if(address[1].s_addr == ring_pos->addr.s_addr && pn == 0)
		{
			next_p = gnn(gnn(ring_pos));

		}
		else if(address[2].s_addr == ring_pos->addr.s_addr && pn == 0)
		{
			next_p = gnn(ring_pos);
		}
		else{
            temp = temp->next; 
			continue;
        }
		sendaddr.sin_addr = next_p->addr;
		sendto(grepfd, &message, sizeof(mess_s), 0,(struct sockaddr *) &sendaddr, sizeof(sendaddr));
		temp = temp->next;
	}
}
struct in_addr get_addr(int key){
    int check = key % M_POW_VAL;
    ring_n* temp;

    for(temp = myring; temp != NULL; temp = temp->next)
    {
        if(temp->value > check)
        {
            return temp->addr;
        }
    }
    return myring->addr;
}
void get_rep_addr(int key, struct in_addr* input){
    int check = key % M_POW_VAL;
    ring_n* temp;

    for(temp = myring; temp != NULL; temp = temp->next)
    {
        if(temp->value > check)
        {
            input[0] = temp->addr;
            break;
        }
    }

    if(temp == NULL){
        temp = myring;
        input[0] = myring->addr;
    }
    
    if(temp->next != NULL){
        input[1] = temp->next->addr;
        temp = temp->next;
    }
    else{
        temp = myring;
        input[1] = myring->addr;
    }

    if(temp->next != NULL){
        input[2] = temp->next->addr;
    }
    else{
        input[2] = myring->addr;
    }

    return;
}
int get_hashed_id(){
    unsigned char hash[20];
    SHA1(inet_ntoa(myaddr.sin_addr),strlen(inet_ntoa(myaddr.sin_addr)),hash);
    return (int)(( (int) 0 ) | hash[19]);
}
void startTime(Timer* timer) {
    gettimeofday(&(timer->startTime), NULL);
}
void stopTime(Timer* timer) {
    gettimeofday(&(timer->endTime), NULL);
}
float elapsedTime(Timer timer) {
    return ((float) ((timer.endTime.tv_sec - timer.startTime.tv_sec) \
                + (timer.endTime.tv_usec - timer.startTime.tv_usec)/1.0e6));
}
void test_lookup(){
    int i;
    int random;
    FILE* fp = fopen("test_lookup.csv","w");
    Timer timer;
    for(i = 0; i < NUMTEST; i++){
        random = rand() % 1000001;
        sem_wait(&test_lock);
        startTime(&timer);
        lookup(random);
        sem_wait(&test_lock);
        stopTime(&timer); fprintf(fp,"%f\n", elapsedTime(timer));
        sem_post(&test_lock);
    }
    fclose(fp);
}
void test_insert(){
    int i;
    int random;
    FILE* fp = fopen("test_insert.csv","w");
    Timer timer;
    for(i = 0; i < NUMTEST; i++){
        random = rand() % 1000001;
        char buf[20];
        memset(buf,'\0',20);
        sprintf(buf, "%d", random);
        sem_wait(&test_lock);
        startTime(&timer);
        insert(random,buf);
        sem_wait(&test_lock);
        stopTime(&timer); fprintf(fp,"%f\n", elapsedTime(timer));
        sem_post(&test_lock);
    }
    fclose(fp);
}
void dump_keys(){
    FILE* fp = fopen("keys.dmp","w");
    keyval* current = mykv;
    while(current != NULL){
        fprintf(fp,"%d,%s\n",current->key,current->value);
        current = current->next;
    }
    fclose(fp);
}
ring_n* gnn(ring_n* current){
	if(current->next != NULL)
	{
		return current->next;
	}
	return myring;
}
ring_n* gpn(ring_n* current){
	if(current->prev != NULL)
	{
		return current->prev;
	}
	return get_last();
}
ring_n* get_last(){
	ring_n* temp = myring;
	ring_n* last = NULL;
	while(temp != NULL)
	{
		last = temp;
		temp = temp->next;
	}
	return last;
}




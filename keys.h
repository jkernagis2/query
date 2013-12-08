//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              keys.h                                  //
//  Includes and declarations for the keys.c file.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//(insert, lookup, update, delete)
#ifndef KEYS_H
#define KEYS_H

#include "shared.h"
#include "init.h"
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/evp.h>
#define M_POW_VAL 256

extern keyval* mykv;
extern ring_n* myring;
extern my_id;
extern struct sockaddr_in myaddr;
extern int grepfd;
extern sem_t test_lock;

void local_insert(int key, char* val, int type);

char* local_lookup(int key);

void local_update(int key, char* new_val, int type);

void local_delete(int key, int type);

void local_show();

void insert(int key, char* val);

void lookup(int key);

void update(int key, char* new_val);

void delete_k(int key);

void shift_keys();
void move_keys();
void fix_keys();

ring_n* gnn(ring_n* ring_pos);
ring_n* gpn(ring_n* current);
ring_n* get_last();

void leave_shift(ring_n* ring_pos);
void crash_shift(ring_n* ring_pos, int pn);
struct in_addr get_addr(int key);
void get_rep_addr(int key, struct in_addr* input);
int get_hashed_id();

void startTime(Timer* timer);
void stopTime(Timer* timer);
float elapsedTime(Timer timer);

void test_lookup();
void test_insert();
void dump_keys();

#endif // KEYS_H

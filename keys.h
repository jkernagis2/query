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

void local_insert(int key, char* val);

char* local_lookup(int key);

void local_update(int key, char* new_val);

void local_delete(int key);

void local_show();

void insert(int key, char* val);

void lookup(int key);

void update(int key, char* new_val);

void delete_k(int key);

void shift_keys();
void move_keys();

struct in_addr get_addr(int key);
int get_hashed_id();

void startTime(Timer* timer);
void stopTime(Timer* timer);
float elapsedTime(Timer timer);

void test_lookup();
void test_insert();
void dump_keys();

#endif // KEYS_H

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
#include <stdlib.h>
#include <stdio.h>
#include "init.h"
#include <openssl/evp.h>
#define M_POW_VAL 256

extern keyval* mykv;
extern ring_n* myring;
extern my_id;
extern struct sockaddr_in myaddr;
extern int grepfd;


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
unsigned int the_hash(const char *mode, const char* dataToHash, size_t dataSize, unsigned char* outHashed);
int get_hashed_id();

#endif // KEYS_H

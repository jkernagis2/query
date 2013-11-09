//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              keys.h                                  //
//  Includes and declarations for the keys.c file.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
(insert, lookup, update, delete)
#ifndef KEYS_H
#define KEYS_H

#include "shared.h"
#include <stdlib.h>
#include <stdio.h>

extern keyval* mykv;
extern ring* myring;

void local_insert(int key, char* val);

char* local_lookup(int key);

void local_update(int key, char* new_val);

void local_delete(int key);

void local_show();

#endif // KEYS_H

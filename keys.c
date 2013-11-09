//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              keys.c                                  //
//  Function definitions for the handling of ring based key-value store.//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "keys.h"

void local_insert(int key, char* value)
{
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
    }
}

char* local_lookup(int key)
{
    keyval* current = mykv;
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

void local_update(int key, char* new_val)
{
    keyval* current = mykv;
    while(current != NULL){
        if(current->key == key){
            free(current->value);
            current->value = malloc(strlen(new_val));
            strcpy(current->value,new_val);
            return;
        }
        else{
            current = current->next;
        }
    }
    return;
}

void local_delete(int key)
{
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
            free(current->value);
            free(current);
            return;
        }
        else{
            current = current->next;
        }
    }
    return;
}

void local_show()
{
    keyval* current = mykv;

    // Print local keys stored here
    printf("Local Keys:\n");
    while(current != NULL){
        printf("%d, ",current->key);
        current = current->next;
    }
    printf("\n\nLocal Membership List:\n");

    ring* current_r = myring;
    while(current_r != NULL){
        printf("%d, ", current_r->value);
        current_r = current_r->next;
    }
    printf("\n\n");

}

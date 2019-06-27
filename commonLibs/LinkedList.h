
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *      Gabriela Ferreira Jorge       RA: 12228441
 *		Rodrigo da Silva Cardoso      RA: 16430126
 *
 *  Biblioteca de Recursos Referentes a 
 *  manipulacao de uma Lista Ligada
 */

#ifndef LinkedList_Def
#define LinkedList_Def

#define KEY_LEN 9
#define DATA_LEN 80

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef	struct LinkedListNode {
    char*   key;
    int     length;
    void*   data;
    struct LinkedListNode* next;
} LinkedListNode;

typedef	struct LinkedListHead {
    int size;
    struct LinkedListNode* initialNode;
} LinkedListHead;

typedef struct LinkedListOps {
    void (*initHead) (struct LinkedListHead**);
    int* (*add) (struct LinkedListHead*, char* key, void* data);
    int* (*remove) (struct LinkedListHead*, char* key);
    struct LinkedListNode* (*get) (struct LinkedListHead*, char* key);
    struct LinkedListNode* (*getFirst) (struct LinkedListHead*);
    void (*destroyHead) (struct LinkedListHead*);
    void (*destroyNode) (struct LinkedListNode*);
} LinkedListOps;

void initList(struct LinkedListHead** head);
void addNode(LinkedListHead** head, char* key, int length, void* data);
void  destroyHead(struct LinkedListHead* head);
void  destroyNode(struct LinkedListNode* node);
int  removeNode(struct LinkedListHead* head, char* key);
struct LinkedListNode* getNode(LinkedListHead head, char* key);
struct LinkedListNode* getFirst(struct LinkedListHead* head);
void    printAllKeys(LinkedListHead* head);
void    forEach(LinkedListHead* head, void (*function)(void*));

extern const struct LinkedListOps llOps;
#endif
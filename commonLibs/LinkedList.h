
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca de Recursos Referentes a 
 *  manipulacao de uma Lista Ligada
 */

/*
 * #define KEY_LEN 9    //  Implementar Aonde for Usado
 * #define DATA_LEN 80  //  Implementar Aonde for Usado
 */
#ifndef LinkedList_Def
#define LinkedList_Def

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef	struct LinkedListHead {
    int size;
    LinkedListNode* initialNode;
} LinkedListHead;

typedef	struct LinkedListNode {
    char*   key;
    int     length;
    void*   data;
    LinkedListNode* next;
} LinkedListNode;

typedef struct LinkedListOps {
    int*            (*initHead)     (LinkedListHead*);
    int*            (*initNode)     (LinkedListHead*);
    int*            (*add)          (LinkedListHead*, char* key, void* data);
    int*            (*remove)       (LinkedListHead*, char* key);
    LinkedListNode* (*get)          (LinkedListHead*, char* key);
    LinkedListNode* (*getFirst)     (LinkedListHead*);
    int*            (*destroyHead)  (LinkedListHead*);
    int*            (*destroyNode)  (LinkedListNode*);
} LinkedListOps;

LinkedListHead* initList();
LinkedListNode* initNode(char* key, void* data);
int  destroyHead(LinkedListHead* head);
int  destroyNode(LinkedListNode* node);
int  addNode(LinkedListHead* head, char* key, void* data);
int  removeNode(LinkedListHead* head, char* key);
LinkedListNode* getNode(LinkedListHead* head, char* key);
LinkedListNode* getFirst(LinkedListHead* head);

LinkedListOps llOps = {
    .initHead   =   initList,
    .initNode   =   initNode,
    .add        =   addNode,
    .remove     =   removeNode,
    .get        =   getNode,
    .getFirst   =   getFirst,
    .destroyHead=   destroyHead,
    .destroyNode=   destroyNode
};
#endif
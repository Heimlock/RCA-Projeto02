
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Desenvolvimento dos Recursos Referentes a 
 *  manipulacao de Listas Ligadas
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "LinkedList.h"

LinkedListHead* initList() {
    LinkedListHead* head = (LinkedListHead*) malloc(sizeof(LinkedListHead));
    memset(head, 0, sizeof(LinkedListHead));

    //  Init Vars
    head->size = 0;
    head->initialNode = NULL;

    //  Init Functions
    // head->add = addNode_Macro;
    // head->remove = removeNode_Macro;
    // head->get = getNode_Macro;
    // head->destroy =
}

LinkedListNode* initList(char* key, void* data) {
    LinkedListNode* node = (LinkedListNode*) malloc(sizeof(LinkedListNode));
    memset(node, 0, sizeof(LinkedListNode));

    //  Init Vars
    memcpy(node->key, key, KEY_LEN);
    memcpy(node->data, data, DATA_LEN);
    node->next = NULL;

    //  Init Functions
    // node->destroy =
}

void addNode(LinkedListHead* head, char* key, void* data) {
    LinkedListNode* node, last;

    last = &(head->initialNode);
    node = head->initialNode;
    do {
        if(node == NULL) {
            head->size++;
            last->next = initNode(key, data);
            //  Success
            return;
        } else {
            last = node;
            node = node->next;
        }
    } while(node != NULL);
}

void removeNode(LinkedListHead* head, char* key) {
    LinkedListNode* node, last;
    int running = 1;
    if(head->initialNode == NULL) {
        perror("Initial Node is Null.");
        return;
    } else {
        node = head->initialNode;
        do {
            if(strcmp(node->key, key) == 0) {
                last->next = node->next;
                node->destroy();
                head->size--;
                //  Success
                return;
            } else if (node->next != null) {
                last = node;
                node = node->next;
            } else {
                running = 0;
            }
        } while(running);
        perror("Node not Found");
        return;
    }
}

LinkedListNode* getNode(LinkedListHead* head, char* key) {
    LinkedListNode* node;
    int running = 1;
    if(head->initialNode == NULL) {
        perror("Initial Node is Null.");
        return NULL;
    } else {
        node = head->initialNode;
        do {
            if(strcmp(node->key, key) == 0) {
                return node;
            } else if (node->next != null) {
                node = node->next;
            } else {
                running = 0;
            }
        } while(running);
        perror("Node not Found");
        return NULL;
    }
}

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

/*
 *  Função que Inicia Corretamente um LinkedListHead
 */
LinkedListHead* initList() {
    LinkedListHead* head = (LinkedListHead*) malloc(sizeof(LinkedListHead));
    memset(head, 0, sizeof(LinkedListHead));

    //  Init Vars
    head->size = 0;
    head->initialNode = NULL;
}

/*
 *  Função que Inicia Corretamente um LinkedListNode
 */
LinkedListNode* initNode(char* key, void* data) {
    LinkedListNode* node = (LinkedListNode*) malloc(sizeof(LinkedListNode));
    memset(node, 0, sizeof(LinkedListNode));

    //  Init Vars
    memcpy(node->key, key, KEY_LEN);
    memcpy(node->data, data, DATA_LEN);
    node->next = NULL;
}

/*
 *  Função que Desaloca Corretamente um LinkedListHead
 */
LinkedListHead* destroyHead(LinkedListHead* head) {
    LinkedListNode* node;
    node = getFirst(head);
    do {
        removeNode(head, node->key);
        destroyNode(node);
        node = getFirst(head);
    } while(node != NULL);
}

/*
 *  Função que Desaloca Corretamente um LinkedListNode
 */
LinkedListNode* destroyNode(LinkedListNode* node) {
    free(node->data);
    free(node);
}

/*
 *  Função que Adiciona um Novo Node em uma Lista Ligada
 *  Argumentos:
 *      @head       ==  Header da Lista Ligada
 *      @key        ==  Chave do Dado
 *      @data       ==  Dado
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Successo
 */
int addNode(LinkedListHead* head, char* key, void* data) {
    LinkedListNode* node, last;
    last = &(head->initialNode);
    node = head->initialNode;
    do {
        if(node == NULL) {
            head->size++;
            last->next = initNode(key, data);
            return 0;
        } else {
            last = node;
            node = node->next;
        }
    } while(node != NULL);
    return -1;
}

/*
 *  Função que Remove um Node de uma Lista Ligada
 *  Argumentos:
 *      @head       ==  Header da Lista Ligada
 *      @key        ==  Chave do Dado
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Successo
 */
int removeNode(LinkedListHead* head, char* key) {
    LinkedListNode* node, last;
    int running = 1;
    if(head->initialNode == NULL) {
        perror("[removeNode] | Initial Node is Null.");
        return -1;
    } else {
        node = head->initialNode;
        do {
            if(strcmp(node->key, key) == 0) {
                last->next = node->next;
                node->destroy();
                head->size--;
                //  Success
                return 0;
            } else if (node->next != null) {
                last = node;
                node = node->next;
            } else {
                running = 0;
            }
        } while(running);
        perror("[removeNode] | Node not Found");
        return -2;
    }
}

/*
 *  Função que Retorna um Node especifico de uma Lista Ligada
 *  Argumentos:
 *      @head       ==  Header da Lista Ligada
 *      @key        ==  Chave do Dado
 *  Retornos:
 *      Null        ==  Erro
 *      Node*       ==  LinkedListNode procurado
 */
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

/*
 *  Função que Retorna o Primeiro Node de uma Lista Ligada
 *  Argumentos:
 *      @head       ==  Header da Lista Ligada
 *  Retornos:
 *      Null        ==  Erro
 *      Node*       ==  LinkedListNode procurado
 */
LinkedListNode* getFirst(LinkedListHead* head) {
    LinkedListNode* node;
    if(head->initialNode == NULL) {
        perror("Initial Node is Null.");
        return NULL;
    } else {
        return head->initialNode;
    }
}

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

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "LinkedList.h"
#include "CustomStreams.h"

const struct LinkedListOps llOps = {
    .initHead   =   initList,
    .add        =   addNode,
    .remove     =   removeNode,
    .get        =   getNode,
    .getFirst   =   getFirst,
    .destroyHead=   destroyHead,
    .destroyNode=   destroyNode
};

/*
 *  Função que Inicia Corretamente um LinkedListHead
 */
void initList(LinkedListHead** head) {
    (*head) = (LinkedListHead*) malloc(sizeof(LinkedListHead));
    memset((*head), 0, sizeof(LinkedListHead));

    //  Init Vars
    (*head)->size = 0;
    (*head)->initialNode = NULL;
}

/*
 *  Função que Inicia Corretamente um LinkedListNode
 */
void initNode(char* key, void* data, struct LinkedListNode** node) {
    (*node) = (LinkedListNode*) malloc(sizeof(LinkedListNode));
    memset((*node), 0, sizeof(LinkedListNode));

    //  Init Vars
    memcpy((*node)->key, key, KEY_LEN);
    memcpy((*node)->data, data, DATA_LEN);
    (*node)->next = NULL;
}

/*
 *  Função que Desaloca Corretamente um LinkedListHead
 */
void destroyHead(LinkedListHead* head) {
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
void destroyNode(LinkedListNode* node) {
    free(node->data);
    free(node);
}

/*
 *  Função que Adiciona um Novo Node em uma Lista Ligada
 *  Argumentos:
 *      @head       ==  Header da Lista Ligada
 *      @key        ==  Chave do Dado
 *      @length     ==  Tamanho do Dado
 *      @data       ==  Ponteiro do Dado
 */
void addNode(LinkedListHead** head, char* key, int length, void* data) {
    LinkedListNode** actual;
    Log.debug(getpid(), "addNode Init\n");

    (*head)->size++;
	actual = &((*head)->initialNode);
	while (*actual) {
		actual = &(*actual)->next;
    }

	(*actual) = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    if(*actual != NULL) {
        (*actual)->key = (char *)malloc((KEY_LEN + 1) * sizeof(char));
        memcpy((*actual)->key, key, KEY_LEN);
        (*actual)->key[KEY_LEN] = '\0';
        (*actual)->length = length;

        (*actual)->data = malloc(length);
        memcpy((*actual)->data, data, length);

        (*actual)->next = NULL;

        Log.debug(getpid(), "Node Added!\n");
        Log.debug(getpid(), "Head.Size: %d\n", (*head)->size);
        Log.debug(getpid(), "Key......: %s\n", (*actual)->key);
        Log.debug(getpid(), "Length...: %d\n", (*actual)->length);
    } else {
        Log.error(getpid(), "Cannot Allocate %d Bytes (addNode).\n", (KEY_LEN + 1) * sizeof(char));
    }
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
int removeNode(LinkedListHead *head, char* key) {
    LinkedListNode**  actual;
    LinkedListNode* toDelete;
    int running = 1;

    if(head->initialNode == NULL) {
        Log.error(getpid(), "Initial Node is Null.\n");
        return -1;
    } else {
        actual = &head->initialNode;
        do {
            if(strcmp((*actual)->key, key) == 0) {
                toDelete = (*actual);
                (*actual) = (*actual)->next;
                destroyNode(toDelete);
                head->size--;
                //  Success
                return 0;
            }
            if ((*actual)->next != NULL) {
                actual = &(*actual)->next;
            } else {
                running = 0;
            }
        } while(running);
        Log.error(getpid(), "Node not Found");
        return -2;
    }
}

/*
 *  Função que Itera a Lista executando um Procedimento sobre cada Node
 *  Argumentos:
 *      @head       ==  Header da Lista Ligada
 *      @function   ==  Ponteiro do Procedimento
 */
void    forEach(LinkedListHead* head, void (*function)(void*)) {
    LinkedListNode** actual;
    if(head->initialNode != NULL || head->size != 0) {
        actual = &head->initialNode;
        while (*actual) {
            function((void*)*actual);
            actual = &(*actual)->next;
        }
    } else {
        Log.error(getpid(), "Head is Empty\n");
    }
}

/*
 *  Função que Compara 2 Chaves
 *  Argumentos:
 *      @keyOne     ==  Chave 1
 *      @keyTwo     ==  Chave 2
 *  Retornos:
 *      1           ==  Iguais
 *      0           ==  Diferentes
 */
int compareKeys(char* keyOne, char* keyTwo) {
    int key_1, key_2;
    Log.debug(getpid(), "Comparing: %s & %s\n", keyOne, keyTwo);

    for(int i = 0; i < KEY_LEN; i++) {
        key_1 = (int)keyOne[i];
        key_2 = (int)keyTwo[i];
        if(key_1 != key_2) {
            return 0;
        }
    }
    return 1;
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
LinkedListNode* getNode(LinkedListHead head, char* key) {
    LinkedListNode** actual;
    int equals = 0;

    Log.debug(getpid(), "getNode Init\n");

    if(head.initialNode != NULL || head.size != 0) {
        actual = &head.initialNode;
        while (*actual) {
            equals = compareKeys((*actual)->key, key);
            if(equals) {
                return (*actual);
            }
            actual = &(*actual)->next;
        }
        Log.error(getpid(), "Node not Found\n");
    }
    return NULL;
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
    if(head->initialNode == NULL) {
        Log.error(getpid(), "Initial Node is Null.\n");
        return NULL;
    } else {
        return head->initialNode;
    }
}

/*
 *  Função que Improme a Key de todos os Nodes de uma Lista Ligada
 *  Argumentos:
 *      @head       ==  Header da Lista Ligada
 */
void    printAllKeys(LinkedListHead* head) {
    LinkedListNode** actual;
    int nodeCount = 0;

    if(head->initialNode != NULL || head->size != 0) {
        actual = &head->initialNode;
        while (*actual) {
            Log.plain("[%.4d] | Key: %s\n", nodeCount++, (*actual)->key);
            actual = &(*actual)->next;
        }
    } else {
        Log.error(getpid(), "Head is Empty\n");
    }
}

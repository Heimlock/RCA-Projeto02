
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

    (*head)->size++;
	actual = &((*head)->initialNode);
	while (*actual) {
		actual = &(*actual)->next;
    }

	(*actual) = (LinkedListNode *)malloc(sizeof(LinkedListNode));
	(*actual)->key = (char *)malloc((KEY_LEN + 1) * sizeof(char));
    memcpy((*actual)->key, key, KEY_LEN);
    (*actual)->key[KEY_LEN] = '\0';
    (*actual)->length = length;

	(*actual)->data = malloc(length);
    memcpy((*actual)->data, data, length);

	(*actual)->next = NULL;

    // #ifdef  DEBUG
        fprintf(stdout, "[addNode] | Node Added!\n");
        fprintf(stdout, "[addNode] | Key...: %s\n", (*actual)->key);
        fprintf(stdout, "[addNode] | Length: %d\n", (*actual)->length);
        fflush(stdout);
    // #endif
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
    LinkedListNode*  node;
    LinkedListNode** last;
    int running = 1;
    if(head->initialNode == NULL) {
        perror("[removeNode] | Initial Node is Null.");
        return -1;
    } else {
        node = head->initialNode;
        last = &(head->initialNode);
        do {
            if(strcmp(node->key, key) == 0) {
                (*last)->next = node->next;
                llOps.destroyNode(node);
                head->size--;
                //  Success
                return 0;
            } else if (node->next != NULL) {
                (*last) = node;
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
LinkedListNode* getNode(LinkedListHead head, char* key) {
    LinkedListNode** actual;
    int equals = 0;

    fprintf(stdout, "[getNode] | Init\n");
    fflush(stdout);

    if(head.initialNode != NULL || head.size != 0) {
        actual = &head.initialNode;
        while (*actual) {
            equals = compareKeys((*actual)->key, key);
            if(equals) {
                return (*actual);
            }
            actual = &(*actual)->next;
        }
        fprintf(stderr, "[getNode] | Node not Found\n");
        fflush(stderr);
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
        perror("Initial Node is Null.");
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
            fprintf(stdout, "[%.4d] | Key: %s\n", nodeCount++, (*actual)->key);
            fflush(stdout);
            actual = &(*actual)->next;
        }
    } else {
        fprintf(stderr, "Head is Empty\n");
        fflush(stderr);
    }
}

/*
 *  Função que Itera a Lista executando um Procedimento sobre cada Node
 *  Argumentos:
 *      @head       ==  Header da Lista Ligada
 *      @function   ==  Ponteiro do Procedimento
 */
void    forEach(LinkedListHead* head, void (*function)(LinkedListNode*)) {
    LinkedListNode** actual;
    if(head->initialNode != NULL || head->size != 0) {
        actual = &head->initialNode;
        while (*actual) {
            function(*actual);
            actual = &(*actual)->next;
        }
    } else {
        fprintf(stderr, "Head is Empty\n");
        fflush(stderr);
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
    #ifdef  DEBUG
        fprintf(stdout, "[compareKeys] | Comparing: %s & %s\n", keyOne, keyTwo);
        fflush(stdout);
    #endif

    int key_1, key_2;

    for(int i = 0; i < KEY_LEN; i++) {
        key_1 = (int)keyOne[i];
        key_2 = (int)keyTwo[i];
        if(key_1 != key_2) {
            return 0;
        }
    }
    return 1;
}

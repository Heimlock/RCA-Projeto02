
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Biblioteca de Recursos Referentes ao Cliente
 */

#include "../commonLibs/Communication.h"
#include "../commonLibs/LinkedList.h"
#include "../commonLibs/ThreadManager.h"

#ifndef VARS_INIT
#define VARS_INIT
    char* ip;
    int port;
    UserState state;
    char* userId;
    int  threadCount;
    int allowNewConnections;

    struct LinkedListHead   *messages;
    struct LinkedListHead   *contacts;
    struct LinkedListHead   *groups;

    pthread_mutex_t  *mutex_list_messages;
    pthread_mutex_t  *mutex_ServerSocket;
    pthread_mutex_t  *mutex_RemoteSocket;

    struct commFacade_t local;
    struct commFacade_t remote;
#else
    extern char* ip;
    extern int port;
    extern UserState state;
    extern char* userId;
    extern int threadCount;
    extern int allowNewConnections;

    extern struct LinkedListHead   *messages;
    extern struct LinkedListHead   *contacts;
    extern struct LinkedListHead   *groups;

    extern pthread_mutex_t  *mutex_list_messages;
    extern pthread_mutex_t  *mutex_ServerSocket;
    extern pthread_mutex_t  *mutex_RemoteSocket;

    extern struct commFacade_t local;
    extern struct commFacade_t remote;
#endif

void    serverThread();
void    logIn();
void    logOut();
User_t* requestClient(char*);

void    newReceiver();
void    attendClientPeer(void *arg);
void    sendMessagePeer(void* vars);// (struct sockaddr_in, struct Message_t);
void 	sendFilePeer(void* vars);//(struct sockaddr_in, struct File_t);
void    printGroup(LinkedListNode* group);

int     canContinue();
void    initSharedData();

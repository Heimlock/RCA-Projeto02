
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
#include "../commonLibs/IPC.h"

#ifndef VARS_INIT
#define VARS_INIT
    char* ip;
    int port;
    UserState state;
    char* userId;
    int  threadCount;
    int allowNewConnections;

    int basePort;

    struct shMemControl_t   shMem_messages;
    struct LinkedListHead   *messages;
    struct LinkedListHead   *contacts;
    struct LinkedListHead   *groups;

    struct semControl_t     sem_list_messages;
    struct semControl_t     sem_CanContinue;
    struct semControl_t     sem_ServerSocket;
    struct pthread_mutex_t  *mutex_RemoteSocket;

    struct commFacade_t local;
    struct commFacade_t remote;
#else
    extern char* ip;
    extern int port;
    extern UserState state;
    extern char* userId;
    extern int threadCount;
    extern int allowNewConnections;

    extern int basePort;

    extern struct shMemControl_t   shMem_messages;
    extern struct LinkedListHead   *messages;
    extern struct LinkedListHead   *contacts;
    extern struct LinkedListHead   *groups;

    extern struct semControl_t     sem_list_messages;
    extern struct semControl_t     sem_CanContinue;
    extern struct semControl_t     sem_ServerSocket;
    extern struct pthread_mutex_t  *mutex_RemoteSocket;

    extern struct commFacade_t local;
    extern struct commFacade_t remote;
#endif

void    serverThread();
void    logIn();
void    logOut();
User_t* requestClient(char*);

void    newReceiver();
void    attendClientPeer(void *arg);
void 	sendMessagePeer(char* peerId, struct Message_t message); //(void* vars);
void 	sendFilePeer(void* vars);//(struct sockaddr_in, struct File_t);
void    printGroup(LinkedListNode* group);

int     canContinue();
void    initSharedData();

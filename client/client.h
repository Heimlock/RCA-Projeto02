
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
    char userId[10];
    int  threadCount;

    struct LinkedListHead   *messages;
    struct LinkedListHead   *contacts;
    struct LinkedListHead   *groups;
    pthread_mutex_t  *mutex_list_messages;
#else
    extern char* ip;
    extern int port;
    extern UserState state;
    extern char userId[10];
    extern int threadCount;

    extern struct LinkedListHead   *messages;
    extern struct LinkedListHead   *contacts;
    extern struct LinkedListHead   *groups;
    extern pthread_mutex_t  *mutex_list_messages;
#endif

void    connectToServer(char *, int);
void    logIn();
void    logOut();
User_t* requestClient(char*);

void    newReceiver();
void    *receiveFromPeer(void *);
void    sendMessagePeer(struct sockaddr_in, struct Message_t);
void    sendFilePeer(struct sockaddr_in, struct File_t);

void    initSharedData();

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
    struct commFacade_t     localPeer;
    struct commFacade_t     remotePeer;

    struct LinkedListHead   *messages;
    pthread_mutex_t  *mutex_list_messages;
    pthread_mutex_t  *mutex_remote;
#else
    extern struct commFacade_t      localPeer;
    extern struct commFacade_t      remotePeer;

    extern struct LinkedListHead    messages;
    extern pthread_mutex_t  *mutex_list_messages;
    extern pthread_mutex_t  *mutex_remote;
    extern int whatsappCount;
#endif

void    connectToServer(char *, int);
void    logIn(char* ip, int port, char* id);
void    logOut(char* ip, int port, char* id);
User_t* requestClient(char* ip, int port, char* peerId);

void    connectToClient(struct User_t *, char *);
void    *whatsapp(void *);
void    receiveFromClient(struct commFacade_t);
void    sendToClient(struct commFacade_t, char *);

void    readMessage();
void    initSharedData();
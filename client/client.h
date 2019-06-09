
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
    struct commFacade_t     localSend;
    struct commFacade_t     remoteSend;
    struct LinkedListHead   messages;
    pthread_mutex_t  *mutex_list_messages;
    pthread_mutex_t  *mutex_remote_Send;
    int whatsappCount;

    struct commFacade_t     localRec;
    struct commFacade_t     remoteRec;
    int    allowNewConnections;
    int    childCount;
    pthread_mutex_t  *mutex_remote_Rec;

#else
    extern struct commFacade_t      localSend;
    extern struct commFacade_t      remoteSend;
    extern struct LinkedListHead    messages;
    extern pthread_mutex_t  *mutex_list_messages;
    extern pthread_mutex_t  *mutex_remote;
    extern int whatsappCount;

    extern struct commFacade_t      localRec;
    extern struct commFacade_t      remoteRec;
    extern int    allowNewConnections;
    extern int    childCount;
    extern pthread_mutex_t  *mutex_remote_Rec;
#endif

void    connectToServer(char *);
void    logIn();
User_t* requestClient();
void    logOut();

void    connectToClient(struct User_t *);
void    *whatsapp(void *);
void    receiveFromCLient(struct commFacade_t);
void    sendToClient(struct commFacade_t);

void    readMessage();
void    initSharedData();
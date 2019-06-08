
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
    struct commFacade_t     local;
    struct commFacade_t     remote;
    struct LinkedListHead   messages;
    pthread_mutex_t  *mutex_list_messages;
    pthread_mutex_t  *mutex_remote;
    int whatsappCount;

#else
    extern struct commFacade_t      local;
    extern struct commFacade_t      remote;
    extern struct LinkedListHead    messages;
    extern pthread_mutex_t  *mutex_list_messages;
    extern pthread_mutex_t  *mutex_remote;
    extern int whatsappCount;
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
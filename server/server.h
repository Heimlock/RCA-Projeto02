
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *      Gabriela Ferreira Jorge       RA: 12228441
 *		Rodrigo da Silva Cardoso      RA: 16430126
 *
 *	 Biblioteca de Recursos Referentes ao Servidor
 */

 #include "../commonLibs/Communication.h"
 #include "../commonLibs/LinkedList.h"
 #include "../commonLibs/ThreadManager.h"
 
 #ifndef VARS_INIT
 #define VARS_INIT
    struct commFacade_t  local;
    struct commFacade_t  remote;
    struct LinkedListHead  *users;
    pthread_mutex_t  *mutex_list_users;
    pthread_mutex_t  *mutex_remote;
    int    acceptNewConnections;
    int    allowNewConnections;
    int    childCount;

 #else
    extern struct   commFacade_t        local;
    extern struct   commFacade_t        remote;
    extern struct   LinkedListHead        *users;
    extern pthread_mutex_t *mutex_list_users;
    extern pthread_mutex_t *mutex_remote;
    extern int      acceptNewConnections;
    extern int      allowNewConnections;
    extern int      childCount;
#endif

 void    newConnection();
 void   *attendClient(void *);
 int    canContinue();
 void   exitServer();
 void   initSharedData();

 void   logIn(struct commFacade_t, struct SPDT_Command *);
 void   logOut(struct commFacade_t, struct SPDT_Command *);
 void   requestClient(struct commFacade_t, struct SPDT_Command *);

/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Biblioteca de Recursos Referentes ao Servidor
 */

 #include "../commonLibs/Communication.h"
 #include "../commonLibs/LinkedList.h"
 #include "../commonLibs/ThreadManager.h"
 
 #ifndef VARS_INIT
 #define VARS_INIT
    struct commFacade_t  commData;
    struct LinkedListHead  *users;
    int    acceptNewConnections;
    int    allowNewConnections;
    int    childCount;

 #else
    extern struct   commFacade_t        commData;
    extern struct   LinkedListHead        *users;
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
 void   requestUser(struct commFacade_t, struct SPDT_Command *);
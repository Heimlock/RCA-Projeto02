
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Biblioteca de Recursos Referentes a Comunicacao TCP
 */

#ifndef Tcp_Def
#define Tcp_Def

#define CONNECTION_QUEUE    20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct commFacade_t {
    int     socketDesc;
    struct  sockaddr_in  socketAddr;
} commFacade_t;

typedef struct commOps_t {
    int	    (*initServer)   (commFacade_t*, int);
    void    (*close)        (commFacade_t*);

    size_t  (*send)         (commFacade_t*, void *, size_t);
    size_t  (*receive)      (commFacade_t*, void **, size_t);

    int     (*accept)       (commFacade_t*, commFacade_t*);
    int     (*connect)      (commFacade_t*, commFacade_t*, int, char*, int);
} commOps_t;


int     init_Server(commFacade_t* commData, int port );
void    close_Socket(commFacade_t* commData);
int     sendData(commFacade_t* commData, void *data, size_t size);
int     receiveData(commFacade_t* commData, void **data, size_t size);
int	    acceptConnection(commFacade_t* local, commFacade_t* remote);
int     connectRemote(commFacade_t* local, commFacade_t* remote, int localPort, char *remoteAddr, int remotePort);

extern const struct commOps_t commOps;

#pragma GCC diagnostic ignored "-Wunused-variable"
#ifdef  DEBUG
    const static int enable = 1;
#else
    const static int enable = 0;
#endif
#endif

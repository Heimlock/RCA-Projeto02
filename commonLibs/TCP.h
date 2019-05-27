
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
    int     localSocketDesc;
    int     remoteSocketDesc;
    struct  sockaddr_in  local;
    struct  sockaddr_in  remote;
} commFacade_t;

typedef struct commOps_t {
    int     (*init)         (commFacade_t*, int);
    int     (*close)        (commFacade_t*);
    int     (*closeRemote)  (commFacade_t*);

    size_t  (*send)         (commFacade_t*, void *, size_t);
    size_t  (*receive)      (commFacade_t*, void *, size_t);

    int     (*accept)       (commFacade_t*);
    int     (*connect)      (commFacade_t*, char *, int);
} commOps_t;


int     init_Socket(commFacade_t* commData, int port );
void    close_Socket(commFacade_t* commData);
int     close_Remote(commFacade_t* commData);
size_t  sendData(commFacade_t* commData, void *data, size_t size);
size_t  receiveData(commFacade_t* commData, void *data, size_t size);
int	    acceptConnection(commFacade_t* commData);
int     connectRemote(commFacade_t* commData, char *addr, int port);

commOps_t commOps = {
    .init       =init_Socket,
    .close      =close_Socket,
    .closeRemote=close_Remote,
    .send       =sendData,
    .receive    =receiveData,
    .accept     =acceptConnection,
    .connect    =connectRemote,
}
#endif


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

    // void    (*init)         (int);
    // void    (*close)        (void);
    // void    (*closeRemote)  (void);

    // size_t  (*send)         (void *, size_t);
    // size_t  (*receive)      (void *, size_t);

    // void    (*accept)       (void);
    // void    (*connect)      (char *, int);
} commFacade_t;

void    init_Socket(int *localSocketDesc, struct sockaddr_in *local, int port );
void    close_Socket(int *localSocketDesc);
size_t  sendData(int *remoteSocketDesc, void *response, size_t size);
size_t  receiveData( int *remoteSocketDesc, void *command, size_t size);
void	acceptConnection( struct  sockaddr_in  *remote, int *remoteSocketDesc, int *localSocketDesc);
void    close_Remote( int *remoteSocketDesc );
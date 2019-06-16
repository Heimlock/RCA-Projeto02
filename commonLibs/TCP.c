
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Desenvolvimento dos Recursos Referentes a Comunicacao TCP
 */

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#include "./TCP.h"
#include "./CustomStreams.h"

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

const struct commOps_t commOps = {
    .initServer =init_Server,
    .close      =close_Socket,
    .send       =sendData,
    .receive    =receiveData,
    .accept     =acceptConnection,
    .connect    =connectRemote,
};

/*
 *  Função que Inicializa um Socket Servidor
 *  Argumentos:
 *      @port       ==  Numero da Porta
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int     init_Server(commFacade_t* commData, int port ) {
    if (((commData->socketDesc) = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[init_Socket] | socket()");
        return -1;
    }
    if (setsockopt((commData->socketDesc), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("[init_Socket] | setsockopt(SO_REUSEADDR) failed");
        return -2;
    }
    commData->socketAddr.sin_family      = AF_INET;
    commData->socketAddr.sin_port        = htons(port);
    commData->socketAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind((commData->socketDesc), (struct sockaddr*)&(commData->socketAddr), sizeof(commData->socketAddr)) < 0) {
        perror("[init_Socket] | bind()");
        return -3;
    }
    if (listen((commData->socketDesc), CONNECTION_QUEUE) == -1) {
        perror("[init_Socket] | listen()");
        return -4;
    }
   Log.fine(getpid(), "Server Socket at %s : %d was Initialized\n", inet_ntoa(commData->socketAddr.sin_addr), ntohs(commData->socketAddr.sin_port) );
   return 0;
}

/*
 *  Função que Finaliza a Comunicação via Socket
 */
void    close_Socket(commFacade_t* commData) {
    Log.fine(getpid(), "Socket at Port %d has been Terminated!\n", ntohs(commData->socketAddr.sin_port));
    close(commData->socketDesc);
}

/*
 *  Função que envia os dados via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *      @data       ==  Dado
 *      @size       ==  Tamanho do Dado
 *  Retornos:
 *      > 0         ==  Erro
 *      < 0         ==  Numero de Bytes Enviados
 */
int  sendData(commFacade_t* commData, void *data, size_t size) {
    int numbytes = 0;
    //  send: Socket Remoto, data, Tam Esperado, Flags
    numbytes = send((commData->socketDesc), data, size, 0);
    if(numbytes < 0) {
        perror("send()");
        return -1;
    }
    return numbytes;
}

/*
 *  Função que recebe os dados via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *      @data       ==  Ponteiro do Dado
 *      @size       ==  Tamanho do Dado
 *  Retornos:
 *      -1          ==  Erro
 *      < 0         ==  Numero de Bytes Recebidos
 */
int  receiveData(commFacade_t* commData, void **data, size_t size) {
    int numbytes;
    //  recv: Socket Remoto, data, Tam Esperado, Flags
    if(-1 == (numbytes = recv((commData->socketDesc), *data, size, 0))) {
        perror("recv()");
        return -1;
    }
    return numbytes;
}

/*
 *  Função que aceita uma conexão TCP
 *  Argumentos:
 *      @local      ==  Local Communication Facade
 *      @remote     ==  Remote Communication Facade
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int     acceptConnection(commFacade_t* local, commFacade_t* remote) {
    socklen_t remoteAddrlen = sizeof(remote->socketAddr);
    if (((remote->socketDesc) = accept((local->socketDesc), (struct sockaddr *)&(remote->socketAddr), &remoteAddrlen)) == -1) {
        perror("accept()");
        return -1;
    }

    Log.fine(getpid(), "\n\n========================================\n");
    Log.fine(getpid(), "Connection Accepted\n");
    Log.fine(getpid(), "Connected with %s : %d\n", inet_ntoa(remote->socketAddr.sin_addr), ntohs(remote->socketAddr.sin_port) );
    return 0;
}

/*
 *  Função que realiza a conexão TCP com o servidor
 *  Argumentos:
 *      @local      ==  Local Communication Facade
 *      @remote     ==  Remote Communication Facade
 *      @localPort  ==  Local Client Port
 *      @remoteAddr ==  Remote Address
 *      @remotePort ==  Remote Port
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int     connectRemote(commFacade_t* local, commFacade_t* remote, int localPort, char *remoteAddr, int remotePort) {
    struct hostent *hostnm;
    int socket_len;

    if (((local->socketDesc) = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[connectRemote] | socket()");
        return -1;
    }
    if (setsockopt((local->socketDesc), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("[connectRemote] | setsockopt(SO_REUSEADDR) failed");
        return -2;
    }
    local->socketAddr.sin_family      = AF_INET;
    local->socketAddr.sin_port        = htons(localPort);
    local->socketAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind((local->socketDesc), (struct sockaddr*)&(local->socketAddr), sizeof(local->socketAddr)) < 0) {
        perror("[connectRemote] | bind()");
        return -3;
    }

    socket_len   =   sizeof(local->socketAddr);
    getsockname(local->socketDesc, (struct sockaddr *)&(local->socketAddr), (socklen_t *)&socket_len);

    Log.fine(getpid(), "Socket at Port %d was Initialized!\n", ntohs(local->socketAddr.sin_port));

    hostnm = gethostbyname(remoteAddr);
    if (hostnm == (struct hostent *) 0) {
        perror("[connectRemote] | gethostbyname()");
        return -4;
    }

    remote->socketAddr.sin_family      = AF_INET;
    remote->socketAddr.sin_port        = htons(remotePort);
    remote->socketAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
    if (connect(local->socketDesc, (struct sockaddr *)&remote->socketAddr, sizeof(remote->socketAddr)) < 0) {
        perror("[connectRemote] | connect()");
        return -5;
    }

    Log.fine(getpid(), "\n\n========================================\n");
    Log.fine(getpid(), "Connected with %s : %d\n", inet_ntoa(remote->socketAddr.sin_addr), ntohs(remote->socketAddr.sin_port) );
    return 0;
}

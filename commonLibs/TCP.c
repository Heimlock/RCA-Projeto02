
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

#include "./TCP.h"

/*
 *  Função que Inicializa a Comunicação via Socket
 *  Argumentos:
 *      @port       ==  Numero da Porta
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int     init_Socket(commFacade_t* commData, int port ) {
   if (((commData->localSocketDesc) = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[init_Socket] | socket()");
        return -1;
   }
   if (setsockopt((commData->localSocketDesc), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("[init_Socket] | setsockopt(SO_REUSEADDR) failed");
        return -2;
   }
   commData->local.sin_family      = AF_INET;
   commData->local.sin_port        = htons(port);
   commData->local.sin_addr.s_addr = INADDR_ANY;
   if (bind((commData->localSocketDesc), (struct sockaddr*)&(commData->local), sizeof(commData->local)) < 0) {
       perror("[init_Socket] | bind()");
       return -3;
   }
    if (listen((commData->localSocketDesc), CONNECTION_QUEUE) == -1) {
        perror("[init_Socket] | listen()");
        return -4;
    }
   fprintf(stdout,"[%d] | Socket at Port %d was Initialized!\n", getpid(), ntohs(commData->local.sin_port));
   fflush(stdout);
   return 0;
}

/*
 *  Função que Finaliza a Comunicação via Socket
 */
void    close_Socket(commFacade_t* commData) {
    close(commData->localSocketDesc);
    fprintf(stdout,"[%d] | Local Socket has been Terminated!\n", getpid());
    fflush(stdout);
}

/*
 *  Função que Finaliza a Comunicação via Socket
 */
void    close_Remote(commFacade_t* commData) {
    close(commData->remoteSocketDesc);
    fprintf(stdout,"[%d] | Remote Socket has been Terminated!\n", getpid());
    fflush(stdout);
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
size_t  sendData(commFacade_t* commData, void *data, size_t size) {
    size_t numbytes;
    //  send: Socket Remoto, data, Tam Esperado, Flags
    if((numbytes = send((commData->remoteSocketDesc), data, size, 0)) < 0) {
        perror("send()");
        return -1;
    }
    return numbytes;
}

/*
 *  Função que recebe os dados via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *      @data       ==  Dado
 *      @size       ==  Tamanho do Dado
 *  Retornos:
 *      > 0         ==  Erro
 *      < 0         ==  Numero de Bytes Recebidos
 */
size_t  receiveData(commFacade_t* commData, void *data, size_t size) {
    size_t numbytes;
    //  recv: Socket Remoto, data, Tam Esperado, Flags
    if((numbytes = recv((commData->remoteSocketDesc), data, size, 0)) < 0) {
        perror("recv()");
        exit(1);
    }
    return numbytes;
}

/*
 *  Função que aceita uma conexão TCP
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int     acceptConnection(commFacade_t* commData) {
    socklen_t remoteAddrlen = sizeof(commData->remote);
    if (((commData->remoteSocketDesc) = accept((commData->localSocketDesc), (struct sockaddr *)&(commData->remote), &remoteAddrlen)) == -1) {
        perror("accept()");
        return -1;
    }
    fprintf( stdout, "\n\n========================================\n");
    fprintf( stdout, "[%d] | Connection Accepted\n", getpid());
    fprintf( stdout, "[%d] | Connected with %s:%d\n", getpid(), inet_ntoa(commData->remote.sin_addr), ntohs(commData->remote.sin_port) );
    fflush(stdout);
    return 0;
}

/*
 *  Função que realiza a conexão TCP com o servidor
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *      @addr       ==  Remote Address
 *      @port       ==  Remote Port
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int     connectRemote(struct commFacade_t *comm_ops, char *addr, int port) {
    struct hostent *hostnm;
    hostnm = gethostbyname(addr);
    if (hostnm == (struct hostent *) 0) {
        perror("[connectRemote] | gethostbyname()");
        return -1;
    }
    comm_ops->remote.sin_family      = AF_INET;
    comm_ops->remote.sin_port        = htons(port);
    comm_ops->remote.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
    if (connect(comm_ops->localSocketDesc, (struct sockaddr *)&comm_ops->remote, sizeof(comm_ops->remote)) < 0) {
        perror("[connectRemote] | connect()");
        return -2;
    }
    printf("[%d] | Connected with Server\n", getpid());
    return 0;
}

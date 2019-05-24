
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
 */
void    init_Socket(int *localSocketDesc, struct sockaddr_in *local, int port ) {
   if (((*localSocketDesc) = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
       perror("socket()");
       exit(1);
   }
   
   local->sin_family      = AF_INET;
   local->sin_port        = htons(port);
   local->sin_addr.s_addr = INADDR_ANY;

   if (bind((*localSocketDesc), (struct sockaddr*)&(*local), sizeof(*local)) < 0) {
       perror("bind()");
       exit(1);
   }

    if (listen((*localSocketDesc), CONNECTION_QUEUE) == -1) {
        perror("listen()");
        exit(1);
    }

   fprintf( stdout,"[%d] | Socket at Port %d was Initialized!\n", getpid(), ntohs(local->sin_port));
   fflush(stdout);
}

/*
 *  Função que Finaliza a Comunicação via Socket
 */
void    close_Socket(int *localSocketDesc) {
    close(*localSocketDesc);
    fprintf( stdout,"[%d] | Local Socket has been Terminated!\n", getpid());
    fflush(stdout);
}

/*
 *  Função que envia os dados via socket
 */
size_t  sendData(int *remoteSocketDesc, void *response, size_t size) {
    size_t numbytes;

    //  send: Socket Remoto, Response, Tam Esperado, Flags
    if((numbytes = send((*remoteSocketDesc), response, size, 0)) < 0) {
        perror("send()");
        exit(1);
    }

    return numbytes;
}

/*
 *  Função que recebe os dados via socket
 */
size_t  receiveData( int *remoteSocketDesc, void *command, size_t size) {
    size_t numbytes;

    //  recv: Socket Remoto, command, Tam Esperado, Flags
    if((numbytes = recv((*remoteSocketDesc), command, size, 0)) < 0) {
        perror("recv()");
        exit(1);
    }

    return numbytes;
}

/*
 *  Função que aceita uma conexão TCP
 */
void	acceptConnection( struct  sockaddr_in  *remote, int *remoteSocketDesc, int *localSocketDesc) {
    socklen_t remoteAddrlen = sizeof(*remote);

    if (((*remoteSocketDesc) = accept((*localSocketDesc), (struct sockaddr *)&(*remote), &remoteAddrlen)) == -1) {
        perror("accept()");
        exit(1);
    }

    fprintf( stdout, "\n\n========================================\n");
    fprintf( stdout, "[%d] | Connection Accepted\n", getpid());
    fprintf( stdout, "[%d] | Connected with %s:%d\n", getpid(), inet_ntoa(remote->sin_addr), ntohs(remote->sin_port) );
    fflush(stdout);
}

/*
 *  Função que Finaliza a Comunicação via Socket
 */
void    close_Remote( int *remoteSocketDesc ) {
    close(*remoteSocketDesc);
    fprintf( stdout,"[%d] | Remote Socket has been Terminated!\n", getpid());
    fflush(stdout);
}

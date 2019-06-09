
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Desenvolvimento Lógico Referente ao Cliente
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./client.h"

int canContinueClient() {
    int rtnValue;
    if(allowNewConnections) {
        rtnValue = 1;
    } else {
        rtnValue = 0;
    }
    return rtnValue;
}

void *attendClientPeer(void *arg) {
	int threadId =  (int)arg;
    struct SPDT_Command  *command;
	struct commFacade_t  innerRemote;

    fprintf(stdout, "[%.4d] | Attend Peer Init\n", threadId);
    fflush(stdout);

	memcpy(&innerRemote, &remoteRec, sizeof(commFacade_t));
    //mutexUnlock(mutex_remote_Rec);


    //Receber data
    //int bytesRecebidos = recv(socketDescConexao, void *buf, int len, 0);

    #define MAXDATASIZE 100 // max number of bytes we can get at once
    char buf[MAXDATASIZE];
    int numbytes;

    if ((numbytes = recv(innerRemote.socketDesc, buf, MAXDATASIZE-1, 0)) < 0) {
        perror("recv");
        exit(1);    
    }
    
    buf[numbytes] = '\0';    
    printf("client: received '%s'\n",buf);
    //Receber data

    close_Socket(&innerRemote);
	threadExit(NULL);
}

void newConnectionClient(int PortaDeMandar) {
    int command_type;
    char ip[9] = "localhost";

    while(command_type != 9){
        

        fprintf(stdout, "[%.4d] | Mandar Msg (0) ou Receber Mensagem (1)\n", getpid());
        fflush(stdout);
        scanf("%d", &command_type);

        switch(command_type){
            case 0:    
                if((commOps.connect(&localSend, &remoteSend, ip, PortaDeMandar)) < 0){
                    fprintf(stderr, "[%.4d] | Error! Client couldn't connect to peer.\n", getpid());
                    fflush(stderr);
                    perror("connectToPeer");
                    exit(-3);
                }

                //Mandar dados
                //int send(int sockfd, const void *msg, int len, int flags); 

                char *msg = "Beej was here!";
                int len, bytes_sent;
                    
                len = strlen(msg);
                bytes_sent = send(localSend.socketDesc, msg, len, 0);
                //Mandar dados
                break;

            case 1:
                if((commOps.accept(&localRec, &remoteRec)) < 0){
                    fprintf(stderr, "[%.4d] | Error! Client couldn't accept connection.\n", getpid());
                    fflush(stderr);
                    perror("newConnection");
                    exit(-4);
                }
                if(allowNewConnections) {
                    fprintf(stdout, "[%.4d] | New Connection!\n", getpid());
                    fflush(stdout);
                    fprintf(stdout, "[%.4d] | 1\n", getpid());

                    childCount++;
                    fprintf(stdout, "[%.4d] | 2\n", getpid());
                    //mutexLock(mutex_remote_Rec);

                    fprintf(stdout, "[%.4d] | 3\n", getpid());
                    if(noResponse(attendClientPeer, childCount) < 0) {
                        fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
                        fflush(stderr);
                        perror("newConnection");
                    }
                } else {
                    commOps.close(&remoteRec);
                }
                break;

            default:
                fprintf(stdout, "[%.4d] | Command invalid.\n", getpid());
                fflush(stdout);
                break;
        }
    }         
}

int main(int argc, char const *argv[]) {
    fprintf(stdout, "[%d] | Client Module Initialized!\n", getpid());
    fflush(stdout);

   if(argc != 3) {
        fprintf(stderr, "[%d] | Error! Not a Valid Input!\n", getpid());
        fprintf(stderr, "[%d] | Usage: ./client <PortaDeReceber> <PortaDeMandar>\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    if((commOps.initClient(&localSend, 0)) < 0) {
        fprintf(stderr, "[%d] | Error! Init Socket Client!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    fprintf(stdout, "[%d] | Socket de Recebimento\n", getpid());
    fflush(stdout);

    if((commOps.initServer(&localRec, atoi(argv[1]))) < 0){
        fprintf(stderr, "[%d] | Error! Init Socket de Recebimento!\n", getpid());
        fflush(stderr);
        exit(-2);    
    }

    initSharedData();

    do{
        newConnectionClient(atoi(argv[2]));        
    }while(canContinueClient());


    //connectToServer(argv[1]);
    commOps.close(&localSend);
    commOps.close(&localRec);
    return 0;
}
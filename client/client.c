
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
#include "./terminal.h"
#include "../commonLibs/ThreadManager.h"
#include "../commonLibs/UserData.h"

void serverThread() {
    //Criação de Threads que recebem mensagens usando o socket de recebimento

    mutexLock(mutex_ServerSocket);
    mutexLock(mutex_ServerSocket);

    do{
        newReceiver();        
    }while(canContinue());
}

int main(int argc, char const *argv[]) {
    fprintf(stdout, "[%d] | Client Module Initialized!\n", getpid());
    fflush(stdout);

   if(argc != 3) {
        fprintf(stderr, "[%d] | Error! Not a Valid Input!\n", getpid());
        fprintf(stderr, "[%d] | Usage: ./client <ServerIP> <Port>\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    //Inicia váriaveis globais
    initSharedData();
    //connectToServer(argv[1], atoi(argv[2]));
    ip = argv[1];
    port = atoi(argv[2]);

    state = Online;
    //  Command Handler
    noResponse(serverThread, NULL);
    //  Terminal
    initTerminal();
    return 0;

    noResponse(logIn, NULL);

    noResponse(serverThread, NULL);

    initTerminal();

    //Criar uma thread que manda e le mensagens
/*
    if(noResponse(initTerminal, NULL) < 0) {
        fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
        fflush(stderr);
        perror("newConnection");
    }
*/

    return 0;
}

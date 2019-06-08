
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Desenvolvimento Lógico Referente ao Servidor
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./server.h"

int main(int argc, char const *argv[]) {
    fprintf(stdout, "[%d] | Central Server Initialized!\n", getpid());
    fflush(stdout);

    if(argc != 2){
        fprintf(stderr, "[%d] | Error! Not a Valid Input!\n", getpid());
        fprintf(stderr, "[%d] | Usage: ./server <port> \n", getpid());
        fflush(stderr);
        exit(-1);
    }

    if((commOps.initServer(&local, atoi(argv[1]))) < 0){
        fprintf(stderr, "[%d] | Error! Init Socket Server!\n", getpid());
        fflush(stderr);
        exit(-2);    
    }

    initSharedData();

    do{
        newConnection();
    }while(canContinue());

    exitServer();
    return 0;
}
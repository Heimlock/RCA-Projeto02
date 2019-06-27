
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *      Gabriela Ferreira Jorge       RA: 12228441
 *		Rodrigo da Silva Cardoso      RA: 16430126
 *
 *	 Desenvolvimento Lógico Referente ao Servidor
 */

#include "./server.h"
#include "../commonLibs/CustomStreams.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    Log.info(getpid(), "Central Server Initialized!\n");

    if(argc != 2) {
        Log.error(getpid(), "Error! Not a Valid Input!\n");
        Log.error(getpid(), "Usage: ./server <port> \n");
        exit(-1);
    }
    if((commOps.initServer(&local, atoi(argv[1]))) < 0) {
        Log.error(getpid(), "Error! Init Socket Server!\n");
        exit(-2);
    }

    initSharedData();
    do {
        newConnection();
    } while(canContinue());

    exitServer();
    return 0;
}
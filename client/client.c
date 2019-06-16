
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
#include "../commonLibs/CustomStreams.h"

int main(int argc, char *argv[]) {
    Log.setLevel(Log_Debug);
    Log.info(getpid(), "Client Module Initialized!\n");

   if(argc != 3) {
        Log.error(getpid(), "Error! Not a Valid Input!\n");
        Log.error(getpid(), "Usage: ./client <ServerIP> <Port>\n");
        exit(-1);
    }

    //Inicia váriaveis globais
    initSharedData();

    ip = argv[1];
    port = atoi(argv[2]);
    state = Online;

    //  Command Handler
    noResponse(serverThread, NULL);
    //  Terminal
    initTerminal();
    return 0;

}

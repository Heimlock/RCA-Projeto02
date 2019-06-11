
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
    ip = argv[1];
    port = atoi(argv[2]);
    state = Online;
    
    //Cria as threads que recebem mensagens
    noResponse(createReceiver, NULL);

    //Inicia a interface para o usuário
    initTerminal();
    return 0;
}
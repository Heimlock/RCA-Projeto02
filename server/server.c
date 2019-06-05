
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

#include "./server.h"

int main(int argc, char const *argv[]) {
    fprintf(stdout, "[%d] | Init server!\n", getpid());
    fflush(stdout);

    if(argc != 2){
        exit(1);
    }

    commOps.init(&commData, atoi(argv[1]));    
    initSharedData();

    do{
        newConnection();
    }while(canContinue());

    exitServer();
    return 0;
}
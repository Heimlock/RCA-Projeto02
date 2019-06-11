
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

    //Colocar numero de telefone
    fprintf(stdout, "Enter with your 9 digit ID\n");
    fprintf(stdout, "UserId: ");
    fflush(stdout);
    fgets(userId, (UserId_Len + 1) * sizeof(char), stdin);
    __fpurge(stdin);

    //Iniciar socket do tipo Servidor
    if((commOps.initServer(&local, 0)) < 0){ 
        fprintf(stderr, "[%d] | Error! Init Socket de Recebimento!\n", getpid());
        fflush(stderr);
        exit(-2);    
    }

    // TODO Mandar a porta criada no .initServer e o IP para o servidor.
    //Sugestao: Mandar na função de LogIn 
    //LogIn
    noResponse(logIn, NULL);

    //Criar uma thread que manda e le mensagens
    if(noResponse(initTerminal, NULL) < 0) {
        fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
        fflush(stderr);
        perror("newConnection");
    }

    //Criação de Threads que recebem mensagens usando o socket de recebimento
    do{
        newReceiver();        
    }while(canContinue());

    return 0;
}

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
#include "../commonLibs/UserData.h"

int main(int argc, char const *argv[]) {
    //Inicia o cliente
    fprintf(stdout, "[%d] | Client Module Initialized!\n", getpid());
    fflush(stdout);


    initSharedData(); //Inicia variaveis globais
    mutex_remote_Rec = mutexInit(); //Precisa disso para mutex nao dar segmetation fault

    //Precisa de uma variavel (./client PortaDeReceber)
    if(argc != 2) {
        fprintf(stderr, "[%d] | Error! Not a Valid Input!\n", getpid());
        fprintf(stderr, "[%d] | Usage: ./client <PortaDeReceber>\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    //Thread que manda mensagens
    if(noResponse(mandarMensagem, NULL) < 0) {
        fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
        fflush(stderr);
        perror("newConnection");
    }

    //Criação do socket que recebe mensagens, tipo servidor
    fprintf(stdout, "[%d] | Socket de Recebimento\n", getpid());
    fflush(stdout);
    if((commOps.initServer(&localRec, atoi(argv[1]))) < 0){
        fprintf(stderr, "[%d] | Error! Init Socket de Recebimento!\n", getpid());
        fflush(stderr);
        exit(-2);    
    }

    //Threads que recebem mensagens usando o socket de recebimento
    do{
        newConnectionClient();        
    }while(canContinueClient());

    //Fechar sockets
    commOps.close(&localSend);
    commOps.close(&localRec);
    return 0; //Nao vai passar daqui se nao comentar esse return

    //  =============================STUB DO FELIPE=====================================
    //  ================================================================================
    char ip[9] = "localhost";
    int port = 5000; //porta do servidor hardcoded

    struct SPDT_Command *command;


    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 00 -- Init\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

   if(argc != 3) {
        fprintf(stderr, "[%d] | Error! Not a Valid Input!\n", getpid());
        fprintf(stderr, "[%d] | Usage: ./client <ServerIp> <Port>\n", getpid());
        fflush(stderr);
        exit(-1);
    }
   

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 01 -- LogIn\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    if(commOps.connect(&localSend, &remoteSend, 0, ip, port) < 0) { //ServerPort, atoi(argv[2]), definido como 5000
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    char *value = "982660910";

    newCommand(&command, LogIn, UserId_Len * sizeof(char), value);
    printCommand(getpid(), *command);

    if(sendCommand(&localSend, (*command)) < 0) {
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-4);
    }
    // destroyCommand(command);
    commOps.close(&localSend);
    commOps.close(&remoteSend);
    sleep(5);

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 02 -- RequestClient\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    if(commOps.connect(&localSend, &remoteSend, 0, ip, port) < 0) { //ServerPort, atoi(argv[2]), definido como 5000
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    newCommand(&command, RequestClient, UserId_Len * sizeof(char), value);
    printCommand(getpid(), *command);

    if(sendCommand(&localSend, (*command)) < 0) {
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-5);
    }
    // destroyCommand(command);
    

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 03 -- ReceiveStruct\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    struct User_t *user = NULL;
    receiveStruct(&localSend, RequestClient, &user);
    fprintf("Telefone: %s\n", user->id);

    commOps.close(&localSend);
    commOps.close(&remoteSend);
    sleep(10);

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 04 -- LogOut\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    if(commOps.connect(&localSend, &remoteSend, 0, ip, port) < 0) {
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    newCommand(&command, LogOut, UserId_Len * sizeof(char), value);
    printCommand(getpid(), *command);

    if(sendCommand(&localSend, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-6);
    }
    // destroyCommand(command);

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 05 -- Terminated\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);
    commOps.close(&localSend);
    commOps.close(&remoteSend);
}
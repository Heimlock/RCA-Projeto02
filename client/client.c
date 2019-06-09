
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
    fprintf(stdout, "[%d] | Client Module Initialized!\n", getpid());
    fflush(stdout);

    struct SPDT_Command *command;

    //  ================================================================================

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

    if(commOps.initClient(&local, 0) < 0) {
        fprintf(stderr, "[%d] | Error! Init Socket Client!\n", getpid());
        fflush(stderr);
        exit(-2);
    }
    if(commOps.connect(&local, &remote, argv[1], atoi(argv[2])) < 0) {
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    char *value = "982660910";

    newCommand(&command, LogIn, UserId_Len * sizeof(char), value);
    printCommand(getpid(), *command);

    if(sendCommand(&local, (*command)) < 0) {
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-4);
    }
    // destroyCommand(command);
    commOps.close(&local);
    commOps.close(&remote);
    sleep(5);

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 02 -- RequestClient\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    if(commOps.initClient(&local, 0) < 0) {
        fprintf(stderr, "[%d] | Error! Init Socket Client!\n", getpid());
        fflush(stderr);
        exit(-2);
    }
    if(commOps.connect(&local, &remote, argv[1], atoi(argv[2])) < 0) {
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    newCommand(&command, RequestClient, UserId_Len * sizeof(char), value);
    printCommand(getpid(), *command);

    if(sendCommand(&local, (*command)) < 0) {
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
    receiveStruct(&local, RequestClient, &user);
    fprintf("Telefone: %s\n", user->id);

    commOps.close(&local);
    commOps.close(&remote);
    sleep(10);

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 04 -- LogOut\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    if(commOps.initClient(&local, 0) < 0) {
        fprintf(stderr, "[%d] | Error! Init Socket Client!\n", getpid());
        fflush(stderr);
        exit(-2);
    }
    if(commOps.connect(&local, &remote, argv[1], atoi(argv[2])) < 0) {
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    newCommand(&command, LogOut, UserId_Len * sizeof(char), value);
    printCommand(getpid(), *command);

    if(sendCommand(&local, (*command)) < 0){
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
    commOps.close(&local);
    commOps.close(&remote);
    return 0;
}
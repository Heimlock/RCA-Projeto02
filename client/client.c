
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

int main(int argc, char const *argv[]) {
    fprintf(stdout, "[%d] | Client Module Initialized!\n", getpid());
    fflush(stdout);

    struct SPDT_Command *command;
    struct User_t *user;

   if(argc != 4){      //./client id ip port
        exit(1);
    }

    commOps.init(&commData, 0);    
    commOps.connect(&commData, argv[2], atoi(argv[3]));

    fprintf(stdout, "[%d] | logIn.\n", getpid());
    fflush(stdout);
    command = newCommand(LogIn, sizeof(argv[1]), argv[1]);
    
    if(sendCommand(&commData, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(1);
    } 

    sleep(1000);

    fprintf(stdout, "[%d] | requestClient.\n", getpid());
    fflush(stdout);
    command = newCommand(RequestClient, sizeof(argv[1]), argv[1]);

    if(sendCommand(&commData, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(2);
    } 

    user  = (User_t *) receiveStruct(&commData, RequestClient);
    fprintf("Telefone: %s\n", user->id);

    sleep(1000);

    fprintf(stdout, "[%d] | logOut.\n", getpid());
    fflush(stdout);
    command = newCommand(LogOut, sizeof(argv[1]), argv[1]);

    if(sendCommand(&commData, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(3);
    }

    commOps.close(&commData);
    return 0;
}
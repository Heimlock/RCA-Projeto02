
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
    fprintf(stdout, "[%d] | Init client!\n", getpid());
    fflush(stdout);

    char enderecoIP[10] = "localhost";
    int porta = 4000;
    char id[2] = "1";

    struct SPDT_Command *command;
    struct User_t *user;

   /* if(argc != 4){      //./client id ip port
        exit(1);
    }*/

    commOps.init(&commClient, 3000);    
    commOps.connect(&commClient, enderecoIP, porta);

    fprintf(stdout, "[%d] | logIn!\n", getpid());
    fflush(stdout);
    command = newCommand(LogIn, sizeof(id), id);
    
    if(sendCommand(&commClient, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(1);
    } 

    sleep(1000);

    fprintf(stdout, "[%d] | requestUser!\n", getpid());
    fflush(stdout);
    command = newCommand(RequestClient, sizeof(id), id);

    if(sendCommand(&commClient, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(2);
    } 

    user  = (User_t *) receiveStruct(&commClient, RequestClient);
    fprintf("Telefone: %s\n", user->id);

    sleep(1000);

    fprintf(stdout, "[%d] | logOut!\n", getpid());
    fflush(stdout);
    command = newCommand(LogOut, sizeof(id), id);

    if(sendCommand(&commClient, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(3);
    }

    commOps.close(&commClient);
    return 0;
}
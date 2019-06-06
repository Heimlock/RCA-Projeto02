﻿
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

    char *value = "1234";

    struct SPDT_Command *command;
    struct User_t *user;

   if(argc != 3) {
        fprintf(stderr, "[%d] | Error! Not a Valid Input!\n", getpid());
        fprintf(stderr, "[%d] | Usage: ./client <ServerIp> <Port>\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    if(commOps.initClient(&commData, 0) < 0) {
        fprintf(stderr, "[%d] | Error! Init Socket Client!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    if(commOps.connect(&commData, argv[1], atoi(argv[2])) < 0) {
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    fprintf(stdout, "[%d] | logIn.\n", getpid());
    fflush(stdout);
    command = newCommand(LogIn, 4 * sizeof(char), value);
    
    if(sendCommand(&commData, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-4);
    } 

    sleep(1000);

    fprintf(stdout, "[%d] | requestClient.\n", getpid());
    fflush(stdout);
    command = newCommand(RequestClient, 4 * sizeof(char), value);

    if(sendCommand(&commData, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-5);
    } 

    user  = (User_t *) receiveStruct(&commData, RequestClient);
    fprintf("Telefone: %s\n", user->id);

    sleep(1000);

    fprintf(stdout, "[%d] | logOut.\n", getpid());
    fflush(stdout);
    command = newCommand(LogOut, 4 * sizeof(char), value);

    if(sendCommand(&commData, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-6);
    }

    commOps.close(&commData);
    return 0;
}

/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Desenvolvimento de Recursos Referentes ao Protocolo de comunicacao
 *  (Simple Protocol for Data Transfer)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "SPDT.h"

SPDT_Command* newCommand(ActionType type, int length, void* data) {
    SPDT_Command* command = (SPDT_Command*) malloc(sizeof(SPDT_Command));
    command->type    = type;
    command->length  = length;
    command->data    = data;
}

/*
 *  Função que Converte um SPDT_Command em Bytes
 *  Argumentos:
 *      @command    ==  SPDT_Command
 */
char*   command2bytes(SPDT_Command command) {
    int     dataLen = 3 + command.length;
    char*   dataOut = (char*) malloc(dataLen);
    char*   dataAux = (char*) malloc(command.length);

    memset((*dataOut), 0, dataLen);
    memset((*dataAux), 0, command.length);

    dataOut[0]    =   (char)command.type;

    dataOut[1]    =   (char)(command.length / 255);
    dataOut[2]    =   (char)(command.length % 255);

    memcpy(dataAux, command.data, command.length);
    for(i = 0; i < command.length; i++) {
        dataOut[(3+i)] = (char)dataAux[i];
    }
    free(auxData);
    return dataOut;
}

/*
 *  Função que Converte Bytes em um SPDT_Command
 *  Argumentos:
 *      @command    ==  SPDT_Command
 */
SPDT_Command* bytes2command(char* data) {
    SPDT_Command* command;

    command.type    =   (ActionType)input[0];
    command.length  =   (255 * (int)input[1]);
    command.length +=   (int)input[2];

    if(command.length != 0) {
        command.data    =   malloc(command.length);
        memcpy(command.data, (data + (3 * sizeof(char))), command.length);
    } else {
        command.data    =   NULL;
    }
    return command;
}

/*
 *  Função que Converte Bytes em um SPDT_Command
 *  Preenchido apenas com o Header
 *  Argumentos:
 *      @command    ==  SPDT_Command
 */
SPDT_Command* bytes2commandHeader(char* data) {
    SPDT_Command* command;
    command.type    =   (ActionType)input[0];
    command.length  =   (255 * (int)input[1]);
    command.length +=   (int)input[2];
    command.data    =   NULL;
    return command;
}
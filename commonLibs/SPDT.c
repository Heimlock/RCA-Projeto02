
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
 *  Função que envia um SPDT_Command via socket
 *  Argumentos:
 *      @command    ==  SPDT_Command
 *  Retornos:
 *      > 0         ==  Erro
 *      < 0         ==  Numero de Bytes Enviados
 */
size_t  sendCommand(commFacade_t* commData, SPDT_Command command) {
    void*   dataOut = command2bytes(*command);
    return sendData(commData, dataOut, (3+comman.length));
}

/*
 *  Função que recebe um SPDT_Command via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *  Retornos:
 *     NULL         ==  Erro
 *     SPDT_Command*==  Ponteiro para um SPDT_Command
 */
SPDT_Command* receiveCommand(commFacade_t* commData) {
    void*   header = malloc(3*sizeof(char));
    void*   data;
    SPDT_Command* command;
    if(receiveData(commData, header, 3*sizeof(char)) > 0) {
        command = bytes2commandHeader(header);
        if(command->length > 0) {   
            data    = malloc(command->length);
            if(receiveData(commData, data, command->length) > 0) {
                command->data = data;
            } else {
                perror("[receiveCommand] | Receive Data.");
                return NULL;
            }
        }
    } else {
        perror("[receiveCommand] | Receive Header.");
        return NULL;
    }
    return command;
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

/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *      Gabriela Ferreira Jorge       RA: 12228441
 *		Rodrigo da Silva Cardoso      RA: 16430126
 *
 *  Desenvolvimento de Recursos Referentes ao Protocolo de comunicacao
 *  (Simple Protocol for Data Transfer)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "SPDT.h"

void newCommand(SPDT_Command** command, ActionType type, int length, void* data) {
    (*command) = (SPDT_Command*) malloc(sizeof(SPDT_Command));
    (*command)->type   = type;
    (*command)->length = length;
    (*command)->value  = data;
}

void    destroyCommand(SPDT_Command* command) {
    free(command->value);
    free(command);
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

    memset(dataOut, 0, dataLen);
    memset(dataAux, 0, command.length);

    dataOut[0]    =   (char)command.type;

    dataOut[1]    =   (char)(command.length / 255);
    dataOut[2]    =   (char)(command.length % 255);

    memcpy(dataAux, command.value, command.length);
    for(int i = 0; i < command.length; i++) {
        dataOut[(3+i)] = (char)dataAux[i];
    }
    free(dataAux);
    return dataOut;
}

/*
 *  Função que Converte Bytes em um SPDT_Command
 *  Argumentos:
 *      @data       ==  Input Bytes
 *      @command    ==  Output SPDT_Command
 */
void bytes2command(char* data, SPDT_Command**  command) {
    (*command) = (SPDT_Command*) malloc(sizeof(SPDT_Command));

    (*command)->type    =   (int)data[0];
    (*command)->length  =   (255 * (int)data[1]);
    (*command)->length +=   (int)data[2];

    if((*command)->length != 0) {
        (*command)->value    =   malloc((*command)->length);
        memcpy((*command)->value, (data + (3 * sizeof(char))), (*command)->length);
    } else {
        (*command)->value    =   NULL;
    }
}

/*
 *  Função que Converte Bytes em um SPDT_Command
 *  Preenchido apenas com o Header
 *  Argumentos:
 *      @command    ==  SPDT_Command
 */
void bytes2commandHeader(char* data, SPDT_Command** command) {
    (*command) = (SPDT_Command*) malloc(sizeof(SPDT_Command));
    (*command)->type    =   (ActionType)data[0];
    (*command)->length  =   (255 * (int)data[1]);
    (*command)->length +=   (int)data[2];
    (*command)->value   =   NULL;
}

void printCommand(int id, SPDT_Command command) {
    fprintf(stdout,"[%.4d] | Command -- Type: %d\n", id, command.type);
    fflush(stdout);
    fprintf(stdout,"[%.4d] | Command -- Length: %d\n", id, command.length);
    fflush(stdout);

    char* auxValue = command.value;
    for( int i = 0; i< command.length; i++ ) {
        fprintf(stdout,"[%.4d] | Command -- Value[%d]: 0x%02hhX\n", id, i, auxValue[i]);
        fflush(stdout);
    }
}
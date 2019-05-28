
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca de Recursos Referentes ao Protocolo de comunicacao
 *  (Simple Protocol for Data Transfer)
 */

#ifndef SPDT_Def
#define SPDT_Def

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "TCP.h"

typedef enum ActionType {
    LogIn        = 0x00,
    LogOut       = 0x01,
    RequestClient= 0x02,
    SendText     = 0x03,
    SendFile     = 0x04,
} ActionType;

typedef struct SPDT_Command {
    ActionType type;
    int length;
    void* value;
} SPDT;

SPDT_Command* newCommand(ActionType type, int length, void* data);
char*   command2bytes(SPDT_Command command);
SPDT_Command* bytes2command(char* data);
SPDT_Command* bytes2commandHeader(char* data);
#endif

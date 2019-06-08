
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca de Recursos Referentes a Comunicacao 
 */

#ifndef Communication_Def
#define Communication_Def

#include "./TCP.h"

/*
 *  SPDT Related
 */
#include "./SPDT.h"
int sendCommand(struct commFacade_t* commData, struct SPDT_Command command);
void receiveCommand(struct commFacade_t* commData, struct SPDT_Command** command);
void*   receiveStruct(struct commFacade_t* commData, enum ActionType expectedType);

/*
 *  UserData Related
 */
#include "./UserData.h"
int sendUser(struct commFacade_t* commData, struct User_t user);

/*
 *  MessageData Related
 */
#include "./MessageData.h"
int sendMessage(struct commFacade_t* commData, struct Message_t message);

/*
 *  FileData Related
 */
#include "./FileData.h"
int sendFile(struct commFacade_t* commData, struct File_t file);

/*
 *  Auxiliary
 */
void printBytes(int id, void *bytes, int length);
#endif

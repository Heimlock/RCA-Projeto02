
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
int sendCommand(commFacade_t* commData, SPDT_Command command);
SPDT_Command* receiveCommand(commFacade_t* commData);
void*   receiveStruct(commFacade_t* commData, ActionType expectedType);

/*
 *  UserData Related
 */
#include "./UserData.h"
int sendUser(commFacade_t* commData, User_t user);

/*
 *  MessageData Related
 */
#include "./MessageData.h"
int sendMessage(commFacade_t* commData, Message_t message);

/*
 *  FileData Related
 */
#include "./FileData.h"
int sendFile(commFacade_t* commData, File_t file);

#endif

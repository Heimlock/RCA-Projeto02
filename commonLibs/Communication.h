
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

/*
 *  UserData Related
 */
#include "./UserData.h"
int sendUser(commFacade_t* commData, User_t user);
User_t* receiveUser(commFacade_t* commData);

/*
 *  MessageData Related
 */
#include "./MessageData.h"
int sendMessage(commFacade_t* commData, Message_t message);
Message_t* receiveMessage(commFacade_t* commData);

/*
 *  FileData Related
 */
#include "./FileData.h"
int sendFile(commFacade_t* commData, File_t file);
File_t* receiveFile(commFacade_t* commData);

#endif

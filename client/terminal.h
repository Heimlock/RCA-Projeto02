
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca de Recursos Referentes ao Terminal
 */

#ifndef Terminal_Def
#define Terminal_Def

#include "../commonLibs/MessageData.h"
#include "../commonLibs/FileData.h"
#include "../commonLibs/UserData.h"

typedef enum MenuItem {
    //  Main Menu
    DirectMessage = 0x01,
    DirectFile = 0x02,
    GroupMessage = 0x03,
    GroupFile = 0x04,
    Inbox = 0x05,
    Contacts = 0x06,
    //  Contacts SubMenu
    ListUsers = 0x07,
    ListGroups = 0x08,
    NewContact = 0x09,
    DeleteContact = 0x0A,
    NewGroup = 0x0B,
    DeleteGroup = 0x0C,
    //  Generic
    Exit = 0x63,
    Error = 0xFF
} MenuItem;

void    *initTerminal();
void    contactsSubMenu ();
void    printHeader();
int     mainMenu();
int     displayContactsMenu();
void    directMessage(char* userId, char** peerId, Message_t** msg);
void    directFile(char* userId, char** peerId, File_t** file);
void    groupMessage(char* userId, char** groupId, Message_t** msg);
void    groupFile(char* userId, char** groupId, File_t** file);
void    getString(char* inputStr, char** id, int maxStrLen);
void    enter2Continue();

void    *sendDirectMessage();
void    *sendGroupMessage();
void    *sendDirectFile();
void    *sendGroupFile();

#endif

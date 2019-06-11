
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

typedef enum ActionType {
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
    Exit = 0x63
} ActionType;

void    *init(void* arg);
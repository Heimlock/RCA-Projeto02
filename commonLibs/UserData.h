
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrant   es:
 *	 addr;	Felipe Moreira Fesockaddr_inira       RA: 16116469
    *
 *  Biblioteca de Recursos Referentes ao Usuario
 */

#ifndef User_Def
#define User_Def
#include <netinet/in.h>

typedef enum UserState {
    Offline     = 0x00,
    Online      = 0x01,
    Away        = 0x02,
} UserState;

typedef struct User_t {
    char*       id;
    sockaddr_in addr;
    UserState   state;
} User_t;

User_t newUser(char* id, sockaddr_in addr, char state);
char*  user2Bytes(User_t user);
User_t bytes2User(char* data);
#endif
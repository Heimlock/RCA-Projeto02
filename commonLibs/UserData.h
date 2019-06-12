
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
 *  Biblioteca de Recursos Referentes ao Usuario
 */

#ifndef User_Def
#define User_Def
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>

#define UserId_Len      9
#define UserData_Len   26

typedef enum UserState {
    Offline     = 0x00,
    Online      = 0x01,
    Away        = 0x02,
} UserState;

typedef struct User_t {
    char*       id;
    struct  sockaddr_in addr;
    enum    UserState   state;
} User_t;

void newUser(User_t** user, char* id, struct sockaddr_in addr, enum UserState state);
char*  user2Bytes(User_t user);
void bytes2User(User_t** user, char* data);
void printUser(User_t user);
char* getState(int stateCode);
#endif
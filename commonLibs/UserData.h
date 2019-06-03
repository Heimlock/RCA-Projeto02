
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
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

User_t* newUser(char* id, struct sockaddr_in addr, enum UserState state);
char*  user2Bytes(User_t user);
User_t* bytes2User(char* data);
void printUser(User_t user);
char* getState(int stateCode);
#endif
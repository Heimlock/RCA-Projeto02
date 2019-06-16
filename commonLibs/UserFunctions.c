
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Desenvolvimento dos Recursos Referentes ao Usuario
 */

#include"./UserData.h"
#include"./CustomStreams.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void newUser(User_t** user, char* id, struct sockaddr_in addr, enum UserState state) {
    (*user) = (User_t*) malloc(sizeof(User_t));

    (*user)->id = (char *) malloc((UserId_Len + 1) * sizeof(char));
    memcpy((*user)->id, id, UserId_Len);
    (*user)->id[UserId_Len] = '\0';
    (*user)->addr = addr;
    (*user)->state= state;
    #ifdef DEBUG
    if((*user) != NULL) {
        printUser(*(*user));
    }
    #endif
}

char*  user2Bytes(User_t user) {
    void* dataOut = malloc(UserData_Len);
    memcpy(dataOut, user.id, UserId_Len);
    memcpy(dataOut + 9, &user.addr, sizeof(struct sockaddr_in));
    memcpy(dataOut + 25, &user.state, 1 * sizeof(char));
    return dataOut;
}

void bytes2User(User_t** user, char* data) {
    (*user) = (User_t *) malloc(sizeof(User_t));
    (*user)->id = (char *) malloc(UserId_Len * sizeof(char));

    memcpy((*user)->id, data, UserId_Len);
    memcpy(&(*user)->addr, data + 9, sizeof(struct sockaddr_in));
    memcpy(&(*user)->state, data + 25, 1 * sizeof(char));
    #ifdef DEBUG
    if((*user) != NULL) {
        printUser(*(*user));
    }
    #endif
}

void printUser(User_t user) {
    Log.info(getpid(), "User Id...: %s\n", user.id);
    Log.info(getpid(), "User Ip...: %s\n", inet_ntoa(user.addr.sin_addr));
    Log.info(getpid(), "User Port.: %d\n", ntohs(user.addr.sin_port));
    Log.info(getpid(), "User State: %s\n", getState(user.state));
}

char* getState(enum UserState stateCode) {
    switch (stateCode) {
    case Offline:
        return "Offline";
    case Online:
        return "Online";
    case Away:
        return "Away";
    default:
        return "Not Defined";
    }
}

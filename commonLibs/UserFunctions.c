
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include"./UserData.h"

User_t* newUser(char* id, struct sockaddr_in addr, enum UserState state) {
    User_t* newUser = (User_t*) malloc(sizeof(User_t));

    newUser->id = (char *) malloc(UserId_Len * sizeof(char));
    memcpy(&newUser->id, id, UserId_Len);
    newUser->addr = addr;
    newUser->state= state;
    #ifdef DEBUG
        printUser(*newUser);
    #endif
    return newUser;
}

char*  user2Bytes(User_t user) {
    void* dataOut = malloc(UserData_Len);
    memcpy(dataOut, user.id, UserId_Len);
    memcpy(dataOut + 9, &user.addr, sizeof(struct sockaddr_in));
    memcpy(dataOut + 25, &user.state, 1 * sizeof(char));
    return dataOut;
}

User_t* bytes2User(char* data) {
    User_t* newUser = (User_t *) malloc(sizeof(User_t));
    newUser->id = (char *) malloc(UserId_Len * sizeof(char));

    memcpy(newUser->id, data, UserId_Len);
    memcpy(&newUser->addr, data + 9, sizeof(struct sockaddr_in));
    memcpy(&newUser->state, data + 25, 1 * sizeof(char));
    #ifdef DEBUG
        printUser(*newUser);
    #endif
    return newUser;
}

void printUser(User_t user) {
    fprintf(stdout, "[%d] | User Id...: %s\n", getpid(), user.id);
    fprintf(stdout, "[%d] | User Ip...: %s\n", getpid(), inet_ntoa(user.addr.sin_addr));
    fprintf(stdout, "[%d] | User Port.: %d\n", getpid(), ntohs(user.addr.sin_port));
    fprintf(stdout, "[%d] | User State: %s\n", getpid(), getState(user.state));
    fflush(stdout);
}

char* getState(int stateCode) {
    switch (stateCode) {
    case 0x00:
        return "Offline";
    case 0x01:
        return "Online";
    case 0x02:
        return "Away";
    default:
        return "Not Defined";
    }
}

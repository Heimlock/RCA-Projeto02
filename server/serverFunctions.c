
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
 *	 Desenvolvimento dos Recursos Referentes ao Servidor
 */

#include "./server.h"
#include "../commonLibs/LinkedList.h"
#include "../commonLibs/CustomStreams.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

void newConnection() {
    Log.debug(getpid(), "NewConnection Init!\n");

    if((commOps.accept(&local, &remote)) < 0){
        Log.error(getpid(), "Error! Server couldn't accept connection.\n");
        perror("newConnection");
        exit(-1);
    }

    Log.debug(getpid(), "NewConnection Accepted!\n");
    if(allowNewConnections) {
        Log.debug(getpid(), "New Connection!\n");

        childCount++;
        mutexLock(mutex_remote);
        if(noResponse(attendClient, childCount) < 0) {
            Log.error(getpid(), "Error! Thread couldn't attend.\n");
            perror("newConnection");
        }
    } else {
        commOps.close(&remote);
    }
}

void *attendClient(void *arg) {
	int threadId =  (int)arg;
    struct SPDT_Command  *command;
	struct commFacade_t  innerRemote;

    Log.debug(threadId, "Attend Client Init\n");

	memcpy(&innerRemote, &remote, sizeof(commFacade_t));
    mutexUnlock(mutex_remote);

    receiveCommand(&innerRemote, &command);
    if(command == NULL) {
        Log.error(threadId, "Error! Command is NULL.\n");
    } else {
        switch(command->type) {
            case LogIn:
                logIn(innerRemote, command);
                break;
            case LogOut:
                logOut(innerRemote, command);
                break;
            case RequestClient:
                requestClient(innerRemote, command);
                break;
            default:
                Log.error(threadId, "Error! Not a Valid Type. Type = %d\n", command->type);
                break;
        }
    }
    close_Socket(&innerRemote);
	threadExit(NULL);
    return NULL;
}

int canContinue() {
    int rtnValue;
    if(allowNewConnections) {
        rtnValue = 1;
    } else {
        rtnValue = 0;
    }
    return rtnValue;
}

void exitServer() {
    int done = 0;
    do {
        sleep(1);
        if(childCount == 0) {
            Log.info(getpid(), "Exiting Server\n");
            commOps.close(&local);
            done = 1;
        }
    } while(!done);
}

void  initSharedData() {
	allowNewConnections = 1;
	childCount  = 0;
    acceptNewConnections = 1;

    initList(&users);

    mutex_remote = mutexInit();
    mutex_list_users = mutexInit();

    if(mutex_remote == NULL || mutex_list_users == NULL){
        mutexDestroy(mutex_remote);
        mutexDestroy(mutex_list_users);
        commOps.close(&local);
        exit(-3);
    }
}

void parseLogin(void* dataIn, char** userId, int** clientSocketPort) {
    (*userId) = (char*) malloc(UserId_Len * sizeof(char));
    (*clientSocketPort) = (int*) malloc(sizeof(int));

    int offset = 0;
    memcpy((*clientSocketPort), dataIn + offset, sizeof(int));
    offset = sizeof(int);
    memcpy((*userId), dataIn + offset, (UserId_Len * sizeof(char)));
}

void  logIn(struct commFacade_t communication_data, struct SPDT_Command *log_in) {
    struct User_t* user;
    struct LinkedListNode *userNode;
    char* userId;
    struct sockaddr_in* clientAddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    int* clientServerPort;

    Log.debug(getpid(), "LogIn Function Init\n");
    #ifdef  DEBUG
        printCommand(getpid(), *log_in);
    #endif

    if(log_in->value != NULL) {
	    parseLogin(log_in->value, &userId, &clientServerPort);
        memcpy(clientAddr, &communication_data.socketAddr, sizeof(struct sockaddr_in));
        clientAddr->sin_port        = htons(*clientServerPort);

        mutexLock(mutex_list_users);
        userNode = getNode(*users, (char *) userId);
        if(userNode != NULL) {
            Log.info(getpid(), "User Updated\n");
            user = (User_t *) userNode->data;
            user->state = Online;
            user->addr = *clientAddr;
            printUser(*user);
        } else {
            newUser(&user, userId, *clientAddr, Online);
			if(user != NULL) {
                Log.info(getpid(), "User Added\n");
                addNode(&users, user->id, UserData_Len, user);
                free(user);
            } else {
                Log.error(getpid(), "Couln't create new user.\n");
            }
        }
        mutexUnlock(mutex_list_users);
    } else {
        Log.error(getpid(), "Error! Client ID not received.\n");
    }
}

void    logOut(struct commFacade_t communication_data, struct SPDT_Command *log_out) {
	struct User_t* user;
    struct LinkedListNode *userNode;

    if(log_out->value != NULL){
        mutexLock(mutex_list_users);
        userNode = getNode(*users, (char *) log_out->value);
        if(userNode != NULL) {
            Log.info(getpid(), "User Offline\n");
            user = (User_t *) userNode->data;
            user->state = Offline;
            printUser(*user);
        } else {
            Log.error(getpid(), "Error! User doens't exist.\n");
        }
        mutexUnlock(mutex_list_users);
    } else {
        Log.error(getpid(), "Error! User couldn't log out.\n");
    }
}

void	requestClient(struct commFacade_t commData, struct SPDT_Command *requestCommand) {
	struct User_t* user;
	struct LinkedListNode *userNode;

	if(requestCommand->value != NULL) {
    mutexLock(mutex_list_users);
    userNode = getNode(*users, (char*) requestCommand->value);
    if(userNode != NULL) {
      user = (User_t *) userNode->data;
      if(user != NULL) {
        if(user->state == Online) {
          if((sendUser(&commData, (*user))) < 0) {
            Log.error(getpid(), "Error! Failed to send user requested.\n");
          }
        } else {
          Log.error(getpid(), "Error! User not online.\n");
        }
      } else {
        Log.error(getpid(), "Error! Coundn't get user data.\n");
      }
    } else {
      Log.error(getpid(), "Error! User doens't exist.\n");
    }
    mutexUnlock(mutex_list_users);
  } else {
    Log.error(getpid(), "Error! Client ID not received.\n");
	}
}

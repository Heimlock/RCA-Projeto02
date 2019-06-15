
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Desenvolvimento dos Recursos Referentes ao Servidor
 */

#include "./server.h"
#include "../commonLibs/LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

void newConnection() {
    fprintf(stdout, "[%.4d] | NewConnection Init!\n", getpid());
    fflush(stdout);
    if((commOps.accept(&local, &remote)) < 0){
        fprintf(stderr, "[%.4d] | Error! Server couldn't accept connection.\n", getpid());
        fflush(stderr);
        perror("newConnection");
        exit(-4);
    }

    fprintf(stdout, "[%.4d] | NewConnection Accepted!\n", getpid());
    fflush(stdout);

    if(allowNewConnections) {
        fprintf(stdout, "[%.4d] | New Connection!\n", getpid());
        fflush(stdout);

        childCount++;
        mutexLock(mutex_remote);
        if(noResponse(attendClient, childCount) < 0) {
            fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
            fflush(stderr);
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

    fprintf(stdout, "[%.4d] | Attend Client Init\n", threadId);
    fflush(stdout);

	memcpy(&innerRemote, &remote, sizeof(commFacade_t));
    mutexUnlock(mutex_remote);

    receiveCommand(&innerRemote, &command);

    if(command == NULL) {
        fprintf(stderr, "[%.4d] | Error! Command is NULL.\n", threadId);
        fflush(stderr);
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
                fprintf(stderr, "[%.4d] | Error! Not a Valid Type. Type = %d\n", threadId, command->type);
                fflush(stderr);
                break;
        }
    }
    close_Socket(&innerRemote);
	threadExit(NULL);
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
            fprintf(stdout, "[%d] | Exiting Server\n", getpid());
            fflush(stdout);
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

    #ifdef  DEBUG
        fprintf(stdout, "[%d] | LogIn Function Init\n", getpid());
        fflush(stdout);

        printCommand(getpid(), *log_in);
    #endif

    if(log_in->value != NULL) {
	    parseLogin(log_in->value, &userId, &clientServerPort);
        memcpy(clientAddr, &communication_data.socketAddr, sizeof(struct sockaddr_in));
        clientAddr->sin_port        = htons(*clientServerPort);

        mutexLock(mutex_list_users);
        userNode = getNode(*users, (char *) userId);
        if(userNode != NULL) {
            fprintf(stdout, "[%d] | User Updated\n", getpid());
            fflush(stdout);
            user = (User_t *) userNode->data;
            user->state = Online;
            user->addr = *clientAddr;
            printUser(*user);
        } else {
            newUser(&user, userId, *clientAddr, Online);
			if(user != NULL) {
                fprintf(stdout, "[%d] | New User Added\n", getpid());
                fflush(stdout);
                addNode(&users, user->id, UserData_Len, user);
                free(user);
            } else {
                fprintf(stderr, "[%d] | Couln't create new user.\n", getpid());
				fflush(stderr);
				perror("logIn");
            }
        }
        mutexUnlock(mutex_list_users);
    } else {
        fprintf(stderr, "[%d] | Error! Client ID not received.\n", getpid());
        fflush(stderr);
        perror("logIn");
    }
}

void    logOut(struct commFacade_t communication_data, struct SPDT_Command *log_out) {
	struct User_t* user;
    struct LinkedListNode *userNode;

    if(log_out->value != NULL){
        mutexLock(mutex_list_users);
        userNode = getNode(*users, (char *) log_out->value);
        if(userNode != NULL) {
            fprintf(stdout, "[%d] | User Offline\n", getpid());
            fflush(stdout);
            user = (User_t *) userNode->data;
            user->state = Offline;
            printUser(*user);
        } else {
            fprintf(stderr, "[%d] | Error! User doens't exist.\n", getpid());
            fflush(stderr);
            perror("logOut");
        }
        mutexUnlock(mutex_list_users);
    } else {
        fprintf(stderr, "[%d] | Error! User couldn't log out.\n", getpid());
        fflush(stderr);
        perror("logOut");
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
      if(user != NULL){
        if(user->state == Online) {
          if((sendUser(&commData, (*user))) < 0) {
            fprintf(stderr, "[%d] | Error! Failed to send user requested.\n", getpid());
            fflush(stderr);
          }
        } else {
          fprintf(stderr, "[%d] | Error! User not online.\n", getpid());
          fflush(stderr);
        }
      } else {
        fprintf(stderr, "[%d] | Error! Coundn't get user data.\n", getpid());
        fflush(stderr);
      }
    } else {
      fprintf(stderr, "[%d] | Error! User doens't exist.\n", getpid());
      fflush(stderr);
    }
    mutexUnlock(mutex_list_users);
  } else {
	  fprintf(stderr, "[%d] | Error! Client ID not received.\n", getpid());
    fflush(stderr);
	}
}

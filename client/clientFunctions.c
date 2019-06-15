
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Desenvolvimento dos Recursos Referentes ao Cliente
 */

#include "./client.h"
#include "../commonLibs/MessageData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

void serverThread() {
    mutexLock(mutex_ServerSocket);
    mutexLock(mutex_ServerSocket);

    #ifdef  DEBUG
        fprintf(stdout, "[%d] | Client Server Thead Initialized!\n", getpid());
        fflush(stdout);
    #endif

    do {
        newReceiver();
    } while(canContinue());
}

void    logIn() {
    commFacade_t sendSocket, serverConnectionSckt;
    struct SPDT_Command *command;

    #ifdef  DEBUG
    fprintf(stdout, "[%.4d] | Log In Function\n", getpid());
    fflush(stdout);
    #endif

    if((commOps.connect(&sendSocket, &serverConnectionSckt, 0, ip, port)) < 0) {
        fprintf(stderr, "[%.4d] | Error! Client couldn't connect to server.\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    int auxRtn, count = 0;
    do {
        if(count > 10) {
            fprintf(stderr, "[%.4d] | Error! Client couldn't Create a Server Socket.\n", getpid());
            fflush(stderr);
            exit(-1);
        } else {
            count++;
            auxRtn = 0;
            auxRtn = init_Server(&local, rand() % 10000);
        }
    } while(auxRtn < 0);

    int commandSize = (UserId_Len * sizeof(char)) + sizeof(int);
    void* dataOut = malloc(commandSize);
    int offset = 0;
    int auxPort = ntohs(local.socketAddr.sin_port);
    memcpy(dataOut + offset, &auxPort, sizeof(int));
    offset = sizeof(int);
    memcpy(dataOut + offset, userId, (UserId_Len * sizeof(char)));

    newCommand(&command, LogIn, commandSize, dataOut);

    if((sendCommand(&sendSocket, (*command))) < 0) {
        fprintf(stderr, "[logIn] | Error! Failed to send.\n");
        fflush(stderr);
        exit(-2);
    }
    commOps.close(&sendSocket);
    commOps.close(&serverConnectionSckt);
    //   MutexUnlock
    mutexUnlock(mutex_ServerSocket);
}

void    logOut() {
    commFacade_t local, remote;
    struct SPDT_Command *command;

    #ifdef  DEBUG
    fprintf(stdout, "[%.4d] | Log Out.\n", getpid());
    fflush(stdout);
    #endif

    if((commOps.connect(&local, &remote, 0, ip, port)) < 0) {
        fprintf(stderr, "[%.4d] | Error! Client couldn't connect to server.\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    newCommand(&command, LogOut, UserId_Len*sizeof(char), userId);

    if((sendCommand(&local, (*command))) < 0) {
        fprintf(stderr, "[logOut] | Error! Failed to send.\n");
        fflush(stderr);
        exit(-1);
    }
    commOps.close(&local);
    commOps.close(&remote);
}

User_t* requestClient(char* peerId) {
    struct commFacade_t local, remote;
    int dataType;
    void *dataReceived;
    struct SPDT_Command *command;
    struct User_t *user;

    #ifdef  DEBUG
    fprintf(stdout, "[%.4d] | Request Client.\n", getpid());
    fflush(stdout);
    #endif

    if((commOps.connect(&local, &remote, 0, ip, port)) < 0) {
        fprintf(stderr, "[%.4d] | Error! Client couldn't connect to server.\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    newCommand(&command, RequestClient, UserId_Len*sizeof(char), peerId);

    if((sendCommand(&local, (*command))) < 0) {
        fprintf(stderr, "[requestClient] | Error! Failed to send.\n");
        fflush(stderr);
        exit(-2);
    }

    dataType = receiveStruct(&local, &dataReceived);
    if(dataReceived != NULL) {
            switch(dataType) {
            case RequestClient: {
                user = (User_t *) dataReceived;
                if(user == NULL) {
                    fprintf(stderr, "[requestClient] | Error! Failed to receive user.\n");
                    fflush(stderr);
                    perror(requestClient);
                    exit(-1);
                }
                commOps.close(&local);
                commOps.close(&remote);
                return user;
            }
            default:{
                fprintf(stderr, "[%.4d] | Type not expected.\n", getpid());
                fflush(stderr);
            break;
            }
        }
    } else {
        fprintf(stderr, "[%.4d] | Error! Couldn't Receive User.\n", getpid());
        fflush(stderr);
    }
    commOps.close(&local);
    commOps.close(&remote);
    return NULL;
}

int    canContinue() {
    int rtnValue;
    if(allowNewConnections) {
        rtnValue = 1;
    } else {
        rtnValue = 0;
    }
    return rtnValue;
}

void  	newReceiver() {
    threadCount++;

    if((commOps.accept(&local, &remote)) < 0) {
        fprintf(stderr, "[%.4d] | Error! Client couldn't accept connection.\n", getpid());
        fflush(stderr);
        perror("newConnection");
        exit(-1);
    }

    if(allowNewConnections) {
        #ifdef  DEBUG
        fprintf(stdout, "[%.4d] | New Connection!\n", getpid());
        fflush(stdout);
        #endif
        mutexLock(mutex_RemoteSocket);
        if(noResponse(attendClientPeer, threadCount) < 0) {
            fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
            fflush(stderr);
            perror("newConnection");
        }
    } else {
        commOps.close(&remote);
    }
}

void    attendClientPeer(void *arg) {
	int threadId =  (int)arg;
	struct commFacade_t  innerRemote;
    int  dataType;
    void *dataReceived;
    Message_t *message;
    File_t *file;

    #ifdef  DEBUG
    fprintf(stdout, "[%.4d] | Attend Peer Init\n", threadId);
    fflush(stdout);
    #endif

	memcpy(&innerRemote, &remote, sizeof(commFacade_t));
    mutexUnlock(mutex_RemoteSocket);

    dataType = receiveStruct(&innerRemote, &dataReceived);
    if(dataReceived != NULL) {
		switch(dataType){
	    	case SendText: {
				message = (Message_t *) dataReceived;
				addNode(&messages, message->senderId, message->length, message->data);
				free(message);
				break;
            }
	    	case SendFile: {
				file = (File_t *) dataReceived;
				memory2Disk((*file));
				free(file);
				break;
			}
			default: {
				fprintf(stderr, "[%.4d] | Type not expected.\n", threadId);
                fflush(stderr);
                break;
			}
		}
	} else {
		fprintf(stderr, "[%.4d] | Error! Couldn't receive data.\n", threadId);
        fflush(stderr);
	}
	commOps.close(&innerRemote);
	threadExit(NULL);
}

void 	sendMessagePeer(void* vars) { //(struct sockaddr_in address, struct Message_t message){
    struct commFacade_t localPeer, remotePeer;
    char *ipPeer;
    int portPeer;
    struct Message_t message;
    char *peerId;
    int offset;

    offset = 0;
    memcpy(peerId, vars + offset, (UserId_Len + 1) * sizeof(char));
    offset = (UserId_Len + 1) * sizeof(char);
    memcpy(&message, vars + offset, sizeof(Message_t));
    free(vars);

    User_t  *user;
    user = requestClient(peerId);

    ipPeer = inet_ntoa(user->addr.sin_addr);
    portPeer = ntohs(user->addr.sin_port);

    if((commOps.connect(&localPeer, &remotePeer, 0, ipPeer, portPeer)) < 0) {
	    fprintf(stderr, "[%.4d] | Error! Couldn't connect to peer.\n", getpid());
        fflush(stderr);
        exit(-1);
    }
    if((sendMessage(&localPeer, message)) < 0) {
	    fprintf(stderr, "[%.4d] | Error! Couldn't send message to peer.\n", getpid());
        fflush(stderr);
        exit(-2);
    }
    commOps.close(&localPeer);
    commOps.close(&remotePeer);
}

void 	sendFilePeer(void* vars) { //(struct sockaddr_in address, struct File_t file){
    struct commFacade_t localPeer, remotePeer;
    char *ipPeer;
    int portPeer;
    struct File_t file;
    char *peerId;
    int offset;

    offset = 0;
    memcpy(peerId, vars + offset, (UserId_Len + 1) * sizeof(char));
    offset = (UserId_Len + 1) * sizeof(char);
    memcpy(&file, vars + offset, sizeof(File_t));
    free(vars);

    User_t  *user;
    user = requestClient(peerId);

    ipPeer = inet_ntoa(user->addr.sin_addr);
    portPeer = ntohs(user->addr.sin_port);

    if((commOps.connect(&localPeer, &remotePeer, 0, ipPeer, portPeer)) < 0) {
	    fprintf(stderr, "[%.4d] | Error! Couldn't connect to peer.\n", getpid());
        fflush(stderr);
        exit(-1);
    }
    if((sendFile(&localPeer, file)) < 0) {
	    fprintf(stderr, "[%.4d] | Error! Couldn't send file to peer.\n", getpid());
        fflush(stderr);
        exit(-2);
    }
    commOps.close(&localPeer);
    commOps.close(&remotePeer);
}

void    printGroup(LinkedListNode* group) {
    LinkedListHead* groupHead = (LinkedListHead*)group->data;
    forEach(groupHead, printAllKeys);
}

void  initSharedData() {
    mutex_list_messages = mutexInit();
    mutex_ServerSocket = mutexInit();
    mutex_RemoteSocket = mutexInit();

    initList(&messages);
    initList(&contacts);
    initList(&groups);

    threadCount = 0;
    allowNewConnections = 1;

    //TODO free nas listas
    if(mutex_list_messages == NULL) {
        mutexDestroy(mutex_list_messages);
        exit(-1);
    }
}

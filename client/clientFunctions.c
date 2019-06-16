
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
#include "../commonLibs/CustomStreams.h"
#include "../commonLibs/IPC.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

void serverThread() {
    IPC.semLock(&sem_ServerSocket);
    IPC.semLock(&sem_ServerSocket);

    Log.debug(getpid(), "Client Server Thead Initialized!\n");
    do {
        newReceiver();
    } while(canContinue());
}

void    logIn() {
    commFacade_t sendSocket, serverConnectionSckt;
    struct SPDT_Command *command;

    Log.debug(getpid(), "Log In Function\n");
    if((commOps.connect(&sendSocket, &serverConnectionSckt, 0, ip, port)) < 0) {
        Log.error(getpid(), "Error! Client couldn't connect to server.\n");
        exit(-1);
    }

    int commandSize = (UserId_Len * sizeof(char)) + sizeof(int);
    void* dataOut = malloc(commandSize);
    int offset = 0;
    int auxPort = ntohs(local.socketAddr.sin_port);
    memcpy(dataOut + offset, &auxPort, sizeof(int));
    offset = sizeof(int);
    memcpy(dataOut + offset, userId, (UserId_Len * sizeof(char)));

    newCommand(&command, LogIn, commandSize, dataOut);

    if((sendCommand(&sendSocket, (*command))) < 0) {
        Log.error(getpid(), "logIn -- Error! Failed to send.\n");
        exit(-2);
    }
    commOps.close(&sendSocket);
    commOps.close(&serverConnectionSckt);
    //   Unlock
    IPC.semUnlock(&sem_ServerSocket);
}

void    logOut() {
    commFacade_t local, remote;
    struct SPDT_Command *command;

    Log.debug(getpid(), "Log Out.\n");

    if((commOps.connect(&local, &remote, 0, ip, port)) < 0) {
        Log.error(getpid(), "Error! Client couldn't connect to server.\n");
        exit(-1);
    }

    newCommand(&command, LogOut, UserId_Len*sizeof(char), userId);

    if((sendCommand(&local, (*command))) < 0) {
        Log.error(getpid(), "Error! Failed to send.\n");
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

    Log.debug(getpid(), "Request Client.\n");

    if((commOps.connect(&local, &remote, 0, ip, port)) < 0) {
        Log.error(getpid(), "Error! Client couldn't connect to server.\n");
        exit(-1);
    }

    newCommand(&command, RequestClient, UserId_Len*sizeof(char), peerId);

    if((sendCommand(&local, (*command))) < 0) {
        Log.error(getpid(), "Error! Failed to send.\n");
        exit(-2);
    }

    dataType = receiveStruct(&local, &dataReceived);
    if(dataReceived != NULL) {
            switch(dataType) {
            case RequestClient: {
                user = (User_t *) dataReceived;
                if(user == NULL) {
                    Log.error(getpid(), "Error! Failed to receive user.\n");
                    exit(-1);
                }
                commOps.close(&local);
                commOps.close(&remote);
                return user;
            }
            default:{
                Log.error(getpid(), "Type not expected.\n");
            break;
            }
        }
    } else {
        Log.error(getpid(), "Error! Couldn't Receive User.\n");
    }
    commOps.close(&local);
    commOps.close(&remote);
    return NULL;
}

int    canContinue() {
    int rtnValue;
    IPC.semLock(&sem_CanContinue);
    if(allowNewConnections) {
        rtnValue = 1;
    } else {
        rtnValue = 0;
    }
    IPC.semUnlock(&sem_CanContinue);
    return rtnValue;
}

void  	newReceiver() {
    threadCount++;

    if((commOps.accept(&local, &remote)) < 0) {
        Log.error(getpid(), "Error! Client couldn't accept connection.\n");
        perror("newReceiver");
        exit(-1);
    }

    if(allowNewConnections) {
        Log.debug(getpid(), "New Connection!\n");
        mutexLock(mutex_RemoteSocket);
        if(waitResponse(attendClientPeer, threadCount) < 0) {
            Log.error(getpid(), "Error! Thread couldn't attend.\n");
            perror("newReceiver");
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

    Log.debug(threadId, "Attend Peer Init\n");

	memcpy(&innerRemote, &remote, sizeof(commFacade_t));
    mutexUnlock(mutex_RemoteSocket);

    dataType = receiveStruct(&innerRemote, &dataReceived);
    if(dataReceived != NULL) {
		switch(dataType) {
	    	case SendText: {
				message = (Message_t *) dataReceived;
                #ifdef  DEBUG
                    printMsg(*message);
                #endif
                IPC.semLock(&sem_list_messages);
				addNode(&messages, message->senderId, message->length, message);
                IPC.semUnlock(&sem_list_messages);
				break;
            }
	    	case SendFile: {
				file = (File_t *) dataReceived;
				memory2Disk((*file));
				free(file);
				break;
			}
			default: {
				Log.error(threadId, "Type not expected.\n");
                break;
			}
		}
	} else {
		Log.error(threadId, "Error! Couldn't receive data.\n");
	}
	commOps.close(&innerRemote);
	threadExit(NULL);
}

void 	sendMessagePeer(char* peerId, struct Message_t message) { //(void* vars) {
    struct commFacade_t localPeer, remotePeer;
    char *ipPeer;
    int portPeer;

    /*
    struct Message_t message;
    char *peerId = (char*) malloc((UserId_Len + 1) * sizeof(char));
    int offset;
    offset = 0;
    memcpy(peerId, vars + offset, (UserId_Len + 1) * sizeof(char));
    offset = (UserId_Len + 1) * sizeof(char);
    memcpy(&message, vars + offset, sizeof(Message_t));
    free(vars);
    //*/

    User_t  *user;
    user = requestClient(peerId);

    ipPeer = inet_ntoa(user->addr.sin_addr);
    portPeer = ntohs(user->addr.sin_port);

    if((commOps.connect(&localPeer, &remotePeer, 0, ipPeer, portPeer)) < 0) {
	    Log.error(getpid(), "Error! Couldn't connect to peer.\n");
        exit(-1);
    }
    if((sendMessage(&localPeer, message)) < 0) {
	    Log.error(getpid(), "Error! Couldn't send message to peer.\n");
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
    char *peerId = (char*) malloc((UserId_Len + 1) * sizeof(char));
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
	    Log.error(getpid(), "Error! Couldn't connect to peer.\n");
        exit(-1);
    }
    if((sendFile(&localPeer, file)) < 0) {
	    Log.error(getpid(), "Error! Couldn't send file to peer.\n");
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
    userId = (char*) malloc(UserId_Len * sizeof(char));

    int auxRtn, count = 0;
    do {
        if(count > 10) {
            Log.error(getpid(), "Error! Client couldn't Create a Server Socket.\n");
            exit(-1);
        } else {
            count++;
            basePort = rand() % 10000;
            auxRtn = 0;
            auxRtn = init_Server(&local, basePort);
        }
    } while(auxRtn < 0);

    IPC.semOpen(&sem_list_messages, basePort);
    IPC.semOpen(&sem_CanContinue, basePort + 1);
    IPC.semOpen(&sem_ServerSocket, basePort + 2);

    IPC.memOpen(&shMem_messages, basePort + 3, sizeof(struct LinkedListHead));
    messages = (Message_t *)shMem_messages.data;

    mutex_RemoteSocket = mutexInit();

    initList(&messages);
    initList(&contacts);
    initList(&groups);

    threadCount = 0;
    allowNewConnections = 1;
}

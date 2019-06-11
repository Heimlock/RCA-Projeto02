
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

void    logIn() {
    commFacade_t local, remote;
    struct SPDT_Command *command;

    fprintf(stdout, "[%.4d] | Log In Function\n", getpid());
    fflush(stdout);

    if((commOps.connect(&local, &remote, 0, ip, port)) < 0) {
        fprintf(stderr, "[%.4d] | Error! Client couldn't connect to server.\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    newCommand(&command, LogIn, UserId_Len * sizeof(char), userId);

    if((sendCommand(&local, (*command))) < 0) {
        fprintf(stderr, "[logIn] | Error! Failed to send.\n");
        fflush(stderr);
        exit(-2);
    }
    commOps.close(&local);
    commOps.close(&remote);
}

void    logOut() {
    commFacade_t local, remote;
    struct SPDT_Command *command;

    fprintf(stdout, "[%.4d] | Log Out.\n", getpid());
    fflush(stdout);

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

    fprintf(stdout, "[%.4d] | Request Client.\n", getpid());
    fflush(stdout);

    if((commOps.connect(&local, &remote, 0, ip, port)) < 0) {
        fprintf(stderr, "[%.4d] | Error! Client couldn't connect to server.\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    newCommand(&command, RequestClient, UserId_Len*sizeof(char), peerId);

    if((sendCommand(&local, (*command))) < 0) {
        fprintf(stderr, "[requestClient] | Error! Failed to send.\n");
        fflush(stderr);
        exit(-1);
    }

    dataType = receiveStruct(&local, &dataReceived);
	
    if(dataReceived != NULL){
	    switch(dataType){
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
			    fprintf(stdout, "[%.4d] | Type not expected.\n", getpid());
         		fflush(stdout);
         		break; 
		    }
	    }
    }
    else{
        fprintf(stdout, "[%.4d] | Error! Couldn't receive user.\n", getpid());
        fflush(stdout);
    }

    commOps.close(&local);
    commOps.close(&remote);
    return NULL;
}

int    canContinue(){
    int rtnValue;
    
    if(allowNewConnections) {
        rtnValue = 1;
    } else {
        rtnValue = 0;
    }
    return rtnValue;
}

void  	newReceiver(){
    threadCount++;

    if((commOps.accept(&local, &remote)) < 0) {
        fprintf(stderr, "[%.4d] | Error! Client couldn't accept connection.\n", getpid());
        fflush(stderr);
        perror("newConnection");
        exit(-4);
    }

    if(allowNewConnections) {
        fprintf(stdout, "[%.4d] | New Connection!\n", getpid());
        fflush(stdout);

        //mutexLock(mutex_remote_Rec);

        if(noResponse(attendClientPeer, threadCount) < 0) {
            fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
            fflush(stderr);
            perror("newConnection");
        }
    } else {
        commOps.close(&remote);
    }
}

void *attendClientPeer(void *arg) {
	int threadId =  (int)arg;
    //struct SPDT_Command  *command;
	struct commFacade_t  innerRemote;

    int  dataType;
    void *dataReceived;
    Message_t *message;
    File_t *file;

    fprintf(stdout, "[%.4d] | Attend Peer Init\n", threadId);
    fflush(stdout);


	memcpy(&innerRemote, &remote, sizeof(commFacade_t));
    //mutexUnlock(mutex_remote_Rec);

    dataType = receiveStruct(&innerRemote, &dataReceived);

    if(dataReceived != NULL){
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
				fprintf(stdout, "[%.4d] | Type not expected.\n", threadId);
         			fflush(stdout);
         			break; 
			}
		}
	} else {
		fprintf(stdout, "[%.4d] | Error! Couldn't receive data.\n", threadId);
        fflush(stdout);
	}

	commOps.close(&innerRemote);
	threadExit(NULL);
}

void 	sendMessagePeer(struct sockaddr_in address, struct Message_t message){
    struct commFacade_t localPeer, remotePeer;
    char *ipPeer; 
    int portPeer;

    ipPeer = inet_ntoa(address.sin_addr);
    portPeer = ntohs(address.sin_port);
      
    if((commOps.connect(&localPeer, &remotePeer, 0, ipPeer, portPeer)) < 0){
	    fprintf(stderr, "[%.4d] | Error! Couldn't connect to peer.\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    if((sendMessage(&localPeer, message)) < 0){
	    fprintf(stderr, "[%.4d] | Error! Couldn't send message to peer.\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    commOps.close(&localPeer);
    commOps.close(&remotePeer);
}

void 	sendFilePeer(struct sockaddr_in address, struct File_t file){
    struct commFacade_t localPeer, remotePeer;
    char *ipPeer; 
    int portPeer;

    ipPeer = inet_ntoa(address.sin_addr);
    portPeer = ntohs(address.sin_port);
      
    if((commOps.connect(&localPeer, &remotePeer, 0, ipPeer, portPeer)) < 0){
	    fprintf(stderr, "[%.4d] | Error! Couldn't connect to peer.\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    if((sendFile(&localPeer, file)) < 0){
	    fprintf(stderr, "[%.4d] | Error! Couldn't send file to peer.\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    commOps.close(&localPeer);
    commOps.close(&remotePeer);
}

void  initSharedData() {
    mutex_list_messages = mutexInit();

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
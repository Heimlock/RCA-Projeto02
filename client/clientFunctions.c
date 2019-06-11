
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

void  	newReceiver(){
    threadCount++;
    
    if((waitResponse(receiveFromPeer, threadCount)) == NULL) {
        fprintf(stderr, "[%.4d] | Error! Failed to create thread.\n", getpid());
        fflush(stderr);
        perror("newConnection");
    }
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

void 	*receiveFromPeer(void *args){
    int threadId = (int)args;
    struct commFacade_t localSocket, remoteSocket;
      
    void *dataReceived;
    int  dataType;
      
    Message_t *message;
    File_t *file;

    if((commOps.initServer(&localSocket, 0)) < 0){
        fprintf(stderr, "[%d] | Error! Init Socket de Client Receiver!\n", getpid());
        fflush(stderr);
        perror("receiveFromPeer");
	    threadExit(NULL);    
    }
      	
	if((commOps.accept(&localSocket, &remoteSocket)) < 0) {
        fprintf(stderr, "[%.4d] | Error! Client couldn't accept connection.\n", threadId);
        fflush(stderr);
        perror("receiveFromPeer");
        threadExit(NULL);
    }

	dataType = receiveStruct(&remoteSocket, &dataReceived);

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

	commOps.close(&localSocket);
    commOps.close(&remoteSocket);
	threadExit(NULL);
}

void  initSharedData() {
    mutex_list_messages = mutexInit();

    initList(&messages);
    initList(&contacts);
    initList(&groups);

    threadCount = 0;

    //TODO free nas listas
    if(mutex_list_messages == NULL) {
        mutexDestroy(mutex_list_messages);
        exit(-1);
    }
}
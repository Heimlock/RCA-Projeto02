
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

 void connectToServer(char *ip, int port){
    int connection = 1, command_type;
    char id[10];
    User_t *user;

    fprintf(stdout, "[%.4d] | Write your cellphone number.\n", getpid());
    fflush(stdout);
    scanf("%s", id);

    while(connection){
        if((commOps.connect(&local, &remote, 0, ip, port)) < 0){
            fprintf(stderr, "[%.4d] | Error! Client couldn't connect to server.\n", getpid());
            fflush(stderr);
            perror("connectToServer");
            exit(-3);
        }

        fprintf(stdout, "[%.4d] | LogIn (0) LogOut (1) RequestClient (2)\n", getpid());
        fflush(stdout);
        fscanf(stdin, "%d", &command_type);
        fflush(stdin);

        switch(command_type){
            case LogIn:     logIn(id);
                            commOps.close(&remote);
                            commOps.close(&local);
                            break;

            case LogOut:    logOut(id);
                            commOps.close(&remote);
                            commOps.close(&local);
                            connection = 0;
                            break;
            
            case RequestClient: user = requestClient();
                                connectToClient(user, id);
                                commOps.close(&remote);
                                commOps.close(&local);
                                free(user);
                                break;

            default:    fprintf(stdout, "[%.4d] | Command invalid.\n", getpid());
                        fflush(stdout);
                        break;
        }

        fprintf(stdout, "[%.4d] | Connect with server again?\n", getpid());
        fflush(stdout);
        fscanf(stdin, "%d", &connection);
        fflush(stdin);

        if(connection != 1){
            break;
        }  
    }

    while(whatsappCount != 0){
        wait();
    }
 }

 void logIn(char *id){
    fprintf(stdout, "[%.4d] | Log In.\n", getpid());
    fflush(stdout);
    struct SPDT_Command *command;

    newCommand(&command, LogIn, 9*sizeof(char), id);

    if((sendCommand(&local, (*command))) < 0) {
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        perror(logIn);
        exit(-4);
    }
 }

 void logOut(char *id){
    fprintf(stdout, "[%.4d] | Log Out.\n", getpid());
    fflush(stdout);
    struct SPDT_Command *command;

    newCommand(&command, LogOut, 9*sizeof(char), id);

    if((sendCommand(&local, (*command))) < 0) {
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        perror(logOut);
        exit(-5);
    }     
 }

 User_t* requestClient(){
    fprintf(stdout, "[%.4d] | Request Client.\n", getpid());
    fflush(stdout);
    struct SPDT_Command *command;
    struct User_t *user;
    char cellphone[9];

    fprintf(stdout, "[%.4d] | User cellphone number?\n", getpid());
    fflush(stdout);
    fscanf(stdin, "%s", cellphone);
    fflush(stdin);

    newCommand(&command, RequestClient, 9*sizeof(char), cellphone);

    if((sendCommand(&local, (*command))) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        perror(requestClient);
        exit(-6);
    }

    receiveStruct(&local, RequestClient, &user);

    if(user == NULL){
        fprintf(stderr, "[%d] | Error! Failed to receive user.\n", getpid());
        fflush(stderr);
        perror(requestClient);
        exit(-7);
    }

    return user;
 }

 void connectToClient(struct User_t *user, char *id){
    char *ip; 
    int port;

    ip = inet_ntoa(user->addr.sin_addr);
    port = (int) user->addr.sin_port;

    if((commOps.connect(&localPeer, &remotePeer, 0, ip, port)) < 0){
        fprintf(stderr, "[%.4d] | Error! Client couldn't connect to user.\n", getpid());
        fflush(stderr);
        perror("connectToClient");
        exit(-8);
    }

    whatsappCount++;
    mutexLock(mutex_remote);
    if((waitResponse(whatsapp, id)) == NULL) {
        fprintf(stderr, "[%.4d] | Error! Failed whatsapp.\n", getpid());
        fflush(stderr);
        perror("connectToClient");
    }

    mutexLock(mutex_remote);
    commOps.close(&remotePeer);
    commOps.close(&localPeer);
    mutexUnlock(mutex_remote);
 }

 void *whatsapp(void *args){
    char *whatsappID =  (char *) args;
    struct commFacade_t  innerLocal;
	struct commFacade_t  innerRemote;
    int operation;

    fprintf(stdout, "[%.4d] | Whatsapp peer 2 peer init\n", whatsappCount);
    fflush(stdout);

    memcpy(&innerLocal, &localPeer, sizeof(commFacade_t));
	memcpy(&innerRemote, &remotePeer, sizeof(commFacade_t));
    mutexUnlock(mutex_remote);

    while(1){
        fprintf(stdout, "[%.4d] | Receive (0) Send (1) Read(2)\n");
        fflush(stdout);
        fscanf(stdin, "%d", &operation);
        fflush(stdin);

        switch(operation){
            case 0: receiveFromClient(innerRemote);
                    break;

            case 1: sendToClient(innerRemote, whatsappID);
                    break;

            case 2: readMessage();
                    break;

            default:    fprintf(stdout, "[%.4d] | Operation invalid.\n", getpid());
                        fflush(stdout);
                        break;
        }
    }
    
    commOps.close(&innerRemote);
    commOps.close(&innerLocal);
    whatsappCount--;
	threadExit(NULL);
 }

 void sendToClient(struct commFacade_t threadCommunication, char *id){
    fprintf(stdout, "[%.4d] | Send to User.\n", getpid());
    fflush(stdout);

    struct Message_t *message;
    char mymessage[10];

    fprintf(stdout, "[%.4d] | Message: \n", getpid());
    fflush(stdout);
    fscanf(stdin, "%s", mymessage);
    fflush(stdin);

    newMessage(&message, id, 10*sizeof(char), mymessage);

    if((sendMessage(&threadCommunication, (*message))) < 0) {
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        perror("sendToClient");
        exit(-5);
    }    

    free(message);
 }

 void receiveFromClient(struct commFacade_t threadCommunication){
    fprintf(stdout, "[.4d] | Receive from User.\n", getpid());
    fflush(stdout);

    struct Message_t *message;

    receiveStruct(&threadCommunication, SendText, &message);

    if(message != NULL){
        addNode(&messages, message->senderId, message->length, message->data);
        free(message);
    } else{
        fprintf(stderr, "[%d] | Error! Couldn't receive new message.\n", getpid());
		fflush(stderr);
		perror("receiveFromClient");
    }
 }

 void   readMessage(){
    struct Message_t *message;

    if(messages == NULL){
        fprintf(stderr, "[%d] | No Messages.\n", getpid());
		fflush(stderr);
		perror("readMessage");
    } else{
        while(messages->initialNode != NULL){
            message =  (Message_t *) messages->initialNode->data; 
            
            fprintf(stdout, "Sender ID: %s\n", message->senderId);
            fflush(stdout);

            fprintf(stdout, "Message length: %d\n", message->length);
            fflush(stdout);

            fprintf(stdout, "Message: %s\n", (char *) message->data);
            fflush(stdout);

            messages->initialNode = messages->initialNode->next;

            if((removeNode(&messages, message->senderId)) < 0){
                fprintf(stderr, "[%d] | Error! Couldn't remove message.\n", getpid());
		        fflush(stderr);
		        perror("readMessage");
            }

            free(message);
        }
    }
 }

  void  initSharedData() {
    mutex_remote = mutexInit();
    mutex_list_messages = mutexInit();
    whatsappCount = 0;

    if(mutex_remote == NULL || mutex_list_messages == NULL){
        mutexDestroy(mutex_remote);
        mutexDestroy(mutex_list_messages);
        commOps.close(&local);
        exit(-2);
    }
 }
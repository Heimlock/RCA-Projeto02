
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

 void connectToServer(char *id){
    char ip[9] = "localhost";
    int port = 5000, connection = 1, command_type;
    User_t *user;

    while(connection){
        if((commOps.connect(&local, &remote, ip, port)) < 0){
            fprintf(stderr, "[%.4d] | Error! Client couldn't connect to server.\n", getpid());
            fflush(stderr);
            perror("connectToServer");
            exit(-3);
        }

        fprintf(stdout, "[%.4d] | LogIn (0) LogOut (1) RequestClient (2)\n", getpid());
        fflush(stdout);
        scanf("%d", &command_type);

        switch(command_type){
            case LogIn:     logIn(id);
                            commOps.close(&remote);
                            break;

            case LogOut:    logOut(id);
                            commOps.close(&remote);
                            connection = 0;
                            break;
            
            case RequestClient: user = requestClient();
                                commOps.close(&remote);
                                //connectToClient(user, id);
                                break;

            default:    fprintf(stdout, "[%.4d] | Command invalid.\n", getpid());
                        fflush(stdout);
                        break;
        }

        if(connection == 1){
            fprintf(stdout, "[%.4d] | Connect with server again?\n");
            scanf("%d", &connection);
        }
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

    fprintf(stdout, "[%.4d] | User cellphone?\n", getpid());
    scanf("%s", cellphone);

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

 /*
 void connectToClient(struct User_t *user, char *id){
    if((commOps.connect(&local, &remote, user->addr.sin_addr, user->addr.sin_ports)) < 0){
        fprintf(stderr, "[%.4d] | Error! Client couldn't connect to user.\n", getpid());
        fflush(stderr);
        perror("connectToClient");
        exit(-8);
    }

    whatsappCount++;
    mutexLock(mutex_remote);
    if((waitResponse(whatsapp, whatsappCount)) == NULL) {
        fprintf(stderr, "[%.4d] | Error! Failed whatsapp.\n", getpid());
        fflush(stderr);
        perror("connectToClient");
    }
 }

 void *whatsapp(void *args){
    int whatsappID =  (int)arg;
	struct commFacade_t  innerRemote;
    int operation;

    fprintf(stdout, "[%.4d] | Whatsapp peer 2 peer init\n", threadId);
    fflush(stdout);

	memcpy(&innerRemote, &remote, sizeof(commFacade_t));
    mutexUnlock(mutex_remote);

    while(1){
        fprintf(stdout, "[%.4d] | Receive (0) Send (1) Read(2)\n");
        scanf("%d", &operation);

        switch(operation){
            case 0: receiveFromClient(innerRemote);
                    break;

            case 1: sendToClient(innerRemote);
                    break;

            case 2: readMessage();
                    break;

            default:    fprintf(stdout, "[%.4d] | Operation invalid.\n", getpid());
                        fflush(stdout);
                        break;
        }
    }
    
    close_Socket(&innerRemote);
	threadExit(NULL);
 }

 void sendToClient(struct commFacade_t threadCommunication){
    fprintf(stdout, "[%.4d] | Send to User.\n", getpid());
    fflush(stdout);

    struct Message_t *message;
    char mymessage[10];

    fprintf(stdout, "[%.4d] | Message: \n", getpid());
    fflush(stdout);
    scanf("%s", mymessage);

    message = newMessage((char *) getpid(), 10*sizeof(char), mymessage);

    if((sendMessage(&threadCommunication, (*message))) < 0) {
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        perror("sendToClient");
        exit(-5);
    }    
 }

 void receiveFromCLient(strut commFacade_t threadCommunication){
    fprintf(stdout, "[.4d] | Receive from User.\n", getpid());

    struct Message_t *message;

    message = receiveStruct(&threadCommunication, SendText);

    if(message != NULL){
        if((addNode(&messages, message->senderId, message->data)) < 0){
            fprintf(stderr, "[%d] | Error! Couldn't add new message.\n", getpid());
			fflush(stderr);
			perror("receiveFromClient");
        }
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
        while(messages->initialNode->next != NULL){
            message =  (Message_t *) messages->initialNode->data; 
            
            fprintf(stdout, "Sender ID: %s\n", message->senderId);
            fflush(stdout);

            fprintf(stdout, "Message length: %d\n", message->length);
            fflush(stdout);

            fprintf(stdout, "Message: %s\n", (Char *) message->data);
            fflush(stdout);

            messages->initialNode = messages->initialNode->next;

            if((removeNode(&messages, message->senderId)) < 0){
                fprintf(stderr, "[%d] | Error! Couldn't remove message.\n", getpid());
		        fflush(stderr);
		        perror("readMessage");
            }
        }
    }
 } 
 */

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
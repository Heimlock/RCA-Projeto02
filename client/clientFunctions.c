
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

 void connectToServer(char *id){
    char ip[10] = "localhost";
    int port = 5000, connection = 1, command_type;
    User_t *user;

    while(connection){
        if((commOps.connect(&localSend, &remoteSend, 0, ip, port)) < 0){
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
                            commOps.close(&remoteSend);
                            break;

            case LogOut:    logOut(id);
                            commOps.close(&remoteSend);
                            connection = 0;
                            break;
            
            case RequestClient: user = requestClient();
                                commOps.close(&remoteSend);
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

    if((sendCommand(&localSend, (*command))) < 0) {
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

    if((sendCommand(&localSend, (*command))) < 0) {
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

    if((sendCommand(&localSend, (*command))) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        perror(requestClient);
        exit(-6);
    }

    receiveStruct(&localSend, RequestClient, &user);

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
    allowNewConnections = 1;
	childCount  = 0;
    
    mutex_remote_Send = mutexInit();
    mutex_list_messages = mutexInit();
    whatsappCount = 0;

    if(mutex_remote_Send == NULL || mutex_list_messages == NULL){
        mutexDestroy(mutex_remote_Send);
        mutexDestroy(mutex_list_messages);
        commOps.close(&localSend);
        exit(-2);
    }

    initList(&messages);
}


int canContinueClient() {
    int rtnValue;
    if(allowNewConnections) {
        rtnValue = 1;
    } else {
        rtnValue = 0;
    }
    return rtnValue;
}

void *mandarMensagem(void *arg) {
	char ip[9] = "localhost"; //por enquanto é sempre localhost
    int PortaDestino;
    int saida = 0;
    struct commFacade_t     localSend;

    while(saida != 1) {
        fprintf(stdout, "[%.4d] | Mandar mensagem Iniciado\n", getpid());
        fprintf(stdout, "[%.4d] | Porta destino da conexao:\n", getpid());
        fflush(stdout);
        fflush(stdin);
        scanf("%d", &PortaDestino);

        //Se digitar 0 na porta Destino, sair do while, fechar thread.
        if(PortaDestino == 0) {
            saida = 1;
            break;
        }else if(PortaDestino == 1){
            //Se digitar 1, ler mensagens na lista ligada
            
            struct LinkedListNode* nodeMensagem = getNode(messages, 1);
            char *msg = nodeMensagem->data;

            //buf[numbytes] = '\0';    
            printf("client: received '%s'\n",msg);
            //Message_t *mensagem;
            //mensagem.length = nodeMensagem->length;
            //mensagem.data = nodeMensagem->data;
            //mensagem.senderId = 10;
            //printMsg(mensagem); 
                        
        }

        //Criação do socket que envia mensagens, tipo cliente
        fprintf(stdout, "\n");
        fprintf(stdout, "[%d] | Socket de Enviamento\n", getpid());
        fflush(stdout);

        //Conect com a PortaDestino
        if((commOps.connect(&localSend, &remoteSend, 0, ip, PortaDestino)) < 0) {
            fprintf(stderr, "[%.4d] | Error! Client couldn't connect to peer.\n", getpid());
            fflush(stderr);
            perror("connectToPeer");
            exit(-3);
        }

        fprintf(stdout, "[%.4d] | Mandando mensagem\n", getpid());
        fflush(stdout);

        //Mandar dados
        char *msg = "Quidquid latine dictum sit, altum viditur.";
        char *userId = "982660910";
        int len, bytes_sent;
        Message_t *message;
        len = strlen(msg);

        newMessage(&message, userId, len, msg);
        if(sendMessage(&localSend, *message) < 0) {
            fprintf(stderr, "[%.4d] | Error! On send Message.\n", getpid());
            fflush(stderr);
            exit(-3);
        }

        //Fechar Socket
        fprintf(stdout, "[%.4d] | Mensagem enviada, fechando socket\n", getpid());
        fflush(stdout);
        close_Socket(&localSend);
    }
	threadExit(NULL);
}


void *attendClientPeer(void *arg) {
	//FIXME nao tem mutex
    int threadId =  (int)arg;
    struct SPDT_Command  *command; //FIXME essa variavel nao é usada? XD
	struct commFacade_t  innerRemote;

    fprintf(stdout, "[%.4d] | Attend Peer Init\n", threadId);
    fflush(stdout);

	memcpy(&innerRemote, &remoteRec, sizeof(commFacade_t));
    mutexUnlock(mutex_remote_Rec);

    //Receber data
    Message_t *message;
    receiveStruct(&innerRemote, SendText, &message);
    printMsg(*message);
    addNode(&messages, 1, sizeof(message), message);
    //Fim de receber data

    close_Socket(&innerRemote);
	threadExit(NULL);
}

void newConnectionClient() {

    if((commOps.accept(&localRec, &remoteRec)) < 0) {
        fprintf(stderr, "[%.4d] | Error! Client couldn't accept connection.\n", getpid());
        fflush(stderr);
        perror("newConnection");
        exit(-4);
    }

    if(allowNewConnections) {
        fprintf(stdout, "[%.4d] | New Connection!\n", getpid());
        fflush(stdout);

        childCount++;
        mutexLock(mutex_remote_Rec);

        if(noResponse(attendClientPeer, childCount) < 0) {
            fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
            fflush(stderr);
            perror("newConnection");
        }
    } else {
        commOps.close(&remoteRec);
    }
}


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
                                //connectToClient(user);
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

    command = newCommand(LogIn, 9*sizeof(char), id);

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

    command = newCommand(LogOut, 9*sizeof(char), id);

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

    command = newCommand(RequestClient, 9*sizeof(char), cellphone);

    if((sendCommand(&local, (*command))) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        perror(requestClient);
        exit(-6);
    }

    user = receiveStruct(&local, RequestClient);

    if(user == NULL){
        fprintf(stderr, "[%d] | Error! Failed to receive user.\n", getpid());
        fflush(stderr);
        perror(requestClient);
        exit(-7);
    }

    return user;
 }

 /*void connectToClient(struct User_t *user){
    if((commOps.connect(&local, &remote, user->addr.sin_addr, user->addr.sin_ports)) < 0){
        fprintf(stderr, "[%.4d] | Error! Client couldn't connect to user.\n", getpid());
        fflush(stderr);
        perror("connectToClient");
        exit(-8);
    }
 }*/

 void  initSharedData() {
    mutex_remote = mutexInit();
    mutex_list_messages = mutexInit();

    if(mutex_remote == NULL || mutex_list_messages == NULL){
        mutexDestroy(mutex_remote);
        mutexDestroy(mutex_list_messages);
        commOps.close(&local);
        exit(-2);
    }
}
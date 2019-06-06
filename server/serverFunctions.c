
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

void newConnection() {
    commOps.accept(&commData);

    if(allowNewConnections){
        fprintf(stdout, "[%.4d] | New Connection!\n", getpid());
        fflush(stdout);
    
        childCount++;

        if(noResponse(attendClient, childCount) < 0){
            fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
            fflush(stderr);
            perror("logIn");
        }

    } else{
        commOps.close(&commData);
    }
}

void *attendClient(void *arg) {
	int threadId =  (int)arg;
    struct SPDT_Command  *command;
	struct commFacade_t  commDataLocal;

    fprintf(stdout, "[%.4d] | Attend Client Init\n", threadId);
    fflush(stdout);

	memcpy(&commDataLocal, &commData, sizeof(commFacade_t));    //criar CommFacade localmente

    command = receiveCommand(&commData);    //Command: ActionType, length, Value

    if(command == NULL) {
        fprintf(stderr, "[%.4d] | Error! Command is NULL.\n", threadId);
        fflush(stderr);
    } else {
        switch(command->type) {
            case LogIn:
                logIn(commDataLocal, command);
                break;
            case LogOut:
                logOut(commDataLocal, command);
                break;
            case RequestClient:
                requestClient(commDataLocal, command);
                break; 
            default:
                fprintf(stderr, "[%.4d] | Error! Not a Valid Type. Type = %d\n", threadId, command->type);
                fflush(stderr);
                break;
        }
    }
    close_Remote(&commData);
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
            commOps.close(&commData);
            done = 1;
        }
    } while(!done);
}

void  initSharedData() {
	allowNewConnections = 1;
	childCount  = 0;
    acceptNewConnections = 1;
}

void  logIn(struct commFacade_t communication_data, struct SPDT_Command *log_in) {
	struct User_t *user;
    struct LinkedListNode *auxUser;

    #ifdef  DEBUG
        fprintf(stdout, "[%d] | LogIn Function Init\n", getpid());
        fflush(stdout);
    #endif

    if(log_in->value != NULL) { //ID do usuario recebido pelo comando (Telefone)
        auxUser = getNode(&users, (char *) log_in->value);
        if(auxUser != NULL) {
            //TODO Usuario cadastrado na fila (Editar Usuario)
        } else {    //  Cadastrar novo usuario
            user = newUser((char*) log_in->value, communication_data.remote, Online);
			if(addNode(&users, user->id, user) < 0) {
				fprintf(stderr, "[%d] | Error! Couldn't add new user.\n", getpid());
				fflush(stderr);
				perror("logIn");
			}
        }
    } else {
        fprintf(stderr, "[%d] | Error! User couldn't log in.\n", getpid());
        fflush(stderr);
        perror("logIn");
    }
}

void    logOut(struct commFacade_t communication_data, struct SPDT_Command *log_out) {
    struct  LinkedListNode *auxUser;

    if(log_out->value != NULL){
        auxUser = getNode(&users, (char *) log_out->value);
        if(auxUser != NULL) {
			//TODO editar usuario ja existente (node)
        } else {
            fprintf(stderr, "[%d] | Error! User doens't exist.\n", getpid());
            fflush(stderr);
            perror("logOut");
        }
    } else {
        fprintf(stderr, "[%d] | Error! User couldn't log out.\n", getpid());
        fflush(stderr);
        perror("logOut");
    }
}

void	requestClient(struct commFacade_t communication_data, struct SPDT_Command *request_user) {
	struct User_t *user;
	struct LinkedListNode *auxUser;

	if(request_user->value != NULL) {
		auxUser = getNode(&users, (char*) request_user->value);     //Recuperar usuario pelo ID
        if(auxUser != NULL) {
            user = (User_t *) auxUser->data;
            if(user->state == Online) {  //Enviar usuario Online
                if(sendUser(&communication_data, (*user)) < 0) {
                    fprintf(stderr, "[%d] | Error! Failed to send user requested.\n", getpid());
                    fflush(stderr);
                    perror("requestClient");
                }
            } else {
                fprintf(stderr, "[%d] | Error! User not online.\n", getpid());
                fflush(stderr);
                perror("requestClient");
            }
        } else {
            fprintf(stderr, "[%d] | Error! User doens't exist.\n", getpid());
            fflush(stderr);
            perror("requestClient");
        }
	} else {
		fprintf(stderr, "[%d] | Error! Client ID not received.\n", getpid());
        fflush(stderr);
        perror("requestClient");
	}
}
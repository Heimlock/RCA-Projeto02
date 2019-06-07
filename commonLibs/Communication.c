
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Desenvolvimento de Recursos Referentes a Comunicacao 
 */

#include "./Communication.h"

/*
 *  SPDT Related
 */


/*
 *  Funcao que envia um SPDT_Command via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *      @command    ==  SPDT_Command
 *  Retornos:
 *      > 0         ==  Erro
 *      < 0         ==  Numero de Bytes Enviados
 */
int sendCommand(struct commFacade_t* commData, struct SPDT_Command command) {
    void*   dataOut = command2bytes(command);
    #ifdef DEBUG
        printBytes(getpid(), dataOut, (3+command.length));
    #endif
    return sendData(commData, dataOut, (3+command.length));
}

/*
 *  Funcao que recebe um SPDT_Command via socket
 *  Argumentos:
 *     @commData   ==  Communication Facade
 *  Retornos:
 *     NULL         ==  Erro
 *     SPDT_Command*==  Ponteiro para um SPDT_Command
 */
struct SPDT_Command* receiveCommand(struct commFacade_t* commData) {
    int     type;
    int     length;
    void*   header = malloc(3*sizeof(char));
    void*   value;
    void*   commandData;
    struct SPDT_Command* command;
    struct SPDT_Command* auxCommand;

    if(-1 != receiveData(commData, &header, 3*sizeof(char))) {
	auxCommand = bytes2commandHeader(header);
        fprintf(stdout, "Type: %d, Length: %d\n", auxCommand->type, auxCommand->length);
        fflush(stdout);
        if(auxCommand->length > 0) {
            fprintf(stdout, "Init Receive Data\n");
            fflush(stdout);

            value = malloc(auxCommand->length * sizeof(char));
            commandData    = malloc((3 * sizeof(char)) + auxCommand->length);
            memcpy(header, commandData, (3 * sizeof(char)));
            fprintf(stdout, "Receive Data\n");
            fflush(stdout);
            if(-1 != receiveData(commData, &value, auxCommand->length)) {
                printBytes(getpid(), value, auxCommand->length);
                memcpy(value, ((3 * sizeof(char)) + commandData), auxCommand->length);
                command = bytes2command(commandData);
                free(value);
            } else {
                perror("[receiveCommand] | Receive Data");
                return NULL;
            }
            free(header);
            free(commandData);
        }
    } else {
        perror("[receiveCommand] | Receive Header");
        return NULL;
    }
    #ifdef DEBUG
	printCommand(*command);
    #endif
    return command;
}


/*
 *  Funcao que recebe um Tipo Abstrato via socket
 *  Argumentos:
 *      @commData    ==  Communication Facade
 *      @expectedType==  Tipo Abstrato a Receber
 *  Retornos:
 *     NULL          ==  Erro
 *     void*         ==  Ponteiro para o Tipo Abstrato
 */
void*   receiveStruct(struct commFacade_t* commData, enum ActionType expectedType) {
    struct SPDT_Command* dataReceived = receiveCommand(commData);
    void*   outputData;

    switch (dataReceived->type) {
    case RequestClient:
        outputData = (void*) bytes2User(dataReceived->value);
        break;
    case SendText:
        outputData = (void*) bytes2Message(dataReceived->value);
        break;
    case SendFile:
        outputData = (void*) bytes2File(dataReceived->value);
        break;
    default:
        perror("Not a Valid Type!\n");
        outputData = NULL;
        break;
    }

    if(expectedType != dataReceived->type) {
        fprintf(stderr, "Type not Expected!\n Expected: %d, Received: %d\n", expectedType, dataReceived->type);
        fflush(stderr);
    }
    return outputData;
}

/*
 *  UserData Related
 */


/*
 *  Funcao que envia um User_t via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *      @user       ==  User_t
 *  Retornos:
 *      > 0         ==  Erro
 *      < 0         ==  Numero de Bytes Enviados
 */
int sendUser(struct commFacade_t* commData, struct User_t user) {
    void*   dataOut = user2Bytes(user);
    struct SPDT_Command* userCommand = newCommand(RequestClient, UserData_Len, dataOut);
    return sendCommand(commData, *userCommand);
}

/*
 *  MessageData Related
 */


/*
 *  Funcao que envia um Message_t via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *      @message    ==  Message_t
 *  Retornos:
 *      > 0         ==  Erro
 *      < 0         ==  Numero de Bytes Enviados
 */
int sendMessage(struct commFacade_t* commData, struct Message_t message) {
    void*   dataOut = message2Bytes(message);
    int messageLength = UserId_Len + 2 + message.length;
    struct SPDT_Command* msgCommand = newCommand(SendText, messageLength, dataOut);
    return sendCommand(commData, *msgCommand);
}

/*
 *  FileData Related
 */


/*
 *  Funcao que envia um File_t via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *      @file       ==  File_t
 *  Retornos:
 *      > 0         ==  Erro
 *      < 0         ==  Numero de Bytes Enviados
 */
int sendFile(struct commFacade_t* commData, struct File_t file) {
    void*   dataOut = file2Bytes(file);
    int     fileLength = UserId_Len + 2 + file.nameLength + 2 + file.length;
    struct SPDT_Command* fileCommand = newCommand(SendFile, fileLength, dataOut);
    return sendCommand(commData, *fileCommand);
}

void printBytes(int id, void *bytes, int length) {
    char* auxValue = bytes;
    for( int i = 0; i< length; i++ ) {
        fprintf(stdout,"[%.4d] | Value[%d]: 0x%02hhX\n", id, i, auxValue[i]);
        fflush(stdout);
    }
}

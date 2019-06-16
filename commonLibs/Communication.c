
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
#include "./CustomStreams.h"

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

    Log.debug(getpid(), "Type: %d, Length: %d\n", command.type, command.length);
    printBytes(getpid(), dataOut, (3+command.length));

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
void receiveCommand(struct commFacade_t* commData, struct SPDT_Command** command) {
    int     offset;
    void*   header = malloc(3*sizeof(char));
    void*   value;
    void*   commandData;
    struct SPDT_Command* auxCommand;

    if(-1 != receiveData(commData, &header, 3*sizeof(char))) {
	    bytes2commandHeader(header, &auxCommand);
        if(auxCommand->length > 0) {
            value = malloc(auxCommand->length * sizeof(char));
            commandData    = malloc((3 * sizeof(char)) + auxCommand->length);
            if(-1 != receiveData(commData, &value, auxCommand->length)) {
                offset = 0;
                memcpy(commandData, header, (3 * sizeof(char)));
                offset = (3 * sizeof(char));
                memcpy((commandData + offset), value, auxCommand->length);
                bytes2command(commandData, command);
                free(value);
            } else {
                perror("[receiveCommand] | Receive Data");
                (*command) = NULL;
            }
            free(header);
            free(commandData);
        }
    } else {
        perror("[receiveCommand] | Receive Header");
        (*command) = NULL;
    }
    #ifdef DEBUG
        if((*command) != NULL) {
	        printCommand(getpid(), **command);
        }
    #endif
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
int receiveStruct(struct commFacade_t* commData, void** outputData) {
    struct SPDT_Command* dataReceived;
    receiveCommand(commData, &dataReceived);

    switch (dataReceived->type) {
    case RequestClient:
        bytes2User((User_t **)outputData, dataReceived->value);
        return RequestClient;
    case SendText:
        bytes2Message((Message_t **)outputData, dataReceived->value);
        return SendText;
    case SendFile:
        bytes2File((File_t **)outputData, dataReceived->value);
        return SendFile;
    default:
        Log.error(getpid(), "Not a Valid Type.\n");
        return -1;
    }
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
    struct SPDT_Command* userCommand;
    newCommand(&userCommand, RequestClient, UserData_Len, dataOut);
    return sendCommand(commData, *userCommand);
    //  Destroy Command
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
    struct SPDT_Command* msgCommand;
    newCommand(&msgCommand, SendText, messageLength, dataOut);
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
    struct SPDT_Command* fileCommand;
    newCommand(&fileCommand, SendFile, fileLength, dataOut);
    return sendCommand(commData, *fileCommand);
}

void printBytes(int id, void *bytes, int length) {
    char* auxValue = bytes;
    for( int i = 0; i< length; i++ ) {
        Log.debug(id,"Value[%d]: 0x%02hhX\n", i, auxValue[i]);
    }
}

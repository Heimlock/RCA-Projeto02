
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
int sendCommand(commFacade_t* commData, SPDT_Command command) {
    void*   dataOut = command2bytes(*command);
    return sendData(commData, dataOut, (3+comman.length));
}

/*
 *  Funcao que recebe um SPDT_Command via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *  Retornos:
 *     NULL         ==  Erro
 *     SPDT_Command*==  Ponteiro para um SPDT_Command
 */
SPDT_Command* receiveCommand(commFacade_t* commData) {
    void*   header = malloc(3*sizeof(char));
    void*   data;
    SPDT_Command* command;
    if(receiveData(commData, header, 3*sizeof(char)) > 0) {
        command = bytes2commandHeader(header);
        if(command->length > 0) {   
            data    = malloc(command->length);
            if(receiveData(commData, data, command->length) > 0) {
                command->data = data;
            } else {
                perror("[receiveCommand] | Receive Data.");
                return NULL;
            }
        }
    } else {
        perror("[receiveCommand] | Receive Header.");
        return NULL;
    }
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
void*   receiveStruct(commFacade_t* commData, ActionType expectedType) {
    SPDT_Command* dataReceived = receiveCommand(commData);
    void*   outputData;

    switch (dataReceived.type) {
    case RequestClient:
        outputData = bytes2User(dataReceived.data);
        break;
    case SendText:
        outputData = bytes2Message(dataReceived.data);
        break;
    case SendFile:
        outputData = bytes2File(dataReceived.data);
        break;
    default:
        perror("Not a Valid Type!\n");
        outputData = NULL;
        break;
    }

    if(expectedType != dataReceived.type) {
        perror("Type not Expected!\n Expected: %d, Received: %d\n", expectedType, dataReceived.type);
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
int sendUser(commFacade_t* commData, User_t user) {
    void*   dataOut = user2Bytes(user);
    SPDT_Command* userCommand = newCommand(ActionType.sendUser, UserData_Len, dataOut);
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
int sendMessage(commFacade_t* commData, Message_t message) {
    void*   dataOut = message2Bytes(message);
    int messageLength = SENDERID_LEN + 2 + message.length;
    SPDT_Command* msgCommand = newCommand(ActionType.sendMessage, messageLength, dataOut);
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
int sendFile(commFacade_t* commData, File_t file) {
    void*   dataOut = file2Bytes(file);
    int     fileLength = SENDERID_LEN + 2 + file.nameLength + 2 + file.length;
    SPDT_Command* fileCommand = newCommand(ActionType.sendMessage, fileLength, dataOut);
    return sendCommand(commData, *fileCommand);
}

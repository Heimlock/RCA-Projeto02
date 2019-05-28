
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
 *  Função que envia um SPDT_Command via socket
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
 *  Função que recebe um SPDT_Command via socket
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
 *  UserData Related
 */


/*
 *  Função que envia um User_t via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *      @user       ==  User_t
 *  Retornos:
 *      > 0         ==  Erro
 *      < 0         ==  Numero de Bytes Enviados
 */
int sendUser(commFacade_t* commData, User_t user) {
    void*   dataOut = user2Bytes(user);
    return sendData(commData, dataOut, UserData_Len);
}

/*
 *  Função que recebe um User_t via socket
 *  Argumentos:
 *      @commData   ==  Communication Facade
 *  Retornos:
 *     NULL         ==  Erro
 *     User_t*      ==  Ponteiro para um User_t
 */
User_t* receiveUser(commFacade_t* commData) {
    void*   dataIn = malloc(UserData_Len);
    User_t* user;
    if(receiveData(commData, dataIn, UserData_Len) > 0) {
        user = bytes2User(dataIn);
    } else {
        perror("[receiveUser] | Receive User.");
        return NULL;
    }
    return user;
}


/*
 *  MessageData Related
 */
int sendMessage(commFacade_t* commData, Message_t message);
Message_t* receiveMessage(commFacade_t* commData);


/*
 *  FileData Related
 */
int sendFile(commFacade_t* commData, File_t file);
File_t* receiveFile(commFacade_t* commData);

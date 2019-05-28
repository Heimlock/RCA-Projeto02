
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Desenvolvimento dos Recursos Referentes as Mensagens
 */

#include "./UserData.h"
#include "./MessageData.h"

Message_t* newMessage(char* senderId, int length, void* data) {
    Message_t* newMsg = (Message_t*) malloc(sizeof(Message_t));
    memcpy(newMsg->senderId, senderId, UserId_Len);
    newMsg->length = length;
    newMsg->data = malloc(length);
    memcpy(newMsg->data, data, length);
    #ifdef DEBUG
        printMsg(*newMsg);
    #endif
    return newMsg;
}

char*   message2Bytes(Message_t message) {
    int     dataSize = UserId_Len + sizeof(int) + message.length;
    int     offset = 0;
    void*   dataOut = malloc(dataSize);
    offset  = 0;
    memcpy(dataOut + offset, message.data,   UserId_Len);
    offset += UserId_Len;
    memcpy(dataOut + offset, message.length, sizeof(int));
    offset += sizeof(int);
    memcpy(dataOut + offset, message.data,   message.length);
    return dataOut;
}

Message_t* bytes2Message(char* data) {
    Message_t* newMsg;
    int     offset = 0;
    offset  = 0;
    memcpy(newMsg.data,  data + offset, UserId_Len);
    offset += UserId_Len;
    memcpy(newMsg.length,data + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(newMsg.data,  data + offset, newMsg.length);
    #ifdef DEBUG
        printMsg(*newMsg);
    #endif
    return newMsg;
}

void printMsg(Message_t message) {
    fprintf(stdout, "[%d] | Sender Id: %s\n", getpid(), message.senderId);
    fprintf(stdout, "[%d] | Length...: %d\n", getpid(), message.length);
    fprintf(stdout, "[%d] | Data.....: %s\n", getpid(), message.data);
    fflush(stdout);
}

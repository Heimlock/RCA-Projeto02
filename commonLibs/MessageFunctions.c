
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

void newMessage(Message_t** message, char* senderId, int length, void* data) {
    (*message) = (Message_t*) malloc(sizeof(Message_t));
    memcpy((*message)->senderId, senderId, UserId_Len);
    (*message)->length = length;
    (*message)->data = malloc(length);
    memcpy((*message)->data, data, length);
    #ifdef DEBUG
        printMsg(*(*message));
    #endif
}

char*   message2Bytes(Message_t message) {
    int     dataSize = UserId_Len + sizeof(int) + message.length;
    int     offset = 0;
    void*   dataOut = malloc(dataSize);
    offset  = 0;

    memcpy(dataOut + offset, message.senderId,   UserId_Len);
    offset += UserId_Len;

    memcpy(dataOut + offset, &message.length, sizeof(int));
    offset += sizeof(int);

    memcpy(dataOut + offset, message.data,   message.length);
    return dataOut;
}

void bytes2Message(Message_t**  message, char* data) {
    (*message) = (Message_t*) malloc(sizeof(Message_t));

    int     offset;
    offset  = 0;

    memcpy((*message)->senderId,  data + offset, UserId_Len);
    offset += UserId_Len;

    memcpy(&(*message)->length,data + offset, sizeof(int));
    offset += sizeof(int);

    (*message)->data = malloc((*message)->length);
    memcpy((*message)->data,  data + offset, (*message)->length);

    #ifdef DEBUG
        printMsg(*(*message));
    #endif
}

void printMsg(Message_t message) {
    fprintf(stdout, "[%d] | Sender Id: %s\n", getpid(), message.senderId);
    fprintf(stdout, "[%d] | Length...: %d\n", getpid(), message.length);
    fprintf(stdout, "[%d] | Data.....: %s\n", getpid(), (char*) message.data);
    fflush(stdout);
}

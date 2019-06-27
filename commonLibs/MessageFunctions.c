
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *      Gabriela Ferreira Jorge       RA: 12228441
 *		Rodrigo da Silva Cardoso      RA: 16430126
 *
 *  Desenvolvimento dos Recursos Referentes as Mensagens
 */

#include "./UserData.h"
#include "./MessageData.h"
#include "./CustomStreams.h"

void newMessage(Message_t** message, char* senderId, int length, void* data) {
    (*message) = (Message_t*) malloc(sizeof(Message_t));

    (*message)->senderId = (char *) malloc((SenderId_Len + 1) * sizeof(char));
    memcpy((*message)->senderId, senderId, SenderId_Len);
    (*message)->senderId[SenderId_Len] = '\0';

    (*message)->length = length;
    (*message)->data = malloc(length);
    memcpy((*message)->data, data, length);
    #ifdef DEBUG
    if((*message) != NULL) {
        printMsg(*(*message));
    }
    #endif
}

char*   message2Bytes(Message_t message) {
    int     dataSize = SenderId_Len + sizeof(int) + message.length;
    int     offset = 0;
    void*   dataOut = malloc(dataSize);
    offset  = 0;

    memcpy(dataOut + offset, message.senderId,   SenderId_Len);
    offset += SenderId_Len;

    memcpy(dataOut + offset, &message.length, sizeof(int));
    offset += sizeof(int);

    memcpy(dataOut + offset, message.data,   message.length);
    return dataOut;
}

void bytes2Message(Message_t**  message, char* data) {
    (*message) = (Message_t*) malloc(sizeof(Message_t));

    int     offset;
    offset  = 0;

    (*message)->senderId = (char *) malloc((SenderId_Len + 1) * sizeof(char));
    memcpy((*message)->senderId,  data + offset, SenderId_Len);
    (*message)->senderId[SenderId_Len] = '\0';
    offset += SenderId_Len;

    memcpy(&(*message)->length,data + offset, sizeof(int));
    offset += sizeof(int);

    (*message)->data = malloc((*message)->length);
    memcpy((*message)->data,  data + offset, (*message)->length);

    #ifdef DEBUG
    if((*message) != NULL) {
        printMsg(*(*message));
    }
    #endif
}

void printMsg(Message_t message) {
    Log.info(getpid(), "Sender Id: %s\n", message.senderId);
    Log.info(getpid(), "Length...: %d\n", message.length);
    Log.info(getpid(), "Data.....: %s\n", (char*) message.data);
}

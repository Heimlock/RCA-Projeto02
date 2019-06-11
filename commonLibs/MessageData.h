
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca de Recursos Referentes as Mensagens
 */

#ifndef Message_Def
#define Message_Def

#define SenderId_Len 9

typedef struct Message_t {
    char*   senderId;
    int     length;
    void*   data;
} Message_t;

void    newMessage(Message_t**  message, char* senderId, int length, void* data);
char*   message2Bytes(Message_t message);
void    bytes2Message(Message_t**  message, char* data);
void    printMsg(Message_t message);
#endif

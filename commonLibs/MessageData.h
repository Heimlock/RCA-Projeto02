
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

/*
 *  #define SENDERID_LEN 9  //  Implementar Aonde for Usado  
 */

#ifndef Message_Def
#define Message_Def

typedef struct Message_t {
    char*   senderId;
    int     length;
    void*   data;
} Message_t;

Message_t* newMessage(char* senderId, int length, void* data);
char*   message2Bytes(Message_t message);
Message_t* bytes2Message(char* data);
void printMsg(Message_t message);
#endif

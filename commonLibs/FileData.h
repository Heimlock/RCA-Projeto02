
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca de Recursos Referentes a Manipulacao 
 *  de Arquivos
 */

#ifndef File_Def
#define File_Def

typedef struct File_t {
    char*   senderId;
    int     nameLength;
    char*   name;
    int     length;
    void*   data;
} File_t;

void  disk2Memory();
void  memory2Disk();
void  sendFile();
void  receiveFile();
#endif



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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FileName_Len    9

typedef struct File_t {
    char*   senderId;
    int     nameLength;
    char*   name;
    int     length;
    void*   data;
} File_t;

char* file2Bytes(File_t file);
File_t* bytes2File(char* data);
void printFile(File_t file);

File_t*  disk2Memory(char* filePath, char* userId);
int memory2Disk(File_t file);
#endif


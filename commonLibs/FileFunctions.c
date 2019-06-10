
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

#include "./FileData.h"
#include "./UserData.h"

char* file2Bytes(File_t file) {
    int     dataSize = UserId_Len + sizeof(int) + file.nameLength + sizeof(int) + file.length;
    int     offset = 0;
    void*   dataOut = malloc(dataSize);

    offset  = 0;
    memcpy(dataOut + offset, file.senderId, UserId_Len);
    offset += UserId_Len;
    memcpy(dataOut + offset, &file.nameLength, sizeof(int));
    offset += sizeof(int);
    memcpy(dataOut + offset, file.name,   file.nameLength);
    offset += file.nameLength;
    memcpy(dataOut + offset, &file.length, sizeof(int));
    offset += sizeof(int);
    memcpy(dataOut + offset, file.data,   file.length);

    return dataOut;
}

void bytes2File(File_t** newFile, char* data) {
    (*newFile) = (File_t*) malloc(sizeof(File_t));
    int     offset;

    offset = 0;
    (*newFile)->senderId = (char*) malloc(UserId_Len);
    memcpy((*newFile)->senderId, data + offset, UserId_Len);
    offset += UserId_Len;

    memcpy(&(*newFile)->nameLength, data + offset, sizeof(int));
    offset += sizeof(int);

    (*newFile)->name = (char*) malloc((*newFile)->nameLength);
    memcpy((*newFile)->name, data + offset, (*newFile)->nameLength);
    offset += (*newFile)->nameLength;

    memcpy(&(*newFile)->length, data + offset, sizeof(int));
    offset += sizeof(int);

    (*newFile)->data = malloc((*newFile)->length);
    memcpy((*newFile)->data, data + offset, (*newFile)->length);

    #ifdef DEBUG
        printFile(*(*newFile));
    #endif
}

void printFile(File_t file) {
    fprintf(stdout, "[%d] | Sender Id.: %s\n", getpid(), file.senderId);
    fprintf(stdout, "[%d] | NameLength: %d\n", getpid(), file.nameLength);
    fprintf(stdout, "[%d] | Name......: %s\n", getpid(), file.name);
    fprintf(stdout, "[%d] | Length....: %d\n", getpid(), file.length);
    fflush(stdout);
}

/*
 *  Função que Le um arquivo no Disco
 *  Argumentos:
 *      @filePath   ==  Caminho do Arquivo
 *      @userId     ==  ID do User
 *  Retornos:
 *      NULL        ==  Erro
 *      File_t*     ==  Ponteiro do File_t
 */
void disk2Memory(File_t** newFile, char* filePath, char* userId) {
    (*newFile) = (File_t*) malloc(sizeof(File_t));

    //  SendId
    (*newFile)->senderId = (char *)malloc((UserId_Len + 1) * sizeof(char));
    memcpy((*newFile)->senderId, userId, UserId_Len);
    (*newFile)->senderId[UserId_Len] = '\0';

    //  File Name
    (*newFile)->nameLength = FileName_Len;
    (*newFile)->name = (char *)malloc(((*newFile)->nameLength + 1) * sizeof(char));
    memcpy((*newFile)->name, basename(filePath), (*newFile)->nameLength);
    (*newFile)->name[(*newFile)->nameLength] = '\0';

    //  Open File
    FILE *fp = fopen((*newFile)->name, "rb");
    if (fp == NULL) {
        fprintf(stderr, "[disk2Memory] | Error! Can't open file\n");
        fflush(stderr);
        (*newFile) =  NULL;
        //Destroy file
        return;
    }

    //  File Size Discover
    fseek(fp, 0L, SEEK_END);
    (*newFile)->length = ftell(fp);
    rewind(fp);

    //  File Data
    (*newFile)->data = malloc((*newFile)->length);
    if(fread((*newFile)->data, (*newFile)->length, 1, fp) != (*newFile)->length) {
        fprintf(stderr, "[disk2Memory] | Error! Can't Read File\n");
        fflush(stderr);
        fclose(fp);
        (*newFile) =  NULL;
        //Destroy file
        return;
    }

    fclose(fp);
    #ifdef  DEBUG
        printFile(*(*newFile));
    #endif
}

/*
 *  Função que Escreve um arquivo no Disco
 *  Argumentos:
 *      @file       ==  File_t A ser Escrito
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int memory2Disk(File_t file) {
    FILE *fp = fopen(file.name, "wb");
    if (fp == NULL) {
        fprintf(stderr, "[disk2Memory] | Error! Can't open file\n");
        fflush(stderr);
        return -1;
    }
    if (fwrite(file.data, file.length, 1, fp) != 1) {
        fprintf(stderr, "[disk2Memory] | Error! Can't Write File\n");
        fflush(stderr);
        return -2;
    }
    fclose(fp);
    //Destroy file
    return 0;
}
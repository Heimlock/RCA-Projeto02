
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca dos Recursos Referentes a Entrada e Saida de Dados
 */

#ifndef Log_Def
#define Log_Def

#define LOG_COLLORED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 *  Log Related Definitions
 */
typedef enum LogLevel {
    Log_Debug   = 0x00,
    Log_Fine    = 0x01,
    Log_Info    = 0x02,
    Log_Error   = 0x99,
    Log_Plain   = 0xFF,
} LogLevel;

typedef struct LogOps {
    void (*setLevel)(enum LogLevel level);
    void (*error)   (int origin, const char *format, ...);
    void (*fine)    (int origin, const char *format, ...);
    void (*info)    (int origin, const char *format, ...);
    void (*debug)   (int origin, const char *format, ...);
    void (*plain)   (const char *format, ...);
} LogOps;
extern struct LogOps Log;

typedef struct UserInput {
    void (*getChar) (char* message, char* value);
    void (*getInt)  (char* message, int* value);
    void (*getStr)  (char* message, char** value, int maxStrLen);
    void (*get)     (char* message, const char *fmt, ...);
} UserInput;
extern struct UserInput Input;

#endif

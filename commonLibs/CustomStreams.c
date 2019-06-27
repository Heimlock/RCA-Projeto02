
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Desenvolvimento dos Recursos Referentes a Entrada e Saida de Dados
 */

#pragma GCC diagnostic ignored "-Wint-conversion"

#include "./CustomStreams.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <stdarg.h>
#include <string.h>


/*
 *  Log General Configs
 */
#ifdef  DEBUG
    static LogLevel LogCurrentLevel = Log_Debug;
#else
    #ifndef LogLevel
        static LogLevel LogCurrentLevel = Log_Info;
    #endif
#endif

static char *LogLevel_Colors[] = {
  "\x1b[94m",   //  [00]
  "\x1b[36m",   //  [01] Cyan
  "\x1b[34m",   //  [02] Bold Blue
  "\x1b[32m",   //  [03] Bold Green
  "\x1b[33m",   //  [04] Bold Yellow
  "\x1b[31m",   //  [05] Red
  "\x1b[35m"    //  [06] Bold Magenta
};

/*
 *  Log Related Functions
 */

void setLogLevel(enum LogLevel level) {
    LogCurrentLevel = level;
}

char* pickCollor(enum LogLevel level) {
    switch (level) {
        case Log_Debug:
            return LogLevel_Colors[1];
        case Log_Fine:
            return LogLevel_Colors[2];
        case Log_Info:
            return LogLevel_Colors[3];
        case Log_Error:
            return LogLevel_Colors[5];
        default:
            return "\x1b[0m";   //  Reset
    }
}

void stdPrint(LogLevel level, int collored, FILE *file, const char *format, ...) {
    va_list args;
    if(level < LogCurrentLevel) {
        return;
    } else {
        flockfile(file);
        #ifdef  LOG_COLLORED
            if(collored == 1) {
                fprintf(file, "%s", pickCollor(level));
            }
        #endif
        va_start(args, format);
        vfprintf(file, format, args);
        va_end(args);
        fflush(file);
        #ifdef  LOG_COLLORED
            if(collored == 1) {
                fprintf(file, "%s", pickCollor(-1));
            }
        #endif
        funlockfile(file);
    }
}

void printError(int origin, const char *format, ...) {
    va_list args;
    if(Log_Error < LogCurrentLevel) {
        return;
    } else {
        stdPrint(Log_Error, 0, (FILE *)stderr, "[%.4d] | ", origin);
        stdPrint(Log_Error, 1, (FILE *)stderr, "ERROR - ", origin);
        flockfile(stderr);
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        fflush(stderr);
        funlockfile(stderr);
    }
}

void printFine(int origin, const char *format, ...) {
    va_list args;
    if(Log_Fine < LogCurrentLevel) {
        return;
    } else {
        stdPrint(Log_Fine, 0, (FILE *)stdout, "[%.4d] | ", origin);
        stdPrint(Log_Fine, 1, (FILE *)stdout, "FINE  - ", origin);
        flockfile(stdout);
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
        fflush(stdout);
        funlockfile(stdout);
    }
}

void printInfo(int origin, const char *format, ...) {
    va_list args;
    if(Log_Info < LogCurrentLevel) {
        return;
    } else {
        stdPrint(Log_Info, 0, (FILE *)stdout, "[%.4d] | ", origin);
        stdPrint(Log_Info, 1, (FILE *)stdout, "INFO  - ", origin);
        flockfile(stdout);
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
        fflush(stdout);
        funlockfile(stdout);
    }
}

void printDebug(int origin, const char *format, ...) {
    va_list args;
    if(Log_Debug < LogCurrentLevel) {
        return;
    } else {
        stdPrint(Log_Debug, 0, (FILE *)stdout, "[%.4d] | ", origin);
        stdPrint(Log_Debug, 1, (FILE *)stdout, "DEBUG - ", origin);
        flockfile(stdout);
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
        fflush(stdout);
        funlockfile(stdout);
    }
}

void printPlain(const char *format, ...) {
    va_list args;
    flockfile(stdout);
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    fflush(stdout);
    funlockfile(stdout);
}

/*
 *  User Input Related Functions
 */

void getIntFunction(char* message, int* value) {
    stdPrint(Log_Plain, 0, (FILE *)stdout, "%s", message);

    flockfile(stdin);
    __fpurge(stdin);
    fscanf(stdin,"%d", value);
    __fpurge(stdin);
    funlockfile(stdin);
}

void getCharFunction(char* message, char* value) {
    stdPrint(Log_Plain, 0, (FILE *)stdout, "%s", message);

    flockfile(stdin);
    __fpurge(stdin);
    fscanf(stdin,"%c", value);
    __fpurge(stdin);
    funlockfile(stdin);
}

void getStringFunction(char* message, char** value, int maxStrLen) {
    stdPrint(Log_Plain, 0, (FILE *)stdout, "%s", message);

    flockfile(stdin);
    __fpurge(stdin);
    fgets(*value, maxStrLen, stdin);
    __fpurge(stdin);
    funlockfile(stdin);
}

void getGeneric(char* message, const char *format, ...) {
    stdPrint(Log_Plain, 0, (FILE *)stdout, "%s", message);

    flockfile(stdin);
    va_list args;
    va_start (args, format);
    __fpurge(stdin);
    vfscanf (stdin, format, args);
    __fpurge(stdin);
    va_end (args);
    funlockfile(stdin);
}


/*
 *  Operations Definitions
 */

struct LogOps Log = {
    .setLevel=   setLogLevel,
    .error   =   printError,
    .fine    =   printFine,
    .info    =   printInfo,
    .debug   =   printDebug,
    .plain   =   printPlain
};

struct UserInput Input = {
    .getChar =  getCharFunction,
    .getInt  =  getIntFunction,
    .getStr  =  getStringFunction,
    .get     =  getGeneric
};

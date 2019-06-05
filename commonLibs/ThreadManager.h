
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca de Recursos Referentes as Threads
 */

#ifndef Thread_Def
#define Thread_Def

#include <pthread.h>

void* waitResponse(void* function, void* args);

int noResponse(void* function, void* args);

void threadExit(void* rtnValue);

pthread_mutex_t* mutexInit();

int mutexDestroy(pthread_mutex_t* mutex);

int mutexLock(pthread_mutex_t *mutex);

int mutexTryLock(pthread_mutex_t *mutex);

void mutexUnlock(pthread_mutex_t *mutex);

#endif

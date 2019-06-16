
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Desenvolvimento de Recursos Referentes as Threads
 */

#include "./ThreadManager.h"
#include "./CustomStreams.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/*
 *  Função que Lança uma Thread e espera por sua Resposta
 *  Argumentos:
 *      @function   ==  Ponteiro da Função de Comportamento
 *      @args       ==  Argumentos da Função
 *  Retornos:
 *      NULL        ==  Erro
 *   != NULL        ==  Sucesso
 */
void* waitResponse(void* function, void* args) {
    pthread_t thread;
    void* retValue = malloc(sizeof(void*));
    if (pthread_create(&thread, NULL, function, args)) {
        Log.error(getpid(), "Error! Thread couldn't be Created.\n");
        perror("pthread_create");
        return NULL;
    }
    if ((pthread_join(thread, &retValue)) != 0) {
        Log.error(getpid(), "Error! Thread couldn't be Joined.\n");
        perror("pthread_join");
        return NULL;
    }
    return retValue;
}

/*
 *  Função que Lança uma Thread
 *  Argumentos:
 *      @function   ==  Ponteiro da Função de Comportamento
 *      @args       ==  Argumentos da Função
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int noResponse(void* function, void* args) {
    pthread_t thread;
    if (pthread_create(&thread, NULL, function, args)) {
        Log.error(getpid(), "Error! Thread couldn't be Created.\n");
        perror("pthread_create");
        return -1;
    }
    if ((pthread_detach(thread)) < 0) {
        Log.error(getpid(), "Error! Thread couldn't be Detached.\n");
        perror("pthread_detach");
        return -2;
    }
    return 0;
}

/*
 *  Função que Finaliza uma Thread
 *  Argumentos:
 *      @rtnValue   ==  Retorno da Thread
 */
void threadExit(void* rtnValue) {
    pthread_exit(rtnValue);
}

/*
 *  Função que Inicia um Mutex
 *  Retornos:
 *      NULL        ==  Erro
 *   != NULL        ==  Sucesso
 */
pthread_mutex_t* mutexInit() {
    pthread_mutex_t* newMutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    if ((pthread_mutex_init(newMutex, NULL)) < 0) {
        Log.error(getpid(), "Error! Mutex couldn't be Created.\n");
        perror("pthread_mutex_init");
        return NULL;
    }
    return newMutex;
}

/*
 *  Função que Finaliza um Mutex
 *  Argumentos:
 *      @mutex      ==  Ponteiro do Mutex
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int mutexDestroy(pthread_mutex_t* mutex) {
    int rtnValue;
    if ((rtnValue = pthread_mutex_destroy(mutex)) < 0) {
        Log.error(getpid(), "Error! Mutex couldn't be Destroyed.\n");
        perror("pthread_mutex_destroy");
    }
    return rtnValue;
}

/*
 *  Função que Trava um Mutex
 *  Argumentos:
 *      @mutex      ==  Ponteiro do Mutex
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int mutexLock(pthread_mutex_t *mutex) {
    int rtnValue;
    if ((rtnValue = pthread_mutex_lock(mutex)) < 0) {
        Log.error(getpid(), "Error! Mutex couldn't be Locked.\n");
        perror("pthread_mutex_lock");
    }
    return rtnValue;
}

/*
 *  Função que Avalia se é possivel Travar um Mutex;
 *  Se sim trava, caso contário retorna imediatamente;
 *  Argumentos:
 *      @mutex      ==  Ponteiro do Mutex
 *  Retornos:
 *      > 0         ==  Erro
 *      EBUSY       ==  Mutex Preso em Outra Thread
 *      = 0         ==  Mutex Travado
 */
int mutexTryLock(pthread_mutex_t *mutex) {
    int rtnValue;
    if ((rtnValue = pthread_mutex_trylock(mutex)) < 0) {
        if(rtnValue != EBUSY) {
            Log.error(getpid(), "Error! Mutex couldn't perform a TryLock.\n");
            perror("pthread_mutex_trylock");
        }
    }
    return rtnValue;
}

/*
 *  Função que Destrava um Mutex
 *  Argumentos:
 *      @mutex      ==  Ponteiro do Mutex
 *  Retornos:
 *      > 0         ==  Erro
 *      = 0         ==  Sucesso
 */
int mutexUnlock(pthread_mutex_t *mutex) {
    int rtnValue;
    if ((rtnValue = pthread_mutex_unlock(mutex)) < 0) {
        if(rtnValue != EBUSY) {
            Log.error(getpid(), "Error! Mutex couldn't be Unlocked.\n");
            perror("pthread_mutex_unlock");
        }
    }
    return rtnValue;
}

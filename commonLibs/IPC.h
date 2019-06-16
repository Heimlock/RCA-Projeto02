
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *		Rodrigo da Silva Cardoso      RA: 16430126
 *
 *	 Biblioteca de Recursos Referentes a Comunicacao Inter Processos
 */

#ifndef IPC_Def
#define IPC_Def

#include    <stdio.h>
#include    <unistd.h>
#include 	<sys/ipc.h>
#include 	<sys/shm.h>
#include 	<sys/sem.h>
#include    <string.h>

typedef struct semControl_t {
    int     key;
    int     id;

    struct  sembuf	sem_lock[1];
    struct  sembuf	sem_unlock[1];
} semControl_t;

typedef struct shMemControl_t {
    int     key;
    int     id;

    void*   data;
    size_t  size;
} shMemControl_t;

typedef struct IPCOps_t {
    int     (*semOpen)      (struct semControl_t* sem_ops, int semKey);
    int     (*semClose)     (struct semControl_t* sem_ops);
    int     (*semLock)      (struct semControl_t* sem_ops);
    int     (*semUnlock)    (struct semControl_t* sem_ops);

    int     (*memOpen)      (struct  shMemControl_t* mem_ops, int memKey, size_t size);
    int     (*memClose)     (struct  shMemControl_t* mem_ops);
} IPCOps_t;

extern struct IPCOps_t IPC;

#endif

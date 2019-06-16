
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
 *	 Desenvolvimento dos Recursos Referentes a Comunicacao Inter Processos
 */


#include    <stdio.h>
#include    <unistd.h>
#include 	<sys/ipc.h>
#include 	<sys/shm.h>
#include 	<sys/sem.h>
#include    <string.h>

#include "./IPC.h"
#include "CustomStreams.h"

#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

/*
 *	Functions Prototypes 
 */
int     semOpen(semControl_t* sem_ops, int semKey);
int     semClose(semControl_t* sem_ops);
int     semLock(semControl_t* sem_ops);
int     semUnlock(semControl_t* sem_ops);

int	    memOpen( shMemControl_t* mem_ops, int memKey, size_t size);
int     memClose( shMemControl_t* mem_ops);

struct IPCOps_t IPC = {
    .semOpen	= semOpen,
    .semClose	= semClose,
    .semLock	= semLock,
    .semUnlock	= semUnlock,
    .memOpen	= memOpen,
    .memClose	= memClose
};

/*
 *	Semaphore Related
 */

int     semOpen(semControl_t *sem_ops, int semKey) {
    //  Populate Semaphore Key
    sem_ops->key  =   semKey;

	//	Initialize	Semaphores Signals
    sem_ops->sem_lock[0].sem_num =  0;
	sem_ops->sem_lock[0].sem_op  = -1;
	sem_ops->sem_lock[0].sem_flg =  0;

	sem_ops->sem_unlock[0].sem_num =  0;
	sem_ops->sem_unlock[0].sem_op  =  1;
	sem_ops->sem_unlock[0].sem_flg =  0;

	//	Semaphore
    if( ( sem_ops->id = semget( sem_ops->key, 1, IPC_CREAT | 0666 ) ) == -1 ) {
		Log.error(getpid(), "Error! 'semget()' failed, Semaphore wasn't created! ID = %d.\n", sem_ops->id);
		perror("semget");
		return -1;
	}
	if( semop( sem_ops->id, sem_ops->sem_unlock, 1 ) == -1 ) {
		Log.error(getpid(), "Error! 'semop()' failed, Semaphore wasn't Initialized. ID = %d.\n", sem_ops->id);
		perror("semop");
		return -1;
	}
    return 0;
}

int     semClose(semControl_t *sem_ops) {
    if(semctl( sem_ops->id, 0, IPC_RMID, 0) != 0) {
        Log.error(getpid(), "Error! Not able to remove Semaphores with ID = %d.\n", sem_ops->id);
		perror("semctl");
        return -1;
    }
    return 0;
}

int     semLock(semControl_t *sem_ops) {
	if( semop(sem_ops->id, sem_ops->sem_lock, 1) == -1 ) {
		Log.error(getpid(), "Error! SemLock has failed! ID = %d\n", sem_ops->id);
		perror("semLock");
		fflush(stderr);
		return -1;
	}
    return 0;
}

int     semUnlock(semControl_t *sem_ops) {
	if( semop(sem_ops->id, sem_ops->sem_unlock, 1) == -1 ) {
		Log.error(getpid(), "Error! SemUnlock has failed! ID = %d\n", sem_ops->id);
		perror("semUnlock");
		fflush(stderr);
        return -1;
	}
    return 0;
}

/*
 *	Shared Memory Related
 */

int	    memOpen(shMemControl_t *mem_ops, int memKey, size_t size) {
    //  Populate Shared Memory data
    mem_ops->key     = memKey;
    mem_ops->size    = size;

	//	Shared Memory
    if((mem_ops->id = shmget( mem_ops->key, mem_ops->size, IPC_CREAT | 0666)) == -1 ) {
		Log.error(getpid(), "Error! Shared Memory wasn't created. ID = %d\n", mem_ops->id);
		perror("shmget");
		return -1;
	}
	if((mem_ops->data = shmat( mem_ops->id, NULL, 0)) < 0 ) {
		Log.error(getpid(), "Error! 'sgmat()' has failed. ID = %d\n", mem_ops->id);
		perror("shmat");
		return -1;
	}
    memset(mem_ops->data, 0, mem_ops->size);
	return 0;
}

int     memClose(shMemControl_t *mem_ops) {
    if( shmctl( mem_ops->id,IPC_RMID,NULL) != 0 ) {
        Log.error(getpid(), "Error! Shared Memory wasn't able to be removed. ID = %d\n", mem_ops->id);
		perror("shmctl");
        return -1;
    }
    return 0;
}


/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Desenvolvimento Lógico Referente ao Cliente
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./client.h"
#include "../commonLibs/UserData.h"

/*
int canContinueClient() {
    int rtnValue;
    if(allowNewConnections) {
        rtnValue = 1;
    } else {
        rtnValue = 0;
    }
    return rtnValue;
}

 void *mandarMensagem(void *arg) {
	char ip[9] = "localhost"; //por enquanto é sempre localhost
    int PortaDestino;
    int saida = 0;

    while(saida != 1){
        fprintf(stdout, "[%.4d] | Mandar mensagem Iniciado\n", getpid());
        fprintf(stdout, "[%.4d] | Porta destino da conexao:\n", getpid());
        fflush(stdout);
        scanf("%d", &PortaDestino);
        
        //Se digitar 0 na porta Destino, sair do while, fechar thread.
        if(PortaDestino == 0){
            saida = 1;
            break;
        }

    
        //Criação do socket que envia mensagens, tipo cliente
        fprintf(stdout, "\n");
        fprintf(stdout, "[%d] | Socket de Enviamento\n", getpid());
        fflush(stdout);
        if(commOps.initClient(&localSend, 0) < 0) {
            fprintf(stderr, "[%d] | Error! Init Socket de Enviamento!\n", getpid());
            fflush(stderr);
            exit(-2);
        }

        //Conect com a PortaDestino
        if((commOps.connect(&localSend, &remoteSend, ip, PortaDestino)) < 0){
            fprintf(stderr, "[%.4d] | Error! Client couldn't connect to peer.\n", getpid());
            fflush(stderr);
            perror("connectToPeer");
            exit(-3);
        }

        fprintf(stdout, "[%.4d] | Mandando mensagem\n", getpid());
        fflush(stdout);
    
        //Mandar dados
        //int send(int sockfd, const void *msg, int len, int flags); 

        //Mandar string
        char *msg = "Beej was here!";
        int len, bytes_sent;          
        len = strlen(msg);
        bytes_sent = send(localSend.socketDesc, msg, len, 0);

        //Mandar imagem
        struct File_t* file;
        disk2Memory(&file, "go.png", "998673533");
        sendFile(&localSend, *file);
        //Fim de Mandar dados

        //Fechar Socket
        fprintf(stdout, "[%.4d] | Mensagem enviada, fechando socket\n", getpid());
        fflush(stdout);
        close_Socket(&localSend);
    }

	threadExit(NULL);
}


void *attendClientPeer(void *arg) {
	int threadId =  (int)arg;
    struct SPDT_Command  *command;
	struct commFacade_t  innerRemote;

    fprintf(stdout, "[%.4d] | Attend Peer Init\n", threadId);
    fflush(stdout);

	memcpy(&innerRemote, &remoteRec, sizeof(commFacade_t));
    mutexUnlock(mutex_remote_Rec);

    //Receber dados
    //int bytesRecebidos = recv(socketDescConexao, void *buf, int len, 0);
    
    //Receber String
    #define MAXDATASIZE 100 // max number of bytes we can get at once
    char buf[MAXDATASIZE];
    int numbytes;
    if ((numbytes = recv(innerRemote.socketDesc, buf, MAXDATASIZE-1, 0)) < 0) {
        perror("recv");
        exit(1);    
    }
    buf[numbytes] = '\0';    
    printf("client: received '%s'\n",buf);

    //Receber imagem
    struct File_t* file;
    receiveStruct(&innerRemote, SendFile, &file);
    file->name[0] = 'R';
    memory2Disk(*file);
    //Fim de Receber dados

    //Fechar socket
    close_Socket(&innerRemote);
	threadExit(NULL);
}

void newConnectionClient() {

    if((commOps.accept(&localRec, &remoteRec)) < 0){
        fprintf(stderr, "[%.4d] | Error! Client couldn't accept connection.\n", getpid());
        fflush(stderr);
        perror("newConnection");
        exit(-4);
    }

    if(allowNewConnections) {
        fprintf(stdout, "[%.4d] | New Connection!\n", getpid());
        fflush(stdout);
                    
        childCount++;
        mutexLock(mutex_remote_Rec);

        if(noResponse(attendClientPeer, childCount) < 0) {
            fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
            fflush(stderr);
            perror("newConnection");
        }
    }else{
        commOps.close(&remoteRec);
    }
}
*/
int main(int argc, char const *argv[]) {
    //Inicia o cliente
    fprintf(stdout, "[%d] | Client Module Initialized!\n", getpid());
    fflush(stdout);


    initSharedData(); //Inicia variaveis globais
    mutex_remote_Rec = mutexInit(); //Precisa disso para mutex nao dar segmetation fault

    //Precisa de duas variaveis (./client IpDoServidor PortaDoServidor)
    if(argc != 3) {
        fprintf(stderr, "[%d] | Error! Not a Valid Input!\n", getpid());
        fprintf(stderr, "[%d] | Usage: ./client <IpDoServidor> <PortaDoServidor>\n", getpid());
        fflush(stderr);
        exit(-1);
    }

    fprintf(stdout, "[%d] | Digite seu número de telefone:\n", getpid());
    
    //FIXME Código errado para testes
    fprintf(stdout, "[%d] | Digite 1 para cliente 1, 2 para cliente 2, 3 para cliente 3\n", getpid());
    fflush(stdout);
    char IdTeclado;
    char* UserId;
    int porta;
    scanf(" %c", &IdTeclado);
    
    if(IdTeclado == '1'){
        UserId = "998673533";
        porta = 5000;
    }else if(IdTeclado == '2'){
        UserId = "999999999";
        porta = 6000;
    }else if(IdTeclado == '3'){
        UserId = "222222222";
        porta = 7000;
    }

    fprintf(stdout, "[%d] | UserId = %s, Porta = %d\n", getpid(), UserId, porta);
    fflush(stdout);
    //Fim do código teste

    //Criação do socket que recebe mensagens, tipo servidor
    fprintf(stdout, "[%d] | Socket de Recebimento\n", getpid());
    fflush(stdout);
    if((commOps.initServer(&localRec, porta)) < 0){ //FIXME depois que Servidor e LogIn estiverem funcionando, mudar "porta" para "0"
        fprintf(stderr, "[%d] | Error! Init Socket de Recebimento!\n", getpid());
        fflush(stderr);
        exit(-2);    
    }

    /*
    TODO
    Mandar a porta criada no .initServer e o IP para o servidor, 
        junto com o ID, que é o telefone
    SendToServer(UserId, IPdoCliente, PortaDoCliente)
    Exemplo de IpDoCliente = "localhost" (funciona para outros valores?)
    Exemplo PortaDoCliente = localRec.socketAddr.sin_port???
     */
    
    //Criar uma thread que manda e le mensagens
    if(noResponse(mandarMensagem, NULL) < 0) {
        fprintf(stderr, "[%.4d] | Error! Thread couldn't attend.\n", getpid());
        fflush(stderr);
        perror("newConnection");
    }

    //Criação de Threads que recebem mensagens usando o socket de recebimento
    do{
        newConnectionClient();        
    }while(canContinueClient());

    /*
    Fechar sockets
    Deveria fechar mais coisas antes de fechar a main?
     */
    commOps.close(&localSend);
    commOps.close(&localRec);
    return 0; //Nao vai passar daqui se nao comentar esse return

    //  =============================STUB DO FELIPE=====================================
    //  ================================================================================
    char ip[9] = "localhost";
    int port = 5000; //porta do servidor hardcoded

    struct SPDT_Command *command;


    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 00 -- Init\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

   if(argc != 3) {
        fprintf(stderr, "[%d] | Error! Not a Valid Input!\n", getpid());
        fprintf(stderr, "[%d] | Usage: ./client <ServerIp> <Port>\n", getpid());
        fflush(stderr);
        exit(-1);
    }
   

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 01 -- LogIn\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    if(commOps.connect(&localSend, &remoteSend, 0, ip, port) < 0) { //ServerPort, atoi(argv[2]), definido como 5000
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    char *value = "982660910";

    newCommand(&command, LogIn, UserId_Len * sizeof(char), value);
    printCommand(getpid(), *command);

    if(sendCommand(&localSend, (*command)) < 0) {
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-4);
    }
    // destroyCommand(command);
    commOps.close(&localSend);
    commOps.close(&remoteSend);
    sleep(5);

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 02 -- RequestClient\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    if(commOps.connect(&localSend, &remoteSend, 0, ip, port) < 0) { //ServerPort, atoi(argv[2]), definido como 5000
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    newCommand(&command, RequestClient, UserId_Len * sizeof(char), value);
    printCommand(getpid(), *command);

    if(sendCommand(&localSend, (*command)) < 0) {
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-5);
    }
    // destroyCommand(command);
    

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 03 -- ReceiveStruct\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    struct User_t *user = NULL;
    receiveStruct(&localSend, RequestClient, &user);
    fprintf("Telefone: %s\n", user->id);

    commOps.close(&localSend);
    commOps.close(&remoteSend);
    sleep(10);

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 04 -- LogOut\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);

    if(commOps.connect(&localSend, &remoteSend, 0, ip, port) < 0) {
        fprintf(stderr, "[%d] | Error! Connect to Server Socket!\n", getpid());
        fflush(stderr);
        exit(-2);
    }

    newCommand(&command, LogOut, UserId_Len * sizeof(char), value);
    printCommand(getpid(), *command);

    if(sendCommand(&localSend, (*command)) < 0){
        fprintf(stderr, "[%d] | Error! Failed to send.\n", getpid());
        fflush(stderr);
        exit(-6);
    }
    // destroyCommand(command);

    //  ================================================================================

    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fprintf(stdout, "[%d] | Stage 05 -- Terminated\n", getpid());
    fprintf(stdout, "[%d] | ========================================\n", getpid());
    fflush(stdout);
    commOps.close(&localSend);
    commOps.close(&remoteSend);
}
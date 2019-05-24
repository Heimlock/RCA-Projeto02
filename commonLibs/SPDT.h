
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca de Recursos Referentes ao Protocolo de comunicacao
 *  (Simple Protocol for Data Transfer)
 */

typedef enum ActionType {
    //  0x00 ate 0x7F -- Sem Payload
    NewClient = 0x00,
    //  0x7F ate 0xFF -- Com Payload
    newMessage = 0x7F,
} ActionType;

typedef struct SPDT_Command {
    ActionType type;
    int length;
    void* value;
} SPDT;

SPDT_Command* newCommand(ActionType type, int length, void* data);
void sendCommand(SPDT_Command* command);
SPDT_Command* receiveCommand();
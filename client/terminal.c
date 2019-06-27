
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsP2P,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *      Bruno Pereira Bannwart        RA: 15171572
 *		Felipe Moreira Ferreira       RA: 16116469
 *      Gabriela Ferreira Jorge       RA: 12228441
 *		Rodrigo da Silva Cardoso      RA: 16430126
 *
 *	 Desenvolvimento dos Recursos Referentes ao Terminal
 */

#include "./terminal.h"
#include "./client.h"
#include "../commonLibs/ThreadManager.h"
#include "../commonLibs/MessageData.h"
#include "../commonLibs/FileData.h"
#include "../commonLibs/CustomStreams.h"

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>

/*
 *  Logic
 */
void    initTerminal() {
    MenuItem option = -1;

    Log.plain("Enter with your 9 digit ID\n");
    Input.getStr("UserId: ", &userId, (UserId_Len + 1) * sizeof(char));
    Log.debug(getpid(), "Launch logIn\n");

    // noResponse(logIn, NULL);
    logIn();
    do {
        option = Error;
        option = mainMenu();
        switch (option) {
            case DirectMessage: { 
                char* peerId = (char*) malloc((1 + UserId_Len) * sizeof(char));
                Message_t *msg;

                directMessage(userId, &peerId, &msg);
                // /*
                // int offset;
                // int size = (UserId_Len + 1) * sizeof(char) + sizeof(Message_t);
                // void* vars = malloc(size);
                // offset = 0;
                // memcpy(vars + offset, peerId, (UserId_Len + 1) * sizeof(char));
                // offset = (UserId_Len + 1) * sizeof(char);
                // memcpy(vars + offset, msg, sizeof(Message_t));

                // noResponse(sendMessagePeer, vars);
                sendMessagePeer(peerId, *msg);
                // */
                enter2Continue();
                free(peerId);
                break;
            }
            case GroupMessage: {
                char* groupId = (char*) malloc((1 + UserId_Len) * sizeof(char));
                Message_t *msg;
                LinkedListNode* groupNode;

                groupMessage(userId, &groupId, &msg);
                /*
                groupNode = getNode(*groups, groupId);

                if(groupNode != NULL) {
                    //  Send Message For Each Node
                    Log.info(getpid(), "Send Message For Each Node\n");
                    enter2Continue();
                } else {
                    Log.error(getpid(), "Group not Found.\n");
                }
                //*/
                enter2Continue();
                free(groupId);
                break;
            }
            case DirectFile: {
                char* peerId = (char*) malloc((1 + UserId_Len) * sizeof(char));
                File_t *file;

                directFile(userId, &peerId, &file);
                /*
                int offset;
                int size = (UserId_Len + 1) * sizeof(char) + sizeof(File_t);
                void* vars = malloc(size);
                offset = 0;
                memcpy(vars + offset, peerId, (UserId_Len + 1) * sizeof(char));
                offset = (UserId_Len + 1) * sizeof(char);
                memcpy(vars + offset, file, sizeof(File_t));

                //  Send File Message
                // noResponse(sendFilePeer, vars);*/
                if(file != NULL) {
                    sendFilePeer(peerId, *file);
                }
                //*/
                enter2Continue();
                free(peerId);
                break;
            }
            case GroupFile: {
                char* groupId = (char*) malloc((1 + UserId_Len) * sizeof(char));
                File_t *file;
                LinkedListNode* groupNode;

                groupFile(userId, &groupId, &file);
                /*
                //  Send File Message
                groupNode = getNode(*groups, groupId);
                if(groupNode != NULL) {
                    //  Send File For Each Node
                } else {
                    Log.error(getpid(), "Group not Found.\n");
                }
                //*/
                enter2Continue();
                free(groupId);
                break;
            }
            case Inbox: {
                LinkedListNode* msgNode;
                Message_t* auxMsg;
                int msgCount;
                mutexLock(mutex_list_messages);
                msgCount = messages->size;
                if(messages->size > 0) {
                    do {
                        msgNode = NULL;
                        msgNode = llOps.getFirst(messages);
                        if(msgNode) {
                            auxMsg = msgNode->data;
                            if(auxMsg->length != 0) {
                                Log.info(getpid(), "Message #%d of %d\n", (msgCount - messages->size) + 1, msgCount);
                                printMsg(*auxMsg);
                                enter2Continue();
                                llOps.remove(messages, msgNode->key);
                            } else {
                                Log.error(getpid(), "Node With Length == 0\n");
                                msgNode = NULL;
                                enter2Continue();
                            }
                        }
                    } while(msgNode != NULL && messages->size != 0);
                } else {
                    Log.plain("No Messages to Read.\n");
                    enter2Continue();
                }
                mutexUnlock(mutex_list_messages);
                break;
            }
            case Contacts: { /*
                contactsSubMenu();
                 */
                break;
            }
            case Exit: {
                break;
            }
            default: {
                Log.plain("Not a Valid Option! Option: %s, Code: %d\n", getOption(option), option);
                enter2Continue();
                break;
            }
        }
        #ifdef  DEBUG
            Log.debug(getpid(), "Sleep for 2s\n");
            sleep(2);
        #endif
    }while(option != Exit);
    waitResponse(logOut, NULL);
    state = Offline;
}

/*
void    contactsSubMenu () {
    MenuItem option;
    do {
        option = -1;
        option = displayContactsMenu();

        switch (option) {
            case ListUsers: {
                printAllKeys(contacts);
                enter2Continue();
                break;
            }
            case ListGroups: {
                //  For Each Node, Print its Key
                //  And Iterate over its Data (Inner Linked List)
                forEach(groups, printGroup);
                enter2Continue();
                break;
            }
            case NewContact: {
                char* userId = (char*) malloc((1 + UserId_Len) * sizeof(char));
                Log.plain("New Contact\n");
                Log.plain("Enter the new userId.\n");
                Input.getStr("UserId: \n", &userId, (UserId_Len + 1) * sizeof(char));
                llOps.add(contacts, userId, userId);
                enter2Continue();
                free(userId);
                break;
            }
            case DeleteContact: {
                char* userId = (char*) malloc((1 + UserId_Len) * sizeof(char));
                Log.plain("Delete Contact\n");
                Log.plain("Enter the userId to Delete.\n");

                Input.getStr("UserId: \n", &userId, (UserId_Len + 1) * sizeof(char));
                llOps.remove(contacts, userId);
                enter2Continue();
                free(userId);
                break;
            }
            case NewGroup: {
                //  Receive GroupId
                //  Do
                //  Receive UserId
                //  While True
                Log.plain("Not yet Implemented!\n");
                break;
            }
            case DeleteGroup: {
                char* groupId = (char*) malloc((1 + UserId_Len) * sizeof(char));
                Log.plain("Delete Contact\n");
                Log.plain("Enter the GroupId to Delete.\n");
                Input.getStr("GroupId: \n", &groupId, (UserId_Len + 1) * sizeof(char));
                llOps.remove(groups, groupId);
                enter2Continue();
                free(groupId);
                break;
            }
            default: {
                Log.plain("Not a Valid Option!\n");
                break;
            }
        }
    } while(option != Exit);
}
 */
/*
 *  Text
 */

void    printHeader() {
    // system("clear");
    Log.plain("========================================\n");
    Log.plain("----------------WhatsP2P----------------\n");
    Log.plain("========================================\n");
    Log.plain("UserId: %s\n", userId);

    mutexLock(mutex_list_messages);
    Log.plain("Inbox.: %d\n", messages->size);
    mutexUnlock(mutex_list_messages);
}

int     mainMenu() {
    int option;
    printHeader();

    Log.plain("\nMenu\n");
    Log.plain("[%.2d] - Direct Message\n", DirectMessage);
    Log.plain("[%.2d] - Direct File\n", DirectFile);
    Log.plain("[%.2d] - Group Message\n", GroupMessage);
    Log.plain("[%.2d] - Group File\n", GroupFile);
    Log.plain("[%.2d] - Inbox\n", Inbox);
    Log.plain("[%.2d] - Contacts\n", Contacts);
    Log.plain("[%.2d] - Exit\n", Exit);
    Input.getInt("Option: ", &option);
    return option;
}

int     displayContactsMenu() {
    printHeader();
    int option;

    Log.plain("\nMenu de Contatos\n");
    Log.plain("[%.2d] - List Users\n", ListUsers);
    Log.plain("[%.2d] - List Groups\n", ListGroups);
    Log.plain("[%.2d] - New Contact\n", NewContact);
    Log.plain("[%.2d] - Delete Contact\n", NewGroup);
    Log.plain("[%.2d] - New Group\n", DeleteGroup);
    Log.plain("[%.2d] - Delete Group\n", DeleteGroup);
    Log.plain("[%.2d] - Exit\n", Exit);
    Input.getInt("Option: ", &option);
    return option;
}

/*
 *  User Input
 */
void    directMessage(char* userId, char** peerId, Message_t** msg) {
    int msgSize = MessageMaxSize * sizeof(char);
    char* messageText = (char*) malloc(msgSize);

    Input.getStr("PeerId: ", peerId, (UserId_Len + 1) * sizeof(char));
    Input.getStr("Message: ", &messageText, msgSize);

    newMessage(msg, userId, msgSize, messageText);
    free(messageText);
}

void    directFile(char* userId, char** peerId, File_t** file) {
    char* filePath = (char*) malloc(FileName_Len * sizeof(char));

    Input.getStr("PeerId...: ", peerId, (UserId_Len + 1) * sizeof(char));
    Input.getStr("File Path: ", &filePath, FileName_Len * sizeof(char));

    disk2Memory(file, filePath, userId);
    free(filePath);
}

void    groupMessage(char* userId, char** groupId, Message_t** msg) {
    char* messageText = (char*) malloc(80 * sizeof(char));

    Input.getStr("GroupId: ", groupId, (UserId_Len + 1) * sizeof(char));
    Input.getStr("Message: ", &messageText, MessageMaxSize * sizeof(char));

    newMessage(msg, userId, sizeof(messageText), messageText);
    free(messageText);
}

void    groupFile(char* userId, char** groupId, File_t** file) {
    char* filePath = (char*) malloc(FileName_Len * sizeof(char));

    Input.getStr("GroupId..: ", groupId, (UserId_Len + 1) * sizeof(char));
    Input.getStr("File Path: ", &filePath, FileName_Len * sizeof(char));

    disk2Memory(file, filePath, userId);
    free(filePath);
}

void enter2Continue() {
    char c;
    Log.plain("Press Enter to continue.\n");
    Input.get("", "%c", &c);
}

char* getOption(enum MenuItem option) {
    switch (option) {
    case DirectMessage: 
        return "DirectMessage";
    case DirectFile: 
        return "DirectFile";
    case GroupMessage: 
        return "GroupMessage";
    case GroupFile: 
        return "GroupFile";
    case Inbox: 
        return "Inbox";
    case Contacts: 
        return "Contacts";
    case ListUsers: 
        return "ListUsers";
    case ListGroups: 
        return "ListGroups";
    case NewContact: 
        return "NewContact";
    case DeleteContact: 
        return "DeleteContact";
    case NewGroup: 
        return "NewGroup";
    case DeleteGroup: 
        return "DeleteGroup";
    case Exit: 
        return "Exit";
    case Error: 
        return "Error";
    default:
        return "Not Defined";
    }
}

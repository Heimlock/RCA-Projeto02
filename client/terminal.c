
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsP2P,
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *	 Desenvolvimento dos Recursos Referentes ao Terminal
 */

#include "./terminal.h"
#include "./client.h"
#include "../commonLibs/ThreadManager.h"
#include "../commonLibs/MessageData.h"
#include "../commonLibs/FileData.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>

/*
 *  Logic
 */

/*
 * noResponse Thread
 */
void    *initTerminal() {
    MenuItem option = -1;

    do {
        option = Error;
        option = mainMenu();
        switch (option) {
            case DirectMessage: {
                char peerId[10];;
                Message_t *msg;
                User_t  *user;

                directMessage(userId, &peerId, &msg);
                user = requestClient(peerId);
                sendMessagePeer(user->addr, (*msg));
                
                enter2Continue();
                break;
            }
            case GroupMessage: {
                char groupId[10];
                Message_t *msg;
                LinkedListNode* groupNode;
                
                groupMessage(userId, &groupId, &msg);
                groupNode = getNode(*groups, groupId);
                
                if(groupNode != NULL) {
                    //  Send Message For Each Node
                    fprintf(stdout, "Send Message For Each Node\n");
                    fflush(stdout);
                    enter2Continue();
                } else {
                    fprintf(stderr, "Group not Found.\n");
                    fflush(stderr);
                }
                
                enter2Continue();
                break;
            }
            case DirectFile: {
                char peerId[10];
                File_t *file;

                directFile(userId, &peerId, &file);
                //  Send File Message
                fprintf(stdout, "Send File Message\n");
                fflush(stdout);
                
                enter2Continue();
                break;
            }
            case GroupFile: {
                char groupId[10];
                File_t *file;
                LinkedListNode* groupNode;
                
                groupFile(userId, &groupId, &file);
                //  Send File Message
                groupNode = getNode(*groups, groupId);
                if(groupNode != NULL) {
                    //  Send File For Each Node
                } else {
                    fprintf(stderr, "Group not Found.\n");
                    fflush(stderr);
}
                enter2Continue();
                break;
            }
            case Inbox: {
                LinkedListNode* msgNode;
                Message_t* auxMsg;
                mutexLock(mutex_list_messages);
                if(messages->size > 0) {
                    do {
                        msgNode = llOps.getFirst(messages);
                        if(msgNode) {
                            llOps.remove(messages, msgNode->key);
                            auxMsg = msgNode->data;
                            printMsg(*auxMsg);
                            enter2Continue();
                        }
                    } while(msgNode != NULL);
                } else {
                    fprintf(stderr, "No Messages to Read.\n");
                    fflush(stderr);
                    enter2Continue();
                }
                mutexUnlock(mutex_list_messages);
                break;
            }
            case Contacts: {
                contactsSubMenu();
                break;
            }
            case Exit: {
                break;
            }
            default: {
                fprintf(stderr, "Not a Valid Option!\n");
                fflush(stderr);
                __fpurge(stdin);
                enter2Continue();
                break;
            }
        }
    }while(option != Exit);
    waitResponse(logOut, NULL);
    state = Offline;
}

void    contactsSubMenu () {
    MenuItem option;
    do {
        option = -1;
        option = displayContactsMenu();

        switch (option) {
            case ListUsers: {
                //  For Each Node, Print its Key
                break;
            }
            case ListGroups: {
                //  For Each Node, Print its Key
                //  And Iterate over its Data (Inner Linked List)
                break;
            }
            case NewContact: {
                char userId[10];
                fprintf(stdout, "New Contact\n");
                fprintf(stdout, "Enter the new userId.\n");
                fflush(stdout);
                getString("UserId: \n", &userId, (UserId_Len + 1) * sizeof(char));
                llOps.add(contacts, userId, userId);
                break;
            }
            case DeleteContact: {
                char userId[10];
                fprintf(stdout, "Delete Contact\n");
                fprintf(stdout, "Enter the userId to Delete.\n");
                fflush(stdout);
                getString("UserId: \n", &userId, (UserId_Len + 1) * sizeof(char));
                llOps.remove(contacts, userId);
                break;
            }
            case NewGroup: {
                //  Receive GroupId
                //  Do
                //  Receive UserId
                //  While True
                fprintf(stdout, "Not yet Implemented!\n");
                fflush(stdout);
                break;
            }
            case DeleteGroup: {
                //  Receive GroupId
                //  Get Group by Its Id
                //  DestroyHead
                fprintf(stdout, "Not yet Implemented!\n");
                fflush(stdout);
                break;
            }
            default: {
                fprintf(stderr, "Not a Valid Option!\n");
                fflush(stderr);
                break;
            }
        }
    } while(option != Exit);
}

/*
 *  Text
 */

void    printHeader() {
    //system("clear"); //FIXME
    fprintf(stdout, "========================================\n");
    fprintf(stdout, "----------------WhatsP2P----------------\n");
    fprintf(stdout, "========================================\n");
    fprintf(stdout, "UserId: %s\n", userId);
    mutexLock(mutex_list_messages);
    fprintf(stdout, "Inbox.: %d\n", messages->size);
    mutexUnlock(mutex_list_messages);
    fflush(stdout);
}

int     mainMenu() {
    printHeader();
    fprintf(stdout, "\nMenu\n");
    fprintf(stdout, "[%.2d] - Direct Message\n", DirectMessage);
    fprintf(stdout, "[%.2d] - Direct File\n", DirectFile);
    fprintf(stdout, "[%.2d] - Group Message\n", GroupMessage);
    fprintf(stdout, "[%.2d] - Group File\n", GroupFile);
    fprintf(stdout, "[%.2d] - Inbox\n", Inbox);
    fprintf(stdout, "[%.2d] - Contacts\n", Contacts);
    fprintf(stdout, "[%.2d] - Exit\n", Exit);
    fprintf(stdout, "Option: ");
    fflush(stdout);

    int option;
    __fpurge(stdin);
    scanf("%d", &option);
    __fpurge(stdin);
    return option;
}

int     displayContactsMenu() {
    printHeader();
    fprintf(stdout, "\nMenu\n");
    fprintf(stdout, "[%.2d] - List Users\n", ListUsers);
    fprintf(stdout, "[%.2d] - List Groups\n", ListGroups);
    fprintf(stdout, "[%.2d] - New Contact\n"), NewContact;
    fprintf(stdout, "[%.2d] - Delete Contact\n", NewGroup);
    fprintf(stdout, "[%.2d] - New Group\n", DeleteGroup);
    fprintf(stdout, "[%.2d] - Delete Group\n", DeleteGroup);
    fprintf(stdout, "[%.2d] - Exit\n", Exit);
    fprintf(stdout, "Option: ");
    fflush(stdout);

    int option;
    __fpurge(stdin);
    scanf("%d", &option);
    __fpurge(stdin);
    return option;
}

/*
 *  User Input
 */
void    directMessage(char* userId, char** peerId, Message_t** msg) {
    char messageText[80];

    getString("PeerId: ", peerId, (UserId_Len + 1) * sizeof(char));

    fprintf(stdout, "Message: ");
    fflush(stdout);

    fgets(messageText, MessageMaxSize * sizeof(char), stdin);
    __fpurge(stdin);

    newMessage(msg, userId, sizeof(messageText), messageText);
}

void    directFile(char* userId, char** peerId, File_t** file) {
    char filePath[FileName_Len];

    getString("PeerId: ", peerId, (UserId_Len + 1) * sizeof(char));

    fprintf(stdout, "File Path: ");
    fflush(stdout);

    fgets(filePath, FileName_Len * sizeof(char), stdin);
    __fpurge(stdin);

    disk2Memory(file, filePath, userId);
}

void    groupMessage(char* userId, char** groupId, Message_t** msg) {
    char messageText[80];

    getString("GroupId: ", groupId, (UserId_Len + 1) * sizeof(char));

    fprintf(stdout, "Message: ");
    fflush(stdout);

    fgets(messageText, MessageMaxSize * sizeof(char), stdin);
    __fpurge(stdin);

    newMessage(msg, userId, sizeof(messageText), messageText);
}

void    groupFile(char* userId, char** groupId, File_t** file) {
    char filePath[FileName_Len];

    getString("GroupId: ", groupId, (UserId_Len + 1) * sizeof(char));

    fprintf(stdout, "File Path: ");
    fflush(stdout);

    fgets(filePath, FileName_Len * sizeof(char), stdin);
    __fpurge(stdin);

    disk2Memory(file, filePath, userId);
}

void    getString(char* inputStr, char** id, int maxStrLen) {
    fprintf(stdout, inputStr);
    fflush(stdout);

    fgets(*id, maxStrLen, stdin);
    __fpurge(stdin);
}

void enter2Continue() {
    fprintf(stdout, "Press Enter to continue.\n");
    fflush(stdout);
    getchar();
    __fpurge(stdin);
}
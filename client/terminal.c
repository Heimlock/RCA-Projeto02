
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

/*
 *  Logic
 */

/*
 * noResponse Thread
 */
void    *init(void* arg) {
    int option = -1;
    char userId[10];

    fprintf(stdout, "Enter with your 9 digit ID\n");
    fprintf(stdout, "UserId: ");
    fflush(stdout);
    fgets(userId, (UserId_Len + 1) * sizeof(char), stdin);
    fflush(stdin);

    //  LogIn

    do {
        option = -1;
        option = mainMenu();
        switch (option) {
            case DirectMessage: {
                char peerId[10];
                Message_t *directMsg;
                directMessage(userId, &peerId, &directMsg);
                //  Send Text Message
                break;
            }
            case GroupMessage: {
                char groupId[10];
                Message_t *groupMsg;
                LinkedListNode* groupNode;
                groupMessage(userId, &groupId, &groupMsg);
                groupNode = getNode(*groups, groupId);
                if(groupNode != NULL) {
                    //  Send Message For Each Node
                } else {
                    fprintf(stderr, "Group not Found.\n");
                    fflush(stderr);
                }
                break;
            }
            case DirectFile: {
                char peerId[10];
                File_t *directFile;
                directFile(userId, &peerId, &directFile);
                //  Send File Message
                break;
            }
            case GroupFile: {
                char peerId[10];
                File_t *groupFile;
                LinkedListNode* groupNode;
                groupFile(userId, &peerId, &groupFile);
                //  Send File Message
                groupNode = getNode(*groups, groupId);
                if(groupNode != NULL) {
                    //  Send File For Each Node
                } else {
                    fprintf(stderr, "Group not Found.\n");
                    fflush(stderr);
                }
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
                            system("pause");    //  Avaliar necessidade
                        }
                    } while(msgNode != NULL);
                } else {
                    fprintf(stderr, "No Messages to Read.\n");
                    fflush(stderr);
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
                break;
            }
        }
    }while(!option == Exit);
    //  LogOut
    threadExit(NULL);
}

void    contactsSubMenu () {
    int option;
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
    system("clear");
    fprintf(stdout, "========================================\n");
    fprintf(stdout, "----------------WhatsP2P----------------\n");
    fprintf(stdout, "========================================\n");
    mutexLock(mutex_list_messages);
    fprintf(stdout, "Inbox: %d\n", messages->size);
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
    scanf("%d", &option);
    fflush(stdin);
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
    scanf("%d", &option);
    fflush(stdin);
    return option;
}

/*
 *  User Input
 */
void    directMessage(char* userId, char** peerId, Message_t** msg) {
    char messageText[80];

    getString("PeerId: \n", peerId, (UserId_Len + 1) * sizeof(char));

    fprintf(stdout, "Message: ");
    fflush(stdout);
    fgets(messageText, MessageMaxSize * sizeof(char), stdin);
    fflush(stdin);

    newMessage(msg, userId, sizeof(messageText), messageText);
}

void    directFile(char* userId, char** peerId, File_t** file) {
    char filePath[FileName_Len];

    getString("PeerId: \n", peerId, (UserId_Len + 1) * sizeof(char));

    fprintf(stdout, "File Path: ");
    fflush(stdout);
    fgets(filePath, FileName_Len * sizeof(char), stdin);
    fflush(stdin);

    disk2Memory(file, filePath, char* userId);
}

void    groupMessage(char* userId, char** groupId, Message_t* msg) {
    char messageText[80];

    getString("GroupId: \n", groupId, (UserId_Len + 1) * sizeof(char));

    fprintf(stdout, "Message: ");
    fflush(stdout);
    fgets(messageText, MessageMaxSize * sizeof(char), stdin);
    fflush(stdin);

    newMessage(&msg, userId, sizeof(messageText), messageText);
}

void    groupFile(char* userId, char** groupId, File_t* file) {
    char filePath[FileName_Len];

    getString("GroupId: \n", groupId, (UserId_Len + 1) * sizeof(char));

    fprintf(stdout, "File Path: ");
    fflush(stdout);
    fgets(filePath, FileName_Len * sizeof(char), stdin);
    fflush(stdin);

    disk2Memory(file, filePath, userId);
}

void    getString(char* inputStr, char** id, int maxStrLen) {
    fprintf(stdout, inputStr);
    fflush(stdout);
    fgets(*id, maxStrLen, stdin);
    fflush(stdin);
}

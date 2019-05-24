
/*
 *		Redes de Computadores A
 *      Projeto 02 - WhatsAp2p, 
 *      Sistema de Mensageiro peer-to-peer hibrido
 *
 *	Integrantes:
 *		Felipe Moreira Ferreira       RA: 16116469
 *
 *  Biblioteca de Recursos Referentes a 
 *  manipulacao de uma Lista Ligada
 */

/*
 * #define KEY_LEN 9    //  Implementar Aonde for Usado
 * #define DATA_LEN 80  //  Implementar Aonde for Usado
 */

typedef	struct LinkedListHead {
    int size;
    LinkedListNode* initialNode;

    // void* add(char* key, void* data);
    // void* remove(char* key);
    // LinkedListNode* get(char* key);
    // void* destroy();
} LinkedListHead;

typedef	struct LinkedListNode {
    char*   key;
    void*   data;
    LinkedListNode* next;
    // void* destroy();
} LinkedListNode;

LinkedListHead* initList();
LinkedListNode* initNode(char* key, void* data);
void destroyHead(LinkedListHead* head);
void destroyNode(LinkedListNode* node);

void addNode(LinkedListHead* head, char* key, void* data);
void removeNode(LinkedListHead* head, char* key);
LinkedListNode* getNode(LinkedListHead* head, char* key);

#define addNode_Macro(key, data) ( addNode(head, key, data) )
#define removeNode_Macro(key) ( removeNode(head, key, data) )
#define getNode_Macro(key) ( addNode(head, key, data) )
#define destroyHead_Macro(key) ( destroyHead(head) )
#define destroyNode_Macro(key) ( destroyNode(node) )
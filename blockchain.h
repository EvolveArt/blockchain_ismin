
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdbool.h>
#include <time.h>
/////
// Constantes
/////

#define HASH_SIZE 32
#define MESSAGE_SIZE 64

/////
// Structures
/////

typedef struct Block_
{
    unsigned int index;
    char hash[HASH_SIZE];
    char previousHash[HASH_SIZE];
    unsigned int timestamp;
    char message[MESSAGE_SIZE];
    struct Block_ *next;
} Block;

typedef struct Blockchain_
{
    Block *head;
    int length;
} Blockchain;

/////
// Fonctions
/////

// Crypto
char *computeHash(Block *block);
void hash256(unsigned char *output, const char *input);

// Blockchain
Block generateNextBlock(char message[MESSAGE_SIZE], Blockchain *blockchain);
bool isValidNewBlock(Block *newBlock, Block *previousBlock);
bool isValidBlockStructure(Block *block);
bool isValidChain(Blockchain *blockchainToValidate);
void replaceChain(Blockchain *newChain);

#endif
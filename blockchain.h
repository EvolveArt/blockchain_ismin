
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdbool.h>
#include <time.h>
/////
// Constantes
/////

#define HASH_SIZE 65
#define HASH_HEX_SIZE 65
#define MESSAGE_SIZE 65
#define BLOCK_STR_SIZE 30000

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
char *computeHash(Block *block, char *output);
void hash256(unsigned char *output, const char *input);
char *string_block(char *output, Block *block);
unsigned char *toString(Block block);
void hashPrinter(unsigned char *hash, int length);

void SHA256(const unsigned char *input, size_t length, unsigned char *md);

// Blockchain
Block generateNextBlock(char message[MESSAGE_SIZE], Blockchain *blockchain);
bool isValidNewBlock(Block *newBlock, Block *previousBlock);
// bool isValidBlockStructure(Block *block);
bool isValidChain(Blockchain *blockchainToValidate);
// void replaceChain(Blockchain *newChain);
Blockchain *initBlockchain();
void displayBlockchain(Blockchain *blockchain);

#endif
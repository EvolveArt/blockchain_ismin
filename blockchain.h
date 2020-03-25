
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdbool.h>
#include <time.h>
/////
// Constantes
/////

#define HASH_SIZE 32
#define HASH_HEX_SIZE 65
#define BINARY_SIZE (HASH_HEX_SIZE * 4 + 1)
#define MESSAGE_SIZE 65
#define BLOCK_STR_SIZE 100

#define BLOCK_GENERATION_INTERVAL 10     // seconds
#define DIFFICULTY_ADJUSMENT_INTERVAL 10 // blocks

/////
// Structures
/////

typedef struct Block_
{
    unsigned int index;
    char hash[HASH_HEX_SIZE];
    char previousHash[HASH_HEX_SIZE];
    long int timestamp;
    char message[MESSAGE_SIZE];

    unsigned int difficulty;
    unsigned int nonce;

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
void hexToBinary(const char *input, char *output);

// Blockchain
Block generateNextBlock(char message[MESSAGE_SIZE], Blockchain *blockchain);
bool isValidNewBlock(Block *newBlock, Block *previousBlock);
// bool isValidBlockStructure(Block *block);
bool isValidChain(Blockchain *blockchainToValidate);
// void replaceChain(Blockchain *newChain);
Blockchain *initBlockchain();
void displayBlockchain(Blockchain *blockchain);

// Proof of work
bool hashMatchesDifficulty(const char hash[HASH_SIZE], const int difficulty);
int getDifficulty(Blockchain *blockchain);
int getAdjustedDifficulty(Block *latestBlock, Blockchain *blockchain);
bool isValidTimestamp(Block *newBlock, Block *previousBlock);

// Utils
bool startsWith(const char *pre, const char *str);

// Distributed computing


#endif
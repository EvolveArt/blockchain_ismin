#include "blockchain.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

void SHA256(const unsigned char *input, size_t length, unsigned char *md)
{
    return;
}

void hash256(unsigned char *output, const char *input)
{

    size_t length = strlen(input);
    unsigned char md[HASH_SIZE];
    SHA256((const unsigned char *)input, length, md);
    memcpy(output, md, HASH_SIZE);

    return;
}

char *computeHash(Block *block, char *output)
{

    char block_string[BLOCK_STR_SIZE];
    string_block(block_string, block);

    unsigned char hash_value[HASH_SIZE];
    hash256(hash_value, block_string);

    char buffer[3];
    char hex_hash[HASH_HEX_SIZE] = {0};
    for (int i = 0; i < HASH_SIZE; i++)
    {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%02x", hash_value[i]);
        strcat(hex_hash, buffer);
    }

    strcpy(output, hex_hash);

    output[HASH_HEX_SIZE] = 0;

    return output;
}

char *string_block(char *output, Block *block)
{

    if (block == NULL || output == NULL)
        return NULL;

    char block_string[BLOCK_STR_SIZE] = {0};

    //Add index and time
    sprintf(block_string, "%010i.%010i.", block->index, block->timestamp);

    //Add previous hash
    strcat(block_string, block->previousHash);
    strcpy(output, block_string);

    //Add message
    strcat(block_string, block->message);
    strcpy(output, block_string);

    return output;
}

/**
 * Generation du block suivant
 * params: 
 * message: char[MESSAGE_SIZE] données du block suivant
 * blockchain: blockchain sur laquelle ajouter le block
**/
Block generateNextBlock(char message[MESSAGE_SIZE], Blockchain *blockchain)
{

    Block *currentBlock = blockchain->head;
    Block *newBlock = (Block *)malloc(sizeof(Block));

    newBlock->next = currentBlock;
    newBlock->index = currentBlock->index + 1;
    strcpy(newBlock->message, message);
    newBlock->timestamp = time(NULL);
    strcpy(newBlock->previousHash, currentBlock->hash);
    computeHash(newBlock, newBlock->hash);

    blockchain->head = newBlock;
    blockchain->length++;

    return *newBlock;
}

bool isValidNewBlock(Block *newBlock, Block *previousBlock)
{
    char hash_to_test[HASH_SIZE];

    if (previousBlock->index + 1 != newBlock->index)
    {
        printf("Block invalide : index invalide.");
        return false;
    }
    else if (strcmp(previousBlock->hash, newBlock->previousHash) != 0)
    {
        printf("Block invalide : hash précédent invalide.");
        return false;
    }
    else if (strcmp(computeHash(newBlock, hash_to_test), newBlock->hash) != 0)
    {
        printf("Block invalide: hashs différents (%s != %s)", hash_to_test, newBlock->hash);
        return false;
    }

    return true;
}

bool isValidChain(Blockchain *blockchainToValidate)
{
    Block *currentBlock = blockchainToValidate->head;
    while (currentBlock->next)
    {
        if (!isValidNewBlock(currentBlock->next, currentBlock))
        {
            printf("Le block d'index %d est invalide.", currentBlock->next->index);
            return false;
        }
        currentBlock = currentBlock->next;
    }

    return true;
}

unsigned char *toString(Block block)
{
    unsigned char *str = malloc(sizeof(unsigned char) * sizeof(block));
    memcpy(str, &block, sizeof(block));
    return str;
}

void hashPrinter(unsigned char *hash, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%02x", hash[i]);
    }
}

Blockchain *initBlockchain()
{

    Blockchain *blockchain = (Blockchain *)malloc(sizeof(Blockchain));

    // Création du premier block
    Block *genesisBlock = (Block *)malloc(sizeof(Block));
    genesisBlock->index = 0;
    strcpy(genesisBlock->hash, "816534932c2b7154836da6afc367695e6337db8a921823784c14378abed4f7d7");
    strcpy(genesisBlock->message, "ISMIN");
    strcpy(genesisBlock->previousHash, "");
    genesisBlock->timestamp = time(NULL);
    genesisBlock->next = NULL;

    // Initialisation de la blockchain
    blockchain->head = genesisBlock;
    blockchain->length = 1;

    return blockchain;
}

void displayBlockchain(Blockchain *blockchain)
{
    Block *currentBlock = blockchain->head;

    printf("\nLongueur de la blockchain : %d", blockchain->length);

    while (currentBlock)
    {
        printf("\n Block %d : msg->'%s' hash->'%s' timestamp->'%d'", currentBlock->index, currentBlock->message, currentBlock->hash, currentBlock->timestamp);
        currentBlock = currentBlock->next;
    }
}
#include "blockchain.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

void hexToBinary(char *input, char *output)
{

    long int i = 0;
    while (input[i])
    {

        switch (input[i])
        {
        case '0':
            strcat(output, "0000");
            break;
        case '1':
            strcat(output, "0001");
            break;
        case '2':
            strcat(output, "0010");
            break;
        case '3':
            strcat(output, "0011");
            break;
        case '4':
            strcat(output, "0100");
            break;
        case '5':
            strcat(output, "0101");
            break;
        case '6':
            strcat(output, "0110");
            break;
        case '7':
            strcat(output, "0111");
            break;
        case '8':
            strcat(output, "1000");
            break;
        case '9':
            strcat(output, "1001");
            break;
        case 'A':
        case 'a':
            strcat(output, "1010");
            break;
        case 'B':
        case 'b':
            strcat(output, "1011");
            break;
        case 'C':
        case 'c':
            strcat(output, "1100");
            break;
        case 'D':
        case 'd':
            strcat(output, "1101");
            break;
        case 'E':
        case 'e':
            strcat(output, "1110");
            break;
        case 'F':
        case 'f':
            strcat(output, "1111");
            break;
        default:
            printf("\nInvalid hexadecimal digit %c",
                   input[i]);
        }
        i++;
    }
}

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

void SHA256(const unsigned char *input, size_t length, unsigned char *md)
{
    return;
}

bool hashMatchesDifficulty(char hash[HASH_SIZE], const int difficulty)
{
    // Conversion en binaire du hash
    char binaryHash[BINARY_SIZE];
    hexToBinary(hash, binaryHash);

    // Creation de la chaine de caractère "0"*difficulty
    char *prefix = malloc(difficulty + 1);
    memset(prefix, '0', difficulty);
    prefix[difficulty] = '\0';

    return startsWith(prefix, hash);
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
    //computeHash(newBlock, newBlock->hash);
    strcpy(newBlock->hash, "0");

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
    strcpy(genesisBlock->hash, "8216534932c2b7154836da6afc367695e6337db8a921823784c14378abed4f7d7");
    strcpy(genesisBlock->message, "ISMIN 1");
    strcpy(genesisBlock->previousHash, "");
    genesisBlock->timestamp = time(NULL);
    genesisBlock->next = NULL;

    genesisBlock->difficulty = 0;
    genesisBlock->nonce = 0;

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
        printf("\n Block %d : msg->'%s' hash->'%s' prevhash->'%s' timestamp->'%d'", currentBlock->index, currentBlock->message, currentBlock->hash, currentBlock->previousHash, currentBlock->timestamp);
        currentBlock = currentBlock->next;
    }
}

Block findBlock(int index, char prevHash[HASH_SIZE], unsigned int timestamp, char message[MESSAGE_SIZE], unsigned int difficulty)
{

    Block *block_ = (Block *)malloc(sizeof(Block));
    strcpy(block_->previousHash, prevHash);
    strcpy(block_->message, message);
    block_->timestamp = timestamp;
    block_->index = index;
    block_->difficulty = difficulty;

    unsigned int nonce = 0;
    while (true)
    {
        const char hash[HASH_SIZE];
        computeHash(block_, hash);
        if (hashMatchesDifficulty(hash, difficulty))
        {
            block_->nonce = nonce;
            strcpy(block_->hash, hash);
            return *block_;
        }
        nonce++;
    }
}
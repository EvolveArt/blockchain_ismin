#include <blockchain.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void hash256(unsigned char *output, const char *input)
{

    size_t length = strlen(input);
    unsigned char md[32];
    SHA256((const unsigned char *)input, length, md);
    memcpy(output, md, 32);

    return;
}

char *computeHash(Block *block)
{
    return "";
}

/**
 * Generation du block suivant
 * params: 
 * message: char[MESSAGE_SIZE] données du block suivant
 * blockchain: blockchain sur laquelle ajouter le block
**/
Block generateNextBlock(char message[MESSAGE_SIZE], Blockchain *blockchain)
{
    if (blockchain->head == NULL)
    {
        Block *newHead = (Block *)malloc(sizeof(Block));
        SHA256("", sizeof(""), newHead->previousHash);
        strcpy(newHead->message, message);
        blockchain->head = newHead;
        return *newHead;
    }

    Block *currentBlock = blockchain->head;
    while (currentBlock->next)
        currentBlock = currentBlock->next;

    Block *newBlock = (Block *)malloc(sizeof(Block));
    currentBlock->next = newBlock;
    strcpy(newBlock->message, message);
    SHA256(toString(*currentBlock), sizeof(*currentBlock), newBlock->previousHash);

    return *newBlock;
}

bool isValidNewBlock(Block *newBlock, Block *previousBlock)
{
    hashPrinter(SHA256(toString(*previousBlock), sizeof(*previousBlock)), HASH_SIZE);
    hashPrinter(newBlock->previousHash, HASH_SIZE);
    if (hashCompare(SHA256(toString(*previousBlock), sizeof(*previousBlock), newBlock->previousHash)))
        return true;
    else
        return false;
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
#include "blockchain.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <openssl/sha.h>

void hexToBinary(const char *input, char *output)
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
    char *checker = NULL;
    checker = strstr(str, pre);
    if(checker == str) return true;
    return false;
}

bool hashMatchesDifficulty(const char *hash, const int difficulty)
{
    // Conversion en binaire du hash
    char binaryHash[BINARY_SIZE] = "";
    hexToBinary(hash, binaryHash);

    // Creation de la chaine de caractère "0"*difficulty
    char *prefix = malloc(difficulty + 1);
    memset(prefix, '0', difficulty);
    prefix[difficulty] = '\0';

    //printf("\n bin : %s", binaryHash);

    return startsWith(prefix, binaryHash);
}

void hash256(unsigned char *output, const char *input)
{

    size_t length = strlen(input);
    unsigned char md[HASH_SIZE];
    SHA256((const unsigned char*)input, length, md);
    memcpy(output,md, HASH_SIZE);

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
    for(int i = 0; i < HASH_SIZE; i++) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer,"%02x", hash_value[i]);
        strcat(hex_hash, buffer);
    }

    strcpy(output,hex_hash);

    output[HASH_HEX_SIZE] = 0;

    return output;
}

char *string_block(char *output, Block *block)
{

    if (block == NULL || output == NULL)
        return NULL;

    char block_string[BLOCK_STR_SIZE] = {0};

    //Add index and time
    sprintf(block_string, "%010i.%010li.%010i", block->index, block->timestamp, block->nonce);

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
    strcpy(newBlock->previousHash, currentBlock->hash);

    strcpy(newBlock->message, message);

    int difficulty = getDifficulty(blockchain);
    printf("\nBlockchain Difficulty = %d", difficulty);

    unsigned int nonce = 0;
    while (true)
    {
        newBlock->timestamp = time(NULL);
        newBlock->nonce = nonce;
        const char hash[HASH_HEX_SIZE];
        computeHash(newBlock, hash);
        //printf("\nhash = %s", hash);
        if (hashMatchesDifficulty(hash, difficulty))
        {
            strcpy(newBlock->hash, hash);
            break;
        }
        nonce++;
    }    

    newBlock->difficulty = difficulty;
    

    blockchain->head = newBlock;

    blockchain->length++;
    printf("\nNew Block found of index %d", newBlock->index);
    return *newBlock;
}

bool isValidNewBlock(Block *newBlock, Block *previousBlock)
{
    char hash_to_test[HASH_HEX_SIZE];

    if (previousBlock->index + 1 != newBlock->index)
    {
        printf("\nBlock invalide : indexs invalide (%d + 1 != %d).", previousBlock->index, newBlock->index);
        return false;
    }
    else if (strcmp(previousBlock->hash, newBlock->previousHash) != 0)
    {
        printf("\nBlock invalide : hash précédent invalide.");
        return false;
    }
    else if (strcmp(computeHash(newBlock, hash_to_test), newBlock->hash) != 0)
    {
        printf("\nBlock invalide: hashs différents (%s != %s)", hash_to_test, newBlock->hash);
        return false;
    }
    else if(!isValidTimestamp(newBlock, previousBlock)) {
        printf("\nBlock invalide : Les timestamps ne concordent pas.");
    }

    return true;
}

bool isValidChain(Blockchain *blockchainToValidate)
{
    Block *currentBlock = blockchainToValidate->head;
    while (currentBlock->next)
    {
        if (!isValidNewBlock(currentBlock, currentBlock->next))
        {
            printf("\nLe block d'index %d est invalide.", currentBlock->next->index);
            return false;
        }
        currentBlock = currentBlock->next;
    }

    printf("\nLa blockchain est valide.");
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
    strcpy(genesisBlock->message, "ISMIN 1");
    strcpy(genesisBlock->previousHash, "");
    genesisBlock->timestamp = time(NULL);
    genesisBlock->next = NULL;

    genesisBlock->difficulty = 1;
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
        printf("\n Block %d : msg->'%s' hash->'%s' prevhash->'%s' timestamp->'%ld'", currentBlock->index, currentBlock->message, currentBlock->hash, currentBlock->previousHash, currentBlock->timestamp);
        currentBlock = currentBlock->next;
    }
}


int getDifficulty(Blockchain *blockchain)
{
    Block *latestBlock = blockchain->head;
    if (latestBlock->index % DIFFICULTY_ADJUSMENT_INTERVAL == 0 && latestBlock->index != 0)
    {
        printf("\nAdjusting difficulty.. ");
        return getAdjustedDifficulty(latestBlock, blockchain);
    }
    else
    {
        return latestBlock->difficulty;
    }
}

int getAdjustedDifficulty(Block *latestBlock, Blockchain *blockchain)
{
    Block *prevAdjustmentBlock = blockchain->head;
    // Sélection du dernier block d'ajustement de la difficulté
    for (size_t i = 0; i < DIFFICULTY_ADJUSMENT_INTERVAL; i++)
    {
        prevAdjustmentBlock = prevAdjustmentBlock->next;
    }

    // Temps écoulé supposé
    int expectedTime = DIFFICULTY_ADJUSMENT_INTERVAL * BLOCK_GENERATION_INTERVAL;
    // Temps réellement écoulé
    int takenTime = latestBlock->timestamp - prevAdjustmentBlock->timestamp;

    // Ajustement de la difficulté
    if (takenTime < expectedTime / 2)
    {
        return prevAdjustmentBlock->difficulty + 1;
    }
    else if (takenTime > expectedTime * 2)
    {
        return prevAdjustmentBlock->difficulty - 1;
    }
    else
    {
        return prevAdjustmentBlock->difficulty;
    }
}

/**
 * Validation d'un nouveau block par rapport a son timestamp
 * Le block est valide si : 
 * - Le timestamp est au plus 1 min dans le futur du temps actuel
 * - Le timestamp est au plus 1 min dans le passé du block précédent
 * 
 * params: 
 * newBlock : Block* -> nouveau block a valider
 * previousBlock : Block* -> block précédent dans la blockchain
 * 
 * return: bool -> true si le block est valide
 * 
 **/
bool isValidTimestamp(Block *newBlock, Block *previousBlock)
{
    return (previousBlock->timestamp - 60 < newBlock->timestamp) && (newBlock->timestamp - 60 < time(NULL));
}


/**
 * Interface Utilisateur
 * ---------------------
 * Choix : 
 * 1. Ajouter un nouveau PC
 * 2. Générer un nouveau block : message
 * 3. Afficher la liste des PC
 * 4. Afficher la Blockchain
 * **/



/**
 * 
 * DISTRIBUTED COMPUTING
 * -> Création de threads pour simuler des "nodes"
 * -> Synchronisation des blockchains entre les "nodes"
 * **/


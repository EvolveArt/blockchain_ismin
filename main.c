#include <stdlib.h>
#include <stdio.h>
#include "blockchain.h"

int main()
{
    printf("Program starting...");

    Blockchain *blockchain_ = initBlockchain();

    generateNextBlock("ISMIN 2", blockchain_);
    generateNextBlock("ISMIN 3", blockchain_);
    generateNextBlock("ISMIN 4", blockchain_);

    displayBlockchain(blockchain_);

    return 0;
}
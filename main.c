#include <stdlib.h>
#include <stdio.h>
#include "blockchain.h"
#include <stdbool.h>

int main()
{
    printf("Program starting...");

    Blockchain *blockchain_ = initBlockchain();

    generateNextBlock("ISMIN 2", blockchain_);
    generateNextBlock("ISMIN 3", blockchain_);
    generateNextBlock("ISMIN 4", blockchain_);
    generateNextBlock("ISMIN 5", blockchain_);
    generateNextBlock("ISMIN 6", blockchain_);
    generateNextBlock("ISMIN 7", blockchain_);
    generateNextBlock("ISMIN 8", blockchain_);
    generateNextBlock("ISMIN 9", blockchain_);
    generateNextBlock("ISMIN 10", blockchain_);
    generateNextBlock("ISMIN 11", blockchain_);
    generateNextBlock("ISMIN 12", blockchain_);
    generateNextBlock("ISMIN 13", blockchain_);

    displayBlockchain(blockchain_);

    isValidChain(blockchain_);

    return 0;
}
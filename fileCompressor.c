#include "fileCompressor.h"

// 1) build max heap, bst, avl?
// 2) 0. Get your program to find all files in a directory and print out their names
//    1. Get your program to recursively descend down all subdirectories and print out their names with paths
//      - open all files and output them to stdout to test
//      - open all files and write them to <filename>.test to test
//    2. Build and test tokenizing static strings in your code
//      - count the number of occurrences of each token read in from files
//    3. Generate a Huffman codebook given a sequence of tokens and frequencies
//    4. Implement the -b and -R flags and test writing out a Huffman code book
//    5.  . ... etc.

void printAll(char* path) {

    char pathToNext[1000];
    DIR* dir;
    struct dirent* entry;
    if((dir = opendir(path)) != NULL) {

        while((entry = readdir(dir)) != NULL) {
            if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0) {
                printf("%s\n", entry->d_name);
                strcpy(pathToNext, path);
                strcat(pathToNext, "/");
                strcat(pathToNext, entry->d_name);
                printAll(pathToNext);
            }
        }

        closedir(dir);

    }

}

int fileExists(char* fName)
{
    struct stat buf;
    int res = stat(fName, &buf);
    if(res == 0)
    {
        return 1; //file exists
    }
    else
    {
        return 0; //file does not exist
    }
}

FreqTree* readAndBuildTree(int fd) {

	char buffer[200];
	int i;
	for(i=0; i<200; i ++) {

		buffer[i]='\0';

	}

	int bytesRead;
	int tokLength=0;
	FreqTree *tokFreqTreeRoot = createFreqTree();

	do {

		char c=0;
		bytesRead = read(fd, &c, sizeof(char));
		//CHECKS
		if(isspace(c) || bytesRead == 0) {

			/*Create Node HERE*/
            if(buffer[0] != '\0') {
                tokFreqTreeRoot = insertIntoFreqTree(tokFreqTreeRoot, buffer);
            }
            else {
                refresh(buffer, 1);
            }

			refresh(buffer, 200);
			tokLength=0;

			//whitespace
			buffer[0] = c;
            if(buffer[0] == ' ') {
                buffer[0] = '!';
                buffer[1] = 's';
            }
            else if(buffer[0] == '\t') {
                buffer[0] = '!';
                buffer[1] = 't';
            }
            else if(buffer[0] == '\n') {
                buffer[0] = '!';
                buffer[1] = 'n';
            }
            else if(buffer[0] == '\0') {
                refresh(buffer, 1);
                continue;
            }
			tokFreqTreeRoot = insertIntoFreqTree(tokFreqTreeRoot, buffer);
            refresh(buffer, 2);

		}
		else {

			buffer[tokLength] = c;
			tokLength ++;

		}

	} while(bytesRead > 0);

	return tokFreqTreeRoot;

}

void refresh(char* buffer, int count){
	int i;
	for (i = 0; i < count; i++) {
		buffer[i] = '\0';
	}
}

int findNumLeafNodes(TreeNode* root)
{
    if(root == NULL)
    {
        return 0;
    }
    if(root->left == NULL && root->right == NULL)
    {
        return 1;
    }
    else
    {
        return findNumLeafNodes(root->left) + findNumLeafNodes(root->right);
    }
}

int findHeight(TreeNode* root)
{
    if(root == NULL)
    {
        return 0;
    }
    else
    {
        int leftHeight = findHeight(root->left);
        int rightHeight = findHeight(root->right);

        if(leftHeight > rightHeight)
        {
            return leftHeight + 1;
        }
        else
        {
            return rightHeight + 1;
        }
    }
}

int indexLeafNodeOne = 0;

void getTokens(TreeNode* root, CodebookNode* leafNodes)
{
    if(root->left) 
    {
        //printf("yerLeft\n");
        getTokens(root->left, leafNodes);
    }
    if(root->right)
    {
        //printf("yerRight\n");
        getTokens(root->right, leafNodes);    
    }
    if(!(root->left || root->right))
    {
        leafNodes[indexLeafNodeOne].token = root->token;
        indexLeafNodeOne ++;
    }
}

int indexLeafNodeTwo = 0;

void getBitSequences(TreeNode* root, CodebookNode* leafNodes, char* bitSequence, int indexBitSequence)
{
    if(root->left)
    {
        //leafNodes[indexLeafNodeTwo].bitSequence[indexBitSequence] = '0';
        bitSequence[indexBitSequence] = '0';
        getBitSequences(root->left, leafNodes, bitSequence, indexBitSequence + 1);
    }
    if(root->right)
    {
        //leafNodes[indexLeafNodeTwo].bitSequence[indexBitSequence] = '1';
        bitSequence[indexBitSequence] = '1';
        getBitSequences(root->right, leafNodes, bitSequence, indexBitSequence + 1);
    }
    if(!(root->left || root->right))
    {
        leafNodes[indexLeafNodeTwo].bitSequence = strdup(bitSequence);
        memset(leafNodes[indexLeafNodeTwo].bitSequence + indexBitSequence, '\0', strlen(leafNodes[indexLeafNodeTwo].bitSequence) - indexBitSequence);
        indexLeafNodeTwo ++;
        return;
    }
}

//CodebookNode not populated inOrder (indexLeafNode not incrementing correctly)
//

void freeCodebookNodes(CodebookNode* leafNodes)
{
    free(leafNodes->token);
    free(leafNodes->bitSequence);
    free(leafNodes);
}

void buildCodebook(char* path)
{    
    //open file
    //CHECK IF FILE DOES NOT EXIST
    int fd = open(path, O_RDONLY);
    if(fd == -1) { //if file does not exist
	    int errsv = errno;
		printf("Error: file \"");
		printf("%s\" ", path);
		printf("does not exist.\n");
		close(fd);
		return;
	}
    
    //CHECK IF FILE IS EMPTY
    char c;
    int bytesRead = read(fd, &c, 1);
    if(bytesRead == 0)
    {
        printf("Warning: File \"%s\" is empty, skipping file.\n", path);
        close(fd);
        return;
    }
    close(fd);
    fd = open(path, O_RDONLY);
     //read and tokenize file using buildHelper in build.c (and refresh)
        //this gives us FreqTree
    //convert to MinHeap
    //free FreqTree
    //Build HuffmanTree
    //Use inorder traversal on HuffmanTree to make HuffmanCodebook file
    //free MinHeap/HuffmanTree
    FreqTree* tree = readAndBuildTree(fd);
    close(fd);
    //printFreqTree(tree);

    MinHeap* tokenMinHeap = convertFreqTreeToMinHeap(tree);
    /*
    int i;
    for(i=0; i<(tokenMinHeap->heapSize); i ++) {
        HeapNode* tmp = tokenMinHeap->nodes[i];
        char* token = tmp->data->token;
        int count = tmp->count;
        printf("%s\t%d\n", token, count);
    }
    */
    
    freeFreqTree(tree);
    
    TreeNode* huffmanTree = buildHuffmanTree(tokenMinHeap);
    //printTree(huffmanTree);
    //printf("\n");

    //find number of leaf nodes
    int numLeafNodes = findNumLeafNodes(huffmanTree);
    //printf("%d\n", numLeafNodes);

    //Use number of leaf nodes to create an array of size numLeafNodes, this is an array of CodebookNodes
    CodebookNode* leafNodes = (CodebookNode*)(malloc(numLeafNodes * sizeof(CodebookNode)));

    //find height of huffmanTree
    int h = findHeight(huffmanTree) - 1;
    //printf("%d\n", h);

    //h is the max number of turns possible to reach a leaf node
    int i;
    for(i = 0; i<numLeafNodes; i ++) 
    {
        leafNodes[i].bitSequence = (char*)(malloc(h * sizeof(char)));
        memset(leafNodes[i].bitSequence, '\0', h * sizeof(char));
    }

    //fill in token and bitSequence information in CodebookNodes array
    getTokens(huffmanTree, leafNodes);

    char* bitSequence = (char*)(malloc(h * sizeof(char)));
    memset(bitSequence, '\0', h * sizeof(char));
    getBitSequences(huffmanTree, leafNodes, bitSequence, 0);

    /*
    for(i=0; i<numLeafNodes; i ++) {
        printf("%s (%d) %s\n", leafNodes[i].token, i, leafNodes[i].bitSequence);
    }
    printf("\n");
    */

    //Now that we have CodebookNode array complete, loop through each, write to HuffmanCodebook file
    //int exist = fileExists("HuffmanCodebook"); 
    //if 1, exists and needs to be overwritten
    //if 0, does not exist and needs to be created
    
    char huffmanCodebookPath[1000];
    memset(huffmanCodebookPath, '\0', 1000 * sizeof(char));
    strcpy(huffmanCodebookPath, path);
    int sizeOfPath = strlen(huffmanCodebookPath) - 1;
    while(huffmanCodebookPath[sizeOfPath] != '/')
    {
        huffmanCodebookPath[sizeOfPath] = '\0';
        sizeOfPath --;
    }
    //printf("%s\n", huffmanCodebookPath);
    strcat(huffmanCodebookPath + sizeOfPath, "/HuffmanCodebook");
    remove(huffmanCodebookPath);
    fd = open(huffmanCodebookPath, O_WRONLY | O_CREAT, 00600);
    //printf("%d\n", fd);
    write(fd, "!\n", 2);
    for(i = 0; i<numLeafNodes; i ++)
    {
        char* bitSequenceBuf = leafNodes[i].bitSequence;
        char* tokenBuf = leafNodes[i].token;

        write(fd, bitSequenceBuf, strlen(bitSequenceBuf));
        write(fd, "\t", 1);
        write(fd, tokenBuf, strlen(tokenBuf));
        write(fd, "\n", 1);
    }
    write(fd, "\n", 1);
    close(fd);
    
    //QUESTION: Do we gotta overwrite existing HuffmanCodebook file if we use the build
    //functionality again?

    /*********************/
    //We have huffmanTree
    //Next: get encoded bitSequences using inorder traversal on the huffmanTree, store them into CodebookNode->bitSequence

        //Algorithm: once inorder traversal hits a leaf node, store the char* token into CodebookNode->token
                   //by the time it hits a leaf node, the recursion should have a full concatenated bitSequence to save to CodebookNode->bitSequence

        //we need to figure out how exactly this will work (maybe a method that looks like printTree, but it needs to also concatenate 1's and 0's to CodebookNode->bitSequence)
        //length of bitSequence array is determined by the height, height is determined by floor(log2(n))+1

    //Once CodebookNode array is complete, it should be in the inorder form.
    //Need to then write each node into the HuffmanCodebook file. Needs to be in the following form:
        // CodebookNode->token\tCodebookNode->bitSequence
    /*********************/
    
}

void recReadAndBuildTree(int fd, FreqTree* masterTree)
{
	char buffer[200];
	int i;
	for(i=0; i<200; i ++) {

		buffer[i]='\0';

	}

	int bytesRead;
	int tokLength=0;

	do {

		char c=0;
		bytesRead = read(fd, &c, sizeof(char));
        //printf("%d\n", bytesRead);
		//CHECKS
		if(isspace(c) || bytesRead == 0) {

			/*Create Node HERE*/
            if(buffer[0] != '\0') {
                masterTree = insertIntoFreqTree(masterTree, buffer);
            }
            else {
                refresh(buffer, 1);
            }

			refresh(buffer, 200);
			tokLength=0;

			//whitespace
			buffer[0] = c;
            if(buffer[0] == ' ') {
                buffer[0] = '!';
                buffer[1] = 's';
            }
            else if(buffer[0] == '\t') {
                buffer[0] = '!';
                buffer[1] = 't';
            }
            else if(buffer[0] == '\n') {
                buffer[0] = '!';
                buffer[1] = 'n';
            }
            else if(buffer[0] == '\0') {
                refresh(buffer, 1);
                continue;
            }
			masterTree = insertIntoFreqTree(masterTree, buffer);
            refresh(buffer, 2);

		}
		else {

			buffer[tokLength] = c;
			tokLength ++;

		}

	} while(bytesRead > 0);

}

void buildMasterTree(DIR* parentDir, FreqTree* masterTree, char* path)
{
    struct dirent* currElement = NULL;
    readdir(parentDir);
    readdir(parentDir);
    currElement = readdir(parentDir);

    while(currElement != NULL)
    {
        char pathToFile[1000];
        //if file
        if(currElement->d_type == DT_REG)
        {
            strcpy(pathToFile, path);
            strcat(pathToFile, "/");
            strcat(pathToFile, currElement->d_name);
            char* extensionPtr = strrchr(pathToFile, '.');
            if(extensionPtr != NULL)
            {
                if(strcmp(extensionPtr, ".hcz") == 0)
                {
                    currElement = readdir(parentDir);
                    continue;
                }
            }
            //printf("%s\n", pathToFile);
            int fd = open(pathToFile, O_RDONLY);
            //check immediately if file is empty
            char c;
            int bytesRead = read(fd, &c, 1);
            //printf("%d\n", bytesRead);
            if(bytesRead == 0)
            {
                close(fd);
                currElement = readdir(parentDir);
                continue;
            }
            close(fd);
            fd = open(pathToFile, O_RDONLY);
            recReadAndBuildTree(fd, masterTree);
            close(fd);
        }
        //else if directory
        else if(currElement->d_type == DT_DIR)
        {
            //printf("DIRECTORY!\n");
            strcpy(pathToFile, path);
            strcat(pathToFile, "/");
            strcat(pathToFile, currElement->d_name);
            DIR* subDir = opendir(pathToFile);
            buildMasterTree(subDir, masterTree, pathToFile);
            closedir(subDir);
        }

        currElement = readdir(parentDir);
    }
}

void recursiveBuildCodebook(char* path)
{
    //Determine of path is valid directory
    DIR* dir = opendir(path);
    if(dir) 
    {

    }
    else if(errno == ENOENT)
    {
        printf("dir does not exist\n");
        closedir(dir);
        return;
    }
    else
    {
        printf("dir failed to open due to an unknown reason\n");
    }
    //printAll(path);

    //Remove currently existing HuffmanCodebook file
    char huffmanCodebookPath[1000];
    strcpy(huffmanCodebookPath, path);
    strcat(huffmanCodebookPath, "/HuffmanCodebook");
    remove(huffmanCodebookPath);

    //Using parent directory, iterate through all files, create masterTree
    FreqTree* masterTree = createFreqTree();
    buildMasterTree(dir, masterTree, path);
    closedir(dir);
    //printFreqTree(masterTree);

    MinHeap* tokenMinHeap = convertFreqTreeToMinHeap(masterTree);
    freeFreqTree(masterTree);

    /*
    int i;
    for(i=0; i<(tokenMinHeap->heapSize); i ++) {
        HeapNode* tmp = tokenMinHeap->nodes[i];
        char* token = tmp->data->token;
        int count = tmp->count;
        printf("%s\t%d\n", token, count);
    }
    */
    
    TreeNode* masterHuffmanTree = buildHuffmanTree(tokenMinHeap);
    //printTree(huffmanTree);
    //printf("\n");

    //find number of leaf nodes
    int numLeafNodes = findNumLeafNodes(masterHuffmanTree);
    //printf("%d\n", numLeafNodes);

    //Use number of leaf nodes to create an array of size numLeafNodes, this is an array of CodebookNodes
    CodebookNode* leafNodes = (CodebookNode*)(malloc(numLeafNodes * sizeof(CodebookNode)));

    //find height of huffmanTree
    int h = findHeight(masterHuffmanTree) - 1;

    int i;
    for(i = 0; i<numLeafNodes; i ++) 
    {
        leafNodes[i].bitSequence = (char*)(malloc(h * sizeof(char)));
        memset(leafNodes[i].bitSequence, '\0', h * sizeof(char));
    }

    //fill in token and bitSequence information in CodebookNodes array
    getTokens(masterHuffmanTree, leafNodes);

    char* bitSequence = (char*)(malloc(h * sizeof(char)));
    memset(bitSequence, '\0', h * sizeof(char));
    getBitSequences(masterHuffmanTree, leafNodes, bitSequence, 0);

    //Now that we have CodebookNode array complete, loop through each, write to HuffmanCodebook file
    //int exist = fileExists("HuffmanCodebook"); 
    //if 1, exists and needs to be overwritten
    //if 0, does not exist and needs to be created
    int fd = open(huffmanCodebookPath, O_WRONLY | O_CREAT, 00600);
    //printf("%d\n", fd);
    write(fd, "!\n", 2);
    for(i = 0; i<numLeafNodes; i ++)
    {
        char* bitSequenceBuf = leafNodes[i].bitSequence;
        char* tokenBuf = leafNodes[i].token;

        write(fd, bitSequenceBuf, strlen(bitSequenceBuf));
        write(fd, "\t", 1);
        write(fd, tokenBuf, strlen(tokenBuf));
        write(fd, "\n", 1);
    }
    write(fd, "\n", 1);
    close(fd);
}

void compress(char* path, char* codebook)
{
    //Read HuffmanCodebook file
    HuffmanCodesTree* huffmanCodesTree = readHuffmanCodebook(codebook);
    if(huffmanCodesTree == NULL)
    {
        printf("There was an error in opening the codebook\n");
        return;
    }


    //printHuffmanCodesTree(huffmanCodesTree);
    //Now we have HuffmanCodesTree

    //open two files: path to read from, new .hcz file to write into
    int fdRead = open(path, O_RDONLY);
    if(fdRead == -1) { //if file does not exist
	    int errsv = errno;
		printf("Fatal Error: file \"");
		printf("%s\" ", path);
		printf("does not exist\n");
        freeHuffmanCodesTree(huffmanCodesTree);
		close(fdRead);
		return;
	}
    
    //CHECK IF FILE IS EMPTY
    char c;
    int bytesRead = read(fdRead, &c, 1);
    if(bytesRead == 0)
    {
        printf("Warning: File is empty!\n");
        freeHuffmanCodesTree(huffmanCodesTree);
        close(fdRead);
        return;
    }
    close(fdRead);
    fdRead = open(path, O_RDONLY);

    //get path for .hcz file
    char compressedFilePath[1000];
    strcpy(compressedFilePath, path);
    strcat(compressedFilePath, ".hcz");
    
    //create .hcz file to prepare for writing
    remove(compressedFilePath);
    int fdWrite = open(compressedFilePath, O_WRONLY | O_CREAT, 00600);

    //start to read from file
    char tokenRead[200];
    memset(tokenRead, '\0', 200 * sizeof(char));
    int tokLength = 0;
	do {

		c = 0;
		bytesRead = read(fdRead, &c, sizeof(char));
        //printf("%d\n", bytesRead);
		//CHECKS
		if(isspace(c) || bytesRead == 0)
        {
            //First step: write bitSequence found from tree into .hcz file
            if(tokenRead[0] != '\0') {
                char* bitSequenceToWrite = findBitSequence(huffmanCodesTree, tokenRead);
                write(fdWrite, bitSequenceToWrite, strlen(bitSequenceToWrite));
            }

            //Second step: find bitSequence of space character, write into .hcz file
            char* bitSequenceSpaceChar;
            if(c == ' ') {
                bitSequenceSpaceChar = findBitSequence(huffmanCodesTree, "!s");
            }
            else if(c == '\t') {
                bitSequenceSpaceChar = findBitSequence(huffmanCodesTree, "!t");
            }
            else if(c == '\n') {
                bitSequenceSpaceChar = findBitSequence(huffmanCodesTree, "!n");
            }
            else if(c == '\0') {
                memset(tokenRead, '\0', tokLength);
                continue;
            }
            if(bitSequenceSpaceChar != NULL)
            {
                write(fdWrite, bitSequenceSpaceChar, strlen(bitSequenceSpaceChar));
            }

            //Third step: refresh tokenRead to read next token
            memset(tokenRead, '\0', tokLength * sizeof(char));
            tokLength = 0;

        }
		else 
        {
			tokenRead[tokLength] = c;
			tokLength ++;
		}

	} while(bytesRead > 0);

    freeHuffmanCodesTree(huffmanCodesTree);
    close(fdRead);
    close(fdWrite);

}

void compressEachFile(DIR* parentDir, char* path, char* codebook)
{
    struct dirent* currElement = NULL;
    readdir(parentDir);
    readdir(parentDir);
    currElement = readdir(parentDir);

    while(currElement != NULL)
    {
        char pathToFile[1000];
        //if file
        if(currElement->d_type == DT_REG)
        {
            //do ALL THE CHECKS
            strcpy(pathToFile, path);
            strcat(pathToFile, "/");
            strcat(pathToFile, currElement->d_name);
            char* extensionPtr = strrchr(pathToFile, '.');
            if(extensionPtr != NULL)
            {
                if(strcmp(extensionPtr, ".hcz") == 0)
                {
                    currElement = readdir(parentDir);
                    continue;
                }
            }
            //printf("%s =?= %s\n", pathToFile, codebook);
            if(strcmp(pathToFile, codebook) == 0)
            {
                currElement = readdir(parentDir);
                continue;
            }
            //printf("%s\n", pathToFile);
            //send the path to this file to the normal compress method, should create .hcz file on its own
            compress(pathToFile, codebook);
            //printf("|compressed successfully|\n");
        }
        //else if directory
        else if(currElement->d_type == DT_DIR)
        {
            //printf("DIRECTORY!\n");
            strcpy(pathToFile, path);
            strcat(pathToFile, "/");
            strcat(pathToFile, currElement->d_name);
            DIR* subDir = opendir(pathToFile);
            //printf("%s\n", pathToFile);
            compressEachFile(subDir, pathToFile, codebook);
            closedir(subDir);
        }

        currElement = readdir(parentDir);        
    }
}

void recursiveCompress(char* path, char* codebook)
{
    //Determine of path is valid directory
    DIR* dir = opendir(path);
    if(dir) 
    {

    }
    else if(errno == ENOENT)
    {
        printf("Error: Directory \"%s\" does not exist.\n", path);
        closedir(dir);
        return;
    }
    else
    {
        printf("Error: Directory \"%s\" failed to open.\n", path);
    }

	int huffmanFD = open(codebook, O_RDONLY);
    if(huffmanFD == -1) { //if file does not exist
	    int errsv = errno;
		printf("Error: HuffmanCodebook file does not exist\n");
		close(huffmanFD);
		return;
	}
    
    //CHECK IF FILE IS EMPTY
    char c;
    int bytesRead = read(huffmanFD, &c, 1);
    if(bytesRead == 0)
    {
        printf("Warning: HuffmanCodebook file is empty\n");
        close(huffmanFD);
        return;
    }
    close(huffmanFD);

    //(If we want .hcz for each file) Call recursiveCompressHelper, which will iterate through each file in this dir and all subdirs and compress them
    compressEachFile(dir, path, codebook);

}

void decompress(char* path, char* codebook)
{
    //path: path to .hcz file
    //codebook: path to HuffmanCodebook

    //Read the HuffmanCodebook file
    HuffmanCodesTree* reverseHuffmanCodesTree = readHuffmanCodebook(codebook);

    if(reverseHuffmanCodesTree == NULL)
    {
        printf("There was en error in opening the codebook\n");
        return;
    }
    //printHuffmanCodesTree(reverseHuffmanCodesTree);

    // get reversedHuffmanCodesTree
    reverseHuffmanCodesTree = getReverseHuffmanCodesTree(reverseHuffmanCodesTree);
    //NOTE: root->token is now the bitSequence
    //      root->bitSequence is now the token
    //printHuffmanCodesTree(reverseHuffmanCodesTree);

    //printHuffmanCodesTree(reverseHuffmanCodesTree);
    //Now we have a reverseHuffmanCodesTree

    //open two files: path (to read from, a .hcz file), new file to write into without .hcz extension
    int fdRead = open(path, O_RDONLY);
    if(fdRead == -1) 
    { //if file does not exist
	    int errsv = errno;
		printf("Fatal Error: file \"");
		printf("%s\" ", path);
		printf("does not exist\n");
        freeHuffmanCodesTree(reverseHuffmanCodesTree);
		close(fdRead);
		return;
	}
    
    //CHECK IF FILE IS EMPTY
    char c;
    int bytesRead = read(fdRead, &c, 1);
    if(bytesRead == 0)
    {
        printf("Warning: File is empty!\n");
        freeHuffmanCodesTree(reverseHuffmanCodesTree);
        close(fdRead);
        return;
    }
    close(fdRead);
    //open file.hcz that was sent in as a path to read from
    fdRead = open(path, O_RDONLY);

    //create file without "testdir/file1.txt.hcz" to prepare for writing
    char decompressFilePath[1000];
    memset(decompressFilePath, '\0', 1000 * sizeof(char));
    strcpy(decompressFilePath, path);
    int originalLen = strlen(decompressFilePath);
    int i;
    for(i = strlen(decompressFilePath) - 1; i >= originalLen-4; i --) {
        decompressFilePath[i] = '\0';
    }

    remove(decompressFilePath);
    int fdWrite = open(decompressFilePath, O_WRONLY | O_CREAT, 00600);

    //start to read from file
    char bitSequenceRead[200];
    memset(bitSequenceRead, '\0', 200 * sizeof(char));
    int bitSequenceLength = 0;
	do {

		c = 0;
		bytesRead = read(fdRead, &c, sizeof(char));
        //printf("%d\n", bytesRead);
		//CHECKS
		if(bytesRead == 0 || c == '\0')
        {
            //this means you reached the end of .hcz
            break;
        }
		else 
        {
            //First Step: Save the read-in bed to the buffer
            bitSequenceRead[bitSequenceLength] = c;
            bitSequenceLength ++;

            //Second Step: Call findBitSequence() to see if it exists in the look-up tree
            char* tokenToWrite = findBitSequence(reverseHuffmanCodesTree, bitSequenceRead);
            //If it is not NULL, that means bitSequence is in look-up tree, so write the token to the file
            if(tokenToWrite != NULL)
            {
                if(strcmp(tokenToWrite,"!s") == 0)
                {
                    write(fdWrite, " ", 1 * sizeof(char));
                }
                else if(strcmp(tokenToWrite,"!t") == 0)
                {
                    write(fdWrite, "\t", 1 * sizeof(char));
                }
                else if(strcmp(tokenToWrite,"!n") == 0)
                {
                    write(fdWrite, "\n", 1 * sizeof(char));
                } 
                else
                {
                    write(fdWrite, tokenToWrite, strlen(tokenToWrite) * sizeof(char));
                }
                memset(bitSequenceRead, '\0', strlen(bitSequenceRead) * sizeof(char));
                bitSequenceLength = 0;
            }
		}

	} while(bytesRead > 0);

    freeHuffmanCodesTree(reverseHuffmanCodesTree);
    close(fdRead);
    close(fdWrite);

}


void decompressEachFile(DIR* parentDir, char* path, char* codebook)
{
    struct dirent* currElement = NULL;
    readdir(parentDir);
    readdir(parentDir);
    currElement = readdir(parentDir);

    while(currElement != NULL)
    {
        char pathToFile[1000];
        //if is file
        if(currElement->d_type == DT_REG)
        {
            //do check for the extension
            char* extensionPtr = strrchr(currElement->d_name, '.');
            if(extensionPtr != NULL) 
            {
                //ONLY CALL DECOMPRESS ON THIS FILE WHEN IT HAS EXTENSION: .hcz
                if(strcmp(extensionPtr, ".hcz") == 0)
                {
                    strcpy(pathToFile, path);
                    strcat(pathToFile, "/");
                    strcat(pathToFile, currElement->d_name);
                    decompress(pathToFile, codebook);
                }
            }
            
        }
        //else if is directory
        else if(currElement->d_type == DT_DIR)
        {
            //printf("DIRECTORY!\n");
            strcpy(pathToFile, path);
            strcat(pathToFile, "/");
            strcat(pathToFile, currElement->d_name);
            DIR* subDir = opendir(pathToFile);
            //printf("%s\n", pathToFile);
            decompressEachFile(subDir, pathToFile, codebook);
            closedir(subDir);
        }

        currElement = readdir(parentDir);    

    }
} 

void recursiveDecompress(char* path, char* codebook){
    //Determine of path is valid directory
    DIR* dir = opendir(path);
    if(dir) 
    {

    }
    else if(errno == ENOENT)
    {
        printf("dir does not exist\n");
        closedir(dir);
        return;
    }
    else
    {
        printf("dir failed to open due to an unknown reason\n");
    }

	int huffmanFD = open(codebook, O_RDONLY);
    if(huffmanFD == -1) { //if file does not exist
	    int errsv = errno;
		printf("Error: HuffmanCodebook file does not exist\n");
		close(huffmanFD);
		return;
	}
    
    //CHECK IF FILE IS EMPTY
    char c;
    int bytesRead = read(huffmanFD, &c, 1);
    if(bytesRead == 0)
    {
        printf("Warning: HuffmanCodebook file is empty\n");
        close(huffmanFD);
        return;
    }
    close(huffmanFD);

    decompressEachFile(dir, path, codebook);

}

int main(int argc, char* argv[]) {

    if (argc < 3)
	{
		printf("Need more/ less  of arguments.\n Here is what the input should be formatted as: ./fileCompressor <flag> <path or file> |codebook|\n");
		
		EXIT_FAILURE;
	}

    char recursiveFlag = 0;
	char compressFlag = 0;
	char decompressFlag = 0;	
	char buildFlag = 0;

    //check if too few args
    if(argc < 3)
    {
        printf("Error: Too few arguments.\n");
        return 0;
    }
    else if(argc > 5)
    {
        printf("Error: Too many arguments.\n");
        return 0;
    }
    //if 3 args, then it can only be buildCodebook
    else if(argc == 3)
    {
        if(strcmp(argv[1], "-b") == 0)
        {
            buildFlag = 1;
        }
        else if(strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-R") == 0)
        {
            printf("Error: Expected more arguments.\n");
            return 0;
        }
        else //otherwise, it's the incorrect flag
        {
            printf("Error: Incorrect flag (second arg should be -R, -b, -c, or -d).\n");
            return 0;
        }
    }
    //else if 4 args, then it can be:
        //./fileCompressor -c <fileName or pathToFile> <HuffmanCodebook path>
        //./fileCompressor -d <fileName or pathToFile> <HuffmanCodebook path>
        //./fileCompressor -R -b <fileName or pathToFile>
    else if(argc == 4)
    {
        if(strcmp(argv[1], "-c") == 0)
        {
            compressFlag = 1;
        }
        else if(strcmp(argv[1], "-d") == 0)
        {
            decompressFlag = 1;
        }
        else if(strcmp(argv[1], "-R") == 0)
        {
            recursiveFlag = 1;
            if(strcmp(argv[2], "-b") == 0)
            {
                buildFlag = 1;
            }
            else if(strcmp(argv[2], "-c") == 0 || strcmp(argv[2], "-d") == 0)
            {
                printf("Error: Expected more arguments.\n");
                return 0;
            }
            else
            {
                printf("Error: Incorrect flag (third arg can only be -b if only four arguments).\n");
                return 0;
            }
        }
        else
        {
            printf("Error: Incorrect flag (second arg should be -R, -b, -c, or -d).\n");
            return 0;   
        }
    }
    //else if 5 args, then it can be:
        //./fileCompressor -R -c <fileName or pathToFile> <HuffmanCodebook path>
        //./fileCompressor -R -d <fileName or pathToFile> <HuffmanCodebook path>
    else
    {
        if(strcmp(argv[1], "-R") == 0)
        {
            recursiveFlag = 1;
            if(strcmp(argv[2], "-c") == 0)
            {
                compressFlag = 1;
            }
            else if(strcmp(argv[2], "-d") == 0)
            {
                decompressFlag = 1;
            }
            else
            {
                printf("Error: Incorrect flag (third arg should be -b, -c, or -d).\n");
                return 0;
            }
        }
        else
        {
            printf("Error: Incorrect flag (second arg should be -R, -b, -c, or -d).\n");
            return 0;
        }
    }

/*
    int i;
    for (i = 0; i < argc; i++)
    {
    //comparing if that parameter is -R particular flag and then setting the recursive to 1 
      if (strcmp("-R", argv[i]) == 0)
	  {
            recursiveFlag = 1;
	  }
    //comparing if that parameter is -b particular flag and then setting the compress to 1
        if (strcmp("-b", argv[i]) == 0)
	  {
            buildFlag = 1;
	  }
	//comparing if that parameter is -d particular flag and then setting the decompress to 1
	  if (strcmp("-d", argv[i]) == 0)
	  {
            decompressFlag = 1;
	  }
	//comparing if that parameter is -c particular flag and then setting the buildCodebook to 1
	  if (strcmp("-c", argv[i]) == 0)
	  {
            compressFlag = 1;
	  }
    }
*/

    if(recursiveFlag == 0 && buildFlag == 1) 
    {
        if(argv[2][0] == '.' && argv[2][1] == '/')
        {
            buildCodebook(argv[2]);
        }
        else {
            char pathToFile[1000];
            memset(pathToFile, '\0', 1000);
            pathToFile[0] = '.';
            pathToFile[1] = '/';
            strcat(pathToFile, argv[2]);
            //printf("%s\n", pathToFile);
            buildCodebook(pathToFile);
            /*
            char* pathToFile = (char*)(malloc(sizeof(argv[2]) + (2 * sizeof(char))));
            memset(pathToFile, '\0', 1000);
            pathToFile[0] = '.';
            pathToFile[1] = '/';
            strcat(pathToFile + 2, argv[2]);
            //printf("%s\n", pathToFile);
            buildCodebook(pathToFile);
            //free(pathToFile);
            */
        }
    }

    else if(recursiveFlag == 1 && buildFlag == 1)
    {
        char lastChar = argv[3][strlen(argv[3]) - 1];
        if(lastChar == '/')
        {
            argv[3][strlen(argv[3]) - 1] = '\0';
        }
        //printf("%c\n", argv[3][strlen(argv[3]) - 1]);
        recursiveBuildCodebook(argv[3]);
    }
    
    else if(recursiveFlag == 0 && compressFlag == 1)
    {
        char pathToFile[1000];
        char pathToCodebook[1000];
        memset(pathToFile, '\0', 1000);
        memset(pathToCodebook, '\0', 1000);
        if(!(argv[2][0] == '.' && argv[2][1] == '/'))
        {
            pathToFile[0] = '.';
            pathToFile[1] = '/';
            strcat(pathToFile, argv[2]);
        }
        else {
            strcpy(pathToFile, argv[2]);
        }
        if(!(argv[3][0] == '.' && argv[3][1] == '/'))
        {
            pathToCodebook[0] = '.';
            pathToCodebook[1] = '/';
            strcat(pathToCodebook, argv[3]);
        }
        else
        {
            strcpy(pathToCodebook, argv[3]);
        }
        compress(pathToFile, pathToCodebook);
    }

    else if(recursiveFlag == 1 && compressFlag == 1)
    {
        char lastChar = argv[3][strlen(argv[3]) - 1];
        if(lastChar == '/')
        {
            argv[3][strlen(argv[3]) - 1] = '\0';
        }
        char pathToFile[1000];
        char pathToCodebook[1000];
        memset(pathToFile, '\0', 1000);
        memset(pathToCodebook, '\0', 1000);
        if(!(argv[3][0] == '.' && argv[3][1] == '/'))
        {
            pathToFile[0] = '.';
            pathToFile[1] = '/';
            strcat(pathToFile, argv[3]);
        }
        else {
            strcpy(pathToFile, argv[3]);
        }
        if(!(argv[4][0] == '.' && argv[4][1] == '/'))
        {
            pathToCodebook[0] = '.';
            pathToCodebook[1] = '/';
            strcat(pathToCodebook, argv[4]);
        }
        else
        {
            strcpy(pathToCodebook, argv[4]);
        }
        recursiveCompress(pathToFile, pathToCodebook);
    }

    else if(recursiveFlag == 0 && decompressFlag == 1)
    {
        char pathToFile[1000];
        char pathToCodebook[1000];
        memset(pathToFile, '\0', 1000);
        memset(pathToCodebook, '\0', 1000);
        if(!(argv[2][0] == '.' && argv[2][1] == '/'))
        {
            pathToFile[0] = '.';
            pathToFile[1] = '/';
            strcat(pathToFile, argv[2]);
        }
        else {
            strcpy(pathToFile, argv[2]);
        }
        if(!(argv[3][0] == '.' && argv[3][1] == '/'))
        {
            pathToCodebook[0] = '.';
            pathToCodebook[1] = '/';
            strcat(pathToCodebook, argv[3]);
        }
        else
        {
            strcpy(pathToCodebook, argv[3]);
        }
        decompress(pathToFile, pathToCodebook);        
    }

    else if(recursiveFlag == 1 && decompressFlag == 1)
    {
        char lastChar = argv[3][strlen(argv[3]) - 1];
        if(lastChar == '/')
        {
            argv[3][strlen(argv[3]) - 1] = '\0';
        }
        char pathToFile[1000];
        char pathToCodebook[1000];
        memset(pathToFile, '\0', 1000);
        memset(pathToCodebook, '\0', 1000);
        if(!(argv[3][0] == '.' && argv[3][1] == '/'))
        {
            pathToFile[0] = '.';
            pathToFile[1] = '/';
            strcat(pathToFile, argv[3]);
        }
        else {
            strcpy(pathToFile, argv[3]);
        }
        if(!(argv[4][0] == '.' && argv[4][1] == '/'))
        {
            pathToCodebook[0] = '.';
            pathToCodebook[1] = '/';
            strcat(pathToCodebook, argv[4]);
        }
        else
        {
            strcpy(pathToCodebook, argv[4]);
        }
        recursiveDecompress(pathToFile, pathToCodebook);
    }
    return 0;

}
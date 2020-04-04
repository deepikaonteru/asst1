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
		printf("Fatal Error: file \"");
		printf("%s\" ", path);
		printf("does not exist\n");
		close(fd);
		return;
	}
    
    //CHECK IF FILE IS EMPTY
    char c;
    int bytesRead = read(fd, &c, 1);
    if(bytesRead == 0)
    {
        printf("Warning: File is empty!\n");
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
    remove("HuffmanCodebook");
    fd = open("HuffmanCodebook", O_WRONLY | O_CREAT, 00600);
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
    remove("HuffmanCodebook");
    int fd = open("HuffmanCodebook", O_WRONLY | O_CREAT, 00600);
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

    if(recursiveFlag == 0 && buildFlag == 1) 
    {
        buildCodebook(argv[2]);
    }

    else if(recursiveFlag == 1 && buildFlag == 1)
    {
        recursiveBuildCodebook(argv[3]);
    }
    
    else if(recursiveFlag == 0 && compressFlag == 1)
    {
        //OPTION I
        //Read through HuffmanCodebook file and populate a CodebookNode array

        //OPTION II
        //Read through HuffmanCodebook file and populate a CodebookNode BST

        //After either option, read through file. For each token we see, concatenate the
        //corresponding bit sequence to a .hcz file
        //2 file descriptors running simultaneously?
    }

    // Pointer for directory entry 
    /*
    DIR *pDir;

    pDir = opendir (argv[1]);
    // opendir returns NULL if couldn't open directory 
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", argv[1]);
        return 1;
    }

    printAll(pDir);

    closedir (pDir);
    */

    //printAll(argv[1]);

    return 0;


    /*
COMMAND LINE INTERFACE
 ./fileCompressor -R -b ./ 
 recursively index all files found inthe current working directory, all subdirectories and emit a single ./HuffmanCodebook for them

 ./fileCompressor -d ./dir/test/file.txt.hcz ./HuffmanCodebook
decompress ./dir/test/file.txt.hcz in to ./dir/test/file.txt using the codebook ./HuffmanCodebook

*/
}
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
                printf("[%s]\n", entry->d_name);
                strcpy(pathToNext, path);
                strcat(pathToNext, "/");
                strcat(pathToNext, entry->d_name);
                printAll(pathToNext);
            }
        }

        closedir(dir);

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

void buildCodebook(char* path)
{    
    //open file
    int fd = open(path, O_RDONLY);
    if(fd == -1) { //if file does not exist
	    int errsv = errno;
		printf("Fatal Error: file \"");
		printf("%s\" ", path);
		printf("does not exist\n");
		close(fd);
		return;
	}
     //read and tokenize file using buildHelper in build.c (and refresh)
        //this gives us FreqTree
    //convert to MinHeap
    //free FreqTree
    //Build HuffmanTree
    //Use inorder traversal on HuffmanTree to make HuffmanCodebook file
    //free MinHeap/HuffmanTree

    FreqTree* tree = readAndBuildTree(fd);
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
    else if(recursiveFlag == 1 && buildFlag == 1) {

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
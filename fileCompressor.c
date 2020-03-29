#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "minHeap.h"
#include "freqDict.h"
 
void printAll(char*);
void buildCodebook(char*);

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

//printf("[%s]\n", pDirent->d_name);
//opendir(pDirent); //can we open this as a directory?
//if(no) { continue; }
//if(yes) { ...(); }
/*
void printAll(DIR* pDir) {

    struct dirent *pDirent;

    while ((pDirent = readdir(pDir)) != NULL) {
        
        char* dName = pDirent->d_name;
        if(dName[0] == '.') {
            continue;
        }
        printf("[%s]\n", dName);

        DIR* subDir = opendir(dName);
        //int err = errno;
        //does this return NULL? or does this run?
        if(subDir == NULL) {
            continue;
        } 
        else {
            printAll(subDir);
        }
    }

}
*/

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
    //else printf("yer");

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

    printAll(argv[1]);

    return 0;


    /*
COMMAND LINE INTERFACE
 ./fileCompressor -R -b ./ 
 recursively index all files found inthe current working directory, all subdirectories and emit a single ./HuffmanCodebook for them

 ./fileCompressor -d ./dir/test/file.txt.hcz ./HuffmanCodebook
decompress ./dir/test/file.txt.hcz in to ./dir/test/file.txt using the codebook ./HuffmanCodebook

*/
}
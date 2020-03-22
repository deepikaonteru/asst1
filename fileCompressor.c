#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
 


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

int main(int argc, char* argv[]) {

    if (argc < 3)
	{
		printf("Need more/ less  of arguments.\n Here is what the input should be formatted as: ./fileCompressor <flag> <path or file> |codebook|\n");
		
		EXIT_FAILURE;
	}

    char recursive = 0;
	char compress = 0;
	char decompress = 0;	
	char buildCodebook = 0;
    
	int i;
    for (i = 0; i < argc; i++)
    {
    //comparing if that parameter is -R particular flag and then setting the recursive to 1 
      if (strcmp("-R", argv[i]) == 0)
	  {
            recursive = 1;
	  }
    //comparing if that parameter is -b particular flag and then setting the compress to 1
        if (strcmp("-b", argv[i]) == 0)
	  {
            compress = 1;
	  }
	//comparing if that parameter is -d particular flag and then setting the decompress to 1
	  if (strcmp("-d", argv[i]) == 0)
	  {
            decompress = 1;
	  }
	//comparing if that parameter is -c particular flag and then setting the buildCodebook to 1
	  if (strcmp("-c", argv[i]) == 0)
	  {
            buildCodebook = 1;
	  }
    }

    // Pointer for directory entry 
    struct dirent *pDirent;
    DIR *pDir;

    if (argc < 2) {
        printf ("Usage: testprog <dirname>\n");
        return 1;
    }
    pDir = opendir (argv[1]);
    // opendir returns NULL if couldn't open directory 
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", argv[1]);
        return 1;
    }

    while ((pDirent = readdir(pDir)) != NULL) {
        printf ("[%s]\n", pDirent->d_name);
    }
    closedir (pDir);
    return 0;


    /*
COMMAND LINE INTERFACE
 ./fileCompressor -R -b ./ 
 recursively index all files found inthe current working directory, all subdirectories and emit a single ./HuffmanCodebook for them

 ./fileCompressor -d ./dir/test/file.txt.hcz ./HuffmanCodebook
decompress ./dir/test/file.txt.hcz in to ./dir/test/file.txt using the codebook ./HuffmanCodebook

*/
}
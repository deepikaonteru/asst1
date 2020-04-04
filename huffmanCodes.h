#ifndef HUFFMAN_CODES_H
#define HUFFMAN_CODES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stddef.h>
#include <math.h>

typedef struct HuffmanCodesTNode {
    char* token;
	char* bitSequence;
    struct HuffmanCodesTNode* left;
    struct HuffmanCodesTNode* right;
} HuffmanCodesTNode;

typedef struct HuffmanCodesTree {
    HuffmanCodesTNode* root;
    int size;
} HuffmanCodesTree;

HuffmanCodesTree *createHuffmanCodesTree();
HuffmanCodesTNode *createHuffmanCodesTNode(char *token, char *bitSequence);
HuffmanCodesTNode *insertHuffmanCodesTNode(HuffmanCodesTNode *root, char *token, char *bitSequence, int *treeCount) ;
HuffmanCodesTree* insertIntoHuffmanCodesTree(HuffmanCodesTree *root, char *token, char *bitSequence);
HuffmanCodesTree *readHuffmanCodebook(char*);
char* findToken(HuffmanCodesTree *HuffmanCodesTree, char *token, char *bitSequence);

#endif
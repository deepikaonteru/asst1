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
void inverseKeyAndValue(HuffmanCodesTree *result, HuffmanCodesTNode *treeToReverse);
HuffmanCodesTree *getReverseHuffmanCodesTree(HuffmanCodesTree *treeToReverse);
char* findBitSequence(HuffmanCodesTree *HuffmanCodesTree, char *token);
char* findToken(HuffmanCodesTree *reverseHuffmanCodesTree, char *bitSequence);
void printHuffmanCodesTNode(HuffmanCodesTNode *root);
void printHuffmanCodesTree(HuffmanCodesTree *tree);
void freeHuffmanCodesTNode(HuffmanCodesTNode *root);
void freeHuffmanCodesTree(HuffmanCodesTree *tree);

#endif
#ifndef FREQDICT_H
#define FREQDICT_H

#include "minHeap.h"

typedef struct FreqTNode 
{
    int freq;
    char* token;
    struct FreqTNode* left;
    struct FreqTNode* right;
} FreqTNode;

typedef struct FreqTree 
{
    FreqTNode* root;
    int size;
} FreqTree;

FreqTree *createFreqTree();
FreqTNode *createFreqTNode(char *token);
FreqTNode *insertTNode(FreqTNode *root, char *token, int *treeCount);
void insertIntoFreqTree(FreqTree *root, char *token);
void insertLikeMinHeap(MinHeap* heap, FreqTNode *node);
MinHeap* convertFreqTreeToMinHeap(FreqTree *tree);
void printFreqTNode(FreqTNode *root) ;
void printFreqTree(FreqTree *tree);
void freeFreqTNode(FreqTNode *root);
void freeFreqTree(FreqTree *tree);

#endif
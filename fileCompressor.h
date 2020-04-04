#ifndef FILE_COMPRESSOR_H
#define FILE_COMPRESSOR_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "freqDict.h"
#include "minHeap.h"
#include "huffmanCodes.h"

typedef struct CodebookNode {
    char* token;
    char* bitSequence;
} CodebookNode;

void printAll(char*);
void refresh(char*, int);
void compress(char*, char*);
void recursiveBuildCodebook(char*);
void buildMasterTree(DIR*, FreqTree*, char*);
void recReadAndBuildTree(int, FreqTree*);
void buildCodebook(char*);
void freeCodebookNodes(CodebookNode*);
void getTokens(TreeNode*, CodebookNode*);
void getBitSequences(TreeNode*, CodebookNode*, char*, int);
void freeCodebookNodes(CodebookNode*);
int findNumLeafNodes(TreeNode*);
int findHeight(TreeNode*);
int fileExists(char*);
FreqTree* readAndBuildTree(int);

#endif
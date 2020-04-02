#ifndef FILE_COMPRESSOR_H
#define FILE_COMPRESSOR_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "freqDict.h"
#include "minHeap.h"
#include "huffmanCodebook.h"

typedef struct CodebookNode {
    char* token;
    char* bitSequence;
} CodebookNode;

void printAll(char*);
void refresh(char*, int);
void buildCodebook(char*);
FreqTree* readAndBuildTree(int);

#endif
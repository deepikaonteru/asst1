#ifndef FILE_COMPRESSOR_H
#define FILE_COMPRESSOR_H


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


#endif
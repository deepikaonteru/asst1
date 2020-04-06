#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffmanCodes.h"

HuffmanCodesTree *createHuffmanCodesTree() {
	HuffmanCodesTree *tree = (HuffmanCodesTree *) malloc(sizeof(HuffmanCodesTree));
	tree->root = NULL;
	tree->size = 0;
	return tree;
}

HuffmanCodesTNode *createHuffmanCodesTNode(char *token, char *bitSequence) {
	HuffmanCodesTNode *node = (HuffmanCodesTNode *) malloc(sizeof(HuffmanCodesTNode));
	node->token = strdup(token);
	node->bitSequence = strdup(bitSequence);
	node->left = NULL;
	node->right = NULL;
	return node;
}

HuffmanCodesTNode *insertHuffmanCodesTNode(HuffmanCodesTNode *root, char *token, char *bitSequence, int *treeCount) {
	if(root == NULL) {
		root = createHuffmanCodesTNode(token, bitSequence);
		*treeCount += 1;
	} else {
	    if(strcmp(root->token, token) < 0) {
			root->right = insertHuffmanCodesTNode(root->right, token, bitSequence, treeCount);
        // if root->token is greater than token, insert token on root's right
		} else {
			root->left = insertHuffmanCodesTNode(root->left, token, bitSequence, treeCount);
		}
	}
	return root;
}

HuffmanCodesTree* insertIntoHuffmanCodesTree(HuffmanCodesTree *root, char *token, char *bitSequence) {
	root->root = insertHuffmanCodesTNode(root->root, token, bitSequence, &(root->size));
	return root;
}

HuffmanCodesTree *readHuffmanCodebook(char* path) {

	int rd = open(path, O_RDONLY);
    if(rd == -1) { //if file does not exist
	    int errsv = errno;
		printf("Error: HuffmanCodebook file does not exist.\n");
		close(rd);
		return NULL;
	}
    
    //CHECK IF FILE IS EMPTY
    char c;
    int bytesRead = read(rd, &c, 1);
    if(bytesRead == 0)
    {
        printf("Error: HuffmanCodebook file is empty.\n");
        close(rd);
        return NULL;
    }
    close(rd);
    rd = open(path, O_RDONLY);
	
	HuffmanCodesTree *HuffmanCodesTree = createHuffmanCodesTree();
	char token[200], bitSequence[200];
	int bitSequenceLength = 0;
	int tokenLength = 0;
	//flags
	int bitSequenceStarted = 1;
	int tokenStarted = 0;

	int i;
	for(i=0; i<200; i ++) {
		token[i]='\0';
	}

	for(i=0; i<200; i ++) {
		bitSequence[i]='\0';
	}

	read(rd, &c, 1);
	read(rd, &c, 1);
    while (read(rd, &c, 1) != 0) {
		// format of this file is <bitSequence> <tab> <token>
		// if line contains a new line at last, remove it.
		if((c == '\n') && tokenStarted) {
			// end of a line and has just read token so set

			// insert the token & code into HuffmanCodesTree
			HuffmanCodesTree = insertIntoHuffmanCodesTree(HuffmanCodesTree, token, bitSequence);

			//reset flags
			tokenStarted = 0;
			bitSequenceStarted = 1;

			memset(bitSequence, '\0', bitSequenceLength * sizeof(char));
			memset(token, '\0', tokenLength * sizeof(char));

			bitSequenceLength = 0;
			tokenLength = 0;
		} else if(c == '\t' && bitSequenceStarted) {
			// token starts now
			tokenStarted = 1;
			bitSequenceStarted = 0;
		} else if(bitSequenceStarted) {
			bitSequence[bitSequenceLength] = c;
			bitSequenceLength ++;
		} else if(tokenStarted) {
			token[tokenLength] = c;
			tokenLength ++;
		}		
	}
	close(rd);
	return HuffmanCodesTree;

}

void inverseKeyAndValue(HuffmanCodesTree *result, HuffmanCodesTNode *treeToReverse) {
	
	if(treeToReverse != NULL) {
		inverseKeyAndValue(result, treeToReverse->left);
		
		// insertIntoHuffmanCodesTree,
		// interchange key and value
		insertIntoHuffmanCodesTree(result, treeToReverse->bitSequence, treeToReverse->token);
		
		inverseKeyAndValue(result, treeToReverse->right);		
	}
}

HuffmanCodesTree *getReverseHuffmanCodesTree(HuffmanCodesTree *treeToReverse) {
	HuffmanCodesTree *result = createHuffmanCodesTree();	
	inverseKeyAndValue(result, treeToReverse->root);
	return result;
}

//if compress, so return start->bitSequence
//if decompress, so return start->bitSequence (which is token)
char* findBitSequence(HuffmanCodesTree *HuffmanCodesTree, char *token) {
	HuffmanCodesTNode *start = HuffmanCodesTree->root;
	
	while(start != NULL) {
		// if current node has same key
		if(strcmp(start->token, token) == 0) {
			return start->bitSequence;	
		} else if(strcmp(start->token, token) < 0) {
			start = start->right;
		} else {
			start = start->left;
		}
	}
	return NULL;
}

void printHuffmanCodesTNode(HuffmanCodesTNode *root) {
	if(root != NULL) {
		printHuffmanCodesTNode(root->left);
		printf("%s : %s\n", root->bitSequence, root->token);
		printHuffmanCodesTNode(root->right);
	}	
}


void printHuffmanCodesTree(HuffmanCodesTree *tree) {
	printHuffmanCodesTNode(tree->root);
}

void freeHuffmanCodesTNode(HuffmanCodesTNode *root) {
	if(root != NULL) {
		freeFreqTNode(root->left);
		free(root->token);
		free(root->bitSequence);
		free(root);
		freeFreqTNode(root->right);
	}	
}

void freeHuffmanCodesTree(HuffmanCodesTree *tree) {
	freeFreqTNode(tree->root);
	free(tree);
}
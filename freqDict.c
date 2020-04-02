#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freqDict.h"

FreqTree *createFreqTree() {
	FreqTree *tree = (FreqTree *) malloc(sizeof(FreqTree));
	tree->root = NULL;
	tree->size = 0;
	return tree;
}

FreqTNode *createFreqTNode(char *token) {
	FreqTNode *node = (FreqTNode *) malloc(sizeof(FreqTNode));
	node->token = strdup(token);
	node->freq = 1;
	node->left = NULL;
	node->right = NULL;
	return node;
}

FreqTNode *insertTNode(FreqTNode *root, char *token, int *treeCount) {
	if(root == NULL) {
		root = createFreqTNode(token);
		*treeCount += 1;
	} else {
        // if in tree, update frequency
		if(strcmp(root->token, token) == 0) {
			root->freq += 1;
        // if root->token is less than token, insert token on root's right
		} else if(strcmp(root->token, token) < 0) {
			root->right = insertTNode(root->right, token, treeCount);
        // if root->token is greater than token, insert token on root's right
		} else {
			root->left = insertTNode(root->left, token, treeCount);
		}
	}
	return root;
}

FreqTree* insertIntoFreqTree(FreqTree *root, char *token) {
	root->root = insertTNode(root->root, token, &(root->size));
	return root;
}

// in FreqTree, the token is compared lexicograpically, while MinHeap comparison is based on the freq (count) of token in file
void insertLikeMinHeap(MinHeap* heap, FreqTNode *node) {
	if(node != NULL) {
		
		insertLikeMinHeap(heap, node->left);
		
		HeapNode *temp = (HeapNode *) malloc(sizeof(HeapNode));
		temp->count = node->freq;
		temp->data = createTreeNode(node->token, NULL, NULL);
		insertNode(heap, temp);

		insertLikeMinHeap(heap, node->right);
	}
}

MinHeap* convertFreqTreeToMinHeap(FreqTree *tree) {
	
	MinHeap* heap = buildMinHeap(tree->size);
	
	insertLikeMinHeap(heap, tree->root);
	
	return heap;
}


void printFreqTNode(FreqTNode *root) {
	if(root != NULL) {
		printFreqTNode(root->left);
		printf("%s : %d\n", root->token, root->freq);
		printFreqTNode(root->right);
		/*
		freeFreqTNode(root->left);
		freeFreqTNode(root->right);
		free(root->token);
		free(root);
		*/
	}	
}

void printFreqTree(FreqTree *tree) {
	printFreqTNode(tree->root);
}

void freeFreqTNode(FreqTNode *root) {
	if(root != NULL) {
		freeFreqTNode(root->left);
		free(root->token);
		free(root);
		freeFreqTNode(root->right);
	}	
}

void freeFreqTree(FreqTree *tree) {
	freeFreqTNode(tree->root);
	free(tree);
}
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
        // if root is less than token, insert on root's right
		} else if(strcmp(root->token, token) < 0) {
			root->right = insertTNode(root->right, token, treeCount);
        // if root is greater than token, insert on root's right
		} else {
			root->left = insertTNode(root->left, token, treeCount);
		}
	}
	return root;
}

// in FreqTree, the token is compared lexicograpically, while MinHeap comparison is based on the freq (count) of token in file
void insertLikeMinHeap(MinHeap* heap, FreqTNode *node) {
	if(node != NULL) {
		
		insertLikeMinHeap(heap, node->left);
		
		HeapNode *temp = (HeapNode *) malloc(sizeof(HeapNode));
		temp->count = node->freq;
		temp->data = createTreeNode(node->token, NULL, NULL);
		insertLikeMinHeap(heap, temp);

		insertLikeMinHeap(heap, node->right);
	}
}

MinHeap* convertFreqTreeToMinHeap(FreqTree *tree) {
	MinHeap* heap = buildHuffmanTree(tree);
	
	insertLikeMinHeap(heap, tree->root);
	
	return heap;
}

void printFreqTNode(FreqTNode *root) {
	if(root != NULL) {
		printFreqTNode(root->left);
		printf("%s: %d\n", root->token, root->freq);
		printFreqTNode(root->right);
	}
}

void printFreqTree(FreqTree *tree) {
	printFreqTNode(tree->root);
}

// Freeing tree and nodes
void printFreqTNode(FreqTNode *root) {
	if(root != NULL) {
		freeFreqTNode(root->left);
		freeFreqTNode(root->right);
		free(root->token);
		free(root);
	}	
}
void freeFreqTree(FreqTree *tree) {
	freeFreqTNode(tree->root);
	free(tree);
}

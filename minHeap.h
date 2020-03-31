#ifndef MINHEAP_H
#define MINHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>


typedef struct TreeNode {
	char *token;
	struct TreeNode *left, *right;
} TreeNode;

typedef struct HeapNode {
	int count;
	TreeNode *data;
} HeapNode;

typedef struct MinHeap {
	HeapNode **nodes;
	int heapSize;
	int heapCapacity;
} MinHeap;

MinHeap* buildMinHeap(int capacity);
void resizeHeap(MinHeap* heap);
void swap(MinHeap *heap, int i, int j);
void insertNode(MinHeap* heap, HeapNode *heapNode);
void heapify(MinHeap* heap, int i);
HeapNode* popMinNode(MinHeap* heap);
TreeNode* createTreeNode(char *token, TreeNode *leftNode, TreeNode *rightNode);
void freeTree(TreeNode *root);
void printTree(TreeNode *root);
TreeNode* buildHuffmanTree(MinHeap *heap);
int isSizeOne(struct MinHeap* minHeap); 

#endif
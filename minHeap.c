#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "minHeap.h"
#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2

MinHeap* buildMinHeap(int capacity) {
	//allocate space for given capacity and set size counter
	MinHeap* heap = (MinHeap*) malloc(sizeof(MinHeap));
	heap->nodes = (HeapNode**) malloc(capacity * sizeof(HeapNode*));
	heap->heapCapacity = capacity;
	heap->heapSize = 0;
	return heap;
}

void resizeHeap(MinHeap* heap) {
	int s = heap->heapSize;
	int i;
	
	HeapNode **temp = malloc((s + 10) * sizeof(HeapNode*));
	for(i=0; i<s; i++) {
		temp[i] = heap->nodes[i];
	}
	
	// free old array of nodes
	free(heap->nodes);
	
	// assign the temp nodes array back to old reference
	heap->nodes = temp;
	heap->heapCapacity = s + 10;
}

void swap(MinHeap *heap, int i, int j) { 
    HeapNode *temp = heap->nodes[i]; 
    heap->nodes[i] = heap->nodes[j];
    heap->nodes[j] = temp;
}

void insertNode(MinHeap* heap, HeapNode *heapNode) {
	if(heap->heapSize == heap->heapCapacity) {
		resizeHeap(heap);
	}
	
    int i = heap->heapSize;
	//create a node variable and initialize it with data that we need to store in the node.
    heap->nodes[i] = heapNode;
	heap->heapSize += 1;
  
	/* position node at right position; check its parent node, and compare it with new node. If its more, we shall move it to old position of last node. 
	   it will continue until we find a node which has value lower than the new node, and place the new node as a child node of that node. */
    while (i != 0 && heap->nodes[PARENT(i)]->count > heap->nodes[i]->count) { 
       swap(heap, i, PARENT(i));
       i = PARENT(i);
    }
}

/* given a node at index i, we shall compare all the three nodes (parent, left and right child), and find the smallest node among the three. 
	if smallest is not the parent node, then heap property is violated. 
	swap parent node with smallest node, and call heapify() function until heap property is satisfied. */

void heapify(MinHeap* heap, int i) {
	
    HeapNode *curr = heap->nodes[i]; 
    HeapNode *l = NULL; 
    HeapNode *r = NULL; 
	
    HeapNode *smallest = curr;
	
	if(LCHILD(i) < heap->heapSize) {
		l = heap->nodes[LCHILD(i)];
	}
	if(RCHILD(i) < heap->heapSize) {
		r = heap->nodes[RCHILD(i)];
	}
	
    if ((l != NULL) && (l->count < smallest->count)) { 
        smallest = l; 
	}
    if ((r != NULL) && (r->count < smallest->count)) { 
        smallest = r; 
	}
	
    if (smallest != curr) {
		int swapIndex = (smallest == l) ? LCHILD(i): RCHILD(i);
		swap(heap, i, swapIndex);
        heapify(heap, swapIndex); 
    }
}

HeapNode* popMinNode(MinHeap* heap) {
	
	HeapNode *result = NULL;
	
	// if heap is empty.
    if (heap->heapSize <= 0) {
        return result; 
	}
  
	// last node is placed at root node position, heap size decremented by 1, and memory of min heap reduced by one node, call heapify
	result = heap->nodes[0];
    heap->heapSize--;
    heap->nodes[0] = heap->nodes[heap->heapSize]; 
	heap->nodes[heap->heapSize] = NULL;
	if(heap->heapSize != 0) {
		heapify(heap, 0);
	}
  
    return result; 
}

TreeNode* createTreeNode(char *token, TreeNode *leftNode, TreeNode *rightNode) {
	TreeNode *temp = (TreeNode *) malloc(sizeof(TreeNode));
	temp->token = strdup(token);
	temp->left = leftNode;
	temp->right = rightNode;
	return temp;
}

void freeTree(TreeNode *root) {
	if(root != NULL) {
		freeTree(root->left);
		freeTree(root->right);
		free(root->token);
		free(root);
	}
}

void printTree(TreeNode *root) {
	if(root != NULL) {
		printTree(root->left);
		printf("%s ", root->token ? root->token : " ");
		printTree(root->right);
	}
}

int isSizeOne(struct MinHeap* minHeap) 
{ 
    return (minHeap->heapSize == 1); 
} 

/*
1. Create a leaf node for each unique character and build a min heap of all leaf nodes. 
   Initially, the least frequent character is at root.

2. Extract two nodes with the minimum frequency from the min heap.

3. Create a new internal node with a frequency equal to the sum of the two nodes frequencies. Make the first extracted node as its left child and the other extracted node as its right child. Add this node to the min heap.

4. Repeat steps#2 and #3 until the heap contains only one node. The remaining node is the root node and the tree is complete.
*/

TreeNode* buildHuffmanTree(MinHeap *heap) {

	if(isSizeOne(heap)){
		TreeNode *tempRoot = createTreeNode("", heap->nodes[0]->data, NULL);
		heap->nodes[0]->data = tempRoot;
	}
	 while (!isSizeOne(heap)) { 
		HeapNode *min1 = popMinNode(heap);		
		HeapNode *min2 = popMinNode(heap);

		HeapNode *freqNode = (HeapNode *) malloc(sizeof(HeapNode));
		freqNode->count = min1->count + min2->count;
		freqNode->data = createTreeNode("", min1->data, min2->data);

		free(min1);
		free(min2);

		//add freqNode back into min heap
		insertNode(heap, freqNode);
	 }

	if(heap->heapSize > 0) {
		return heap->nodes[0]->data;
	} else {
		return NULL;
	}

}

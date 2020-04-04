#ifndef HUFFMAN_CODES_H
#define HUFFMAN_CODES_H
ypedef struct HuffmanCodesTNode {
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
HuffmanCodesTree *readHuffmanCodebook();
char* findToken(HuffmanCodesTree *HuffmanCodesTree, char *token, char *bitSequence);

#endif
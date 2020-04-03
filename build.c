typedef struct HuffmanCodesTNode {
    char* token;
	char* bitSequence;
    struct HuffmanCodesTNode* left;
    struct HuffmanCodesTNode* right;
} HuffmanCodesTNode;

typedef struct HuffmanCodesTree {
    HuffmanCodesTNode* root;
    int size;
} HuffmanCodesTree;

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
        // if in tree, update frequency
		/*if(strcmp(root->token, token) == 0) {
			root->freq += 1; */
        // if root->token is less than token, insert token on root's right
		} if(strcmp(root->token, token) < 0) {
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

HuffmanCodesTree *readHuffmanCodebook {

	int rd = open(fileName, O_RDONLY);
    if(rd < 0){
        printf("Error in reading file: %s\n", fileName);
        return;
    }
	
	HuffmanCodesTree *HuffmanCodesTree* = createHuffmanCodesTree();
	char token[200], bitSequence[200];
	int bitSequenceLength = 0;
	int tokenLength = 0;
	//flags
	int bitSequenceStarted = 1;
	int tokenStarted = 0;

	for(i=0; i<200; i ++) {
		token[i]='\0';
	}

	for(i=0; i<200; i ++) {
		bitSequence[i]='\0';
	}


	char c;
    while (read(rd, &c, 1) != 0) {
		// format of this file is <bitSequence> <tab> <token>
		// if line contains a new line at last, remove it.
		if((c == '\n') && tokenStarted) {
			// end of a line and has just read token so set
		
			/* THINK ABOUT LOGIC HERE */

			//whitespace checks
			buffer[0] = c;
            if(buffer[0] == ' ') {
                buffer[0] = '!';
                buffer[1] = 's';
            }
            else if(buffer[0] == '\t') {
                buffer[0] = '!';
                buffer[1] = 't';
            }
            else if(buffer[0] == '\n') {
                buffer[0] = '!';
                buffer[1] = 'n';
            }
            else if(buffer[0] == '\0') {
                refresh(buffer, 1);
                continue;
            }

			// insert the token & code into HuffmanCodesTree
			insertHuffmanCodesTNode(HuffmanCodesTree, token, bitSequence);

			tokenLength = 0;
			codeLength = 0;
			tokenStarted = 0;
			codeStarted = 1;
		} else if(c == '\t' && codeStarted) {
			// token starts now
			tokenStarted = 1;
			codeStarted = 0;
		} else if(codeStarted) {
				bitSequence[bitSequenceLength++] = c;
		} else if(tokenStarted) {
			token[tokenLength++] = c;
		}		
	}
	close(rd);
	return HuffmanCodesTree;

}


char* findToken(HuffmanCodesTree *HuffmanCodesTree, char *token, char *bitSequence) {
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


// free shit
void refresh (char* buffer, int count){
	int i;
	for (i = 0; i < count; i++){
		buffer[i] = '\0';
	}
}

FreqTree* buildHelper(int fd) {

	char buffer[200];
	int i;
	for(i=0; i<200; i ++) {

		buffer[i]='\0';

	}

	int bytesRead;
	int tokLength=0;
	FreqTree *tokFreqTreeRoot = createFreqTree();

	do {

		char c=0;
		bytesRead = read(fd, &c, sizeof(char));
		//printf("%d\n", bytesRead);
		//printf("%c\n", c);
		//CHECKS
		if(isspace(c) || bytesRead == 0) {

			/*Create Node HERE*/
			tokFreqTreeRoot = insertIntoFreqTree(tokFreqTreeRoot, buffer);

			//insert(root, buffer);
			//printf("%s\n", root->token);
			//printf("%s\n", buffer);
			refresh(buffer, 200);
			tokLength=0;

			//whitespace
			buffer[0]= c;
			tokFreqTreeRoot = insertIntoFreqTree(tokFreqTreeRoot, buffer);
		}
		else {

			buffer[tokLength] = c;
			tokLength ++;

		}

	} while(bytesRead > 0);

	return root;

}


buildCodebook{
	// convert FreqTree* return root from buildHelper to minHeap
	// free FreqTree
	// build huffman tree from minHeap
    // write to new file created by in order traversing through huffman tree
    
}

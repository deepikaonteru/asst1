Node* readFile(int fd) {

	char buffer[200];
	int i;
	for(i=0; i<200; i ++) {

		buffer[i]='\0';

	}
	int bytesRead;
	int count=0;
	Node* root;

	do {

		char c=0;
		bytesRead = read(fd, &c, sizeof(char));
		//printf("%d\n", bytesRead);
		//printf("%c\n", c);
		//CHECKS
		if(isspace(c) || bytesRead == 0) {

			/*Create Node HERE*/
			root = insert(root, buffer);
			//printf("%s\n", root->token);
			//printf("%s\n", buffer);
			refresh(buffer, 200);
			count=0;
			

		}
		else if(isspace(c)) {

			continue;

		}
		else {

			buffer[count] = c;
			count ++;

		}
		

	} while(bytesRead > 0);

	return root;

}
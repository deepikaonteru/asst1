all: fileCompressor.c minHeap.o freqDict.o huffmanCodes.o
	gcc -o fileCompressor fileCompressor.c minHeap.o freqDict.o huffmanCodes.o

minHeap.o: minHeap.c
	gcc -c minHeap.c

freqDict.o: freqDict.c
	gcc -c freqDict.c

huffmanCodes.o: huffmanCodes.c
	gcc -c huffmanCodes.c

clean:
	rm fileCompressor minHeap.o freqDict.o huffmanCodes.o

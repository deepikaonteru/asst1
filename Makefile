all: fileCompressor.c minHeap.o freqDict.o
	gcc -o fileCompressor fileCompressor.c minHeap.o freqDict.o

minHeap.o: minHeap.c
	gcc -c minHeap.c

freqDict.o: freqDict.c
	gcc -c freqDict.c

clean:
	rm fileCompressor minHeap.o freqDict.o

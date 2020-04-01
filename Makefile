all: fileCompressor.c minHeap.o	freqDict.o
    gcc fileCompressor.c minHeap.o freqDict.o

minHeap.o: minHeap.c
    gcc -c minHeap.c

freqDict.o: freqDict.c
    gcc -c freqDict.c

clean:
    rm a.out minHeap.o freqDict.o
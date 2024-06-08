CC = gcc
CFLAGS = -Wall -Werror
OBJS = main.o treenode.o compressor.o decompressor.o huffman.o filehelper.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o huffman.exe
	rm -f $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

treenode.o: treenode.c
	$(CC) $(CFLAGS) -c treenode.c

compressor.o: compressor.c
	$(CC) $(CFLAGS) -c compressor.c

decompressor.o: decompressor.c
	$(CC) $(CFLAGS) -c decompressor.c

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

filehelper.o: filehelper.c
	$(CC) $(CFLAGS) -c filehelper.c

clean:
	rm -f *.o *.exe
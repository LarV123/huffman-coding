#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"
#include "treenode.h"
#include "compressor.h"
#include "decompressor.h"

#define MAX_BUFFER_SIZE 100

typedef unsigned char uchar;


int main(int argc, char** argv){

    if(argc < 3){
        puts("Huffman is a lossless ASCII file compression program.");
        puts("Usage: huffman [mode] [filepath]");
        puts("Available mode:");
        puts("\t-c\tCompress mode. Outputs a file with a file type \".compressed\"");
        puts("\t-d\tDecompress mode. Outputs a file with a file type \".txt\" and file name suffix \"-uncompressed\"");
        return 0;
    }

    const char* op_mode = argv[1];
    const char* file_name = argv[2];

    //Compress mode
    if(strncmp(op_mode, "-c", 2) == 0){
        return Compress(file_name);
    }

    //Decompress mode
    if(strncmp(op_mode, "-d", 2) == 0){
        return Decompress(file_name);
    }

    printf("Huffman is a lossless file compression program.\nUsage:\n\thuffman [mode] [filepath]");
    return 0;
}
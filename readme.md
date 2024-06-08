# Huffman Coding
this is my implementation of Huffman Coding using raw C and C standard library.

Huffman coding is a way to losslessly compress text file. Using huffman coding, we can try to use less bits for characters that appears often thus saving bits.

## Building
to build this code, use the make file provided.
```make all```
if you're on windows you can utilize w64devkit. You can find it on github.

## Using the program
To use the program
huffman [mode] [filename]
There are two available modes :
- Compression mode (-c)
this will compress the file and output [filename] but with a `.compressed` extension.
- Decompression mode (-d)
this will decompress the compressed file and output [filename] but with a `.txt` extension.
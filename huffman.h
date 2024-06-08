#include "treenode.h"

#ifndef HUFFMAN_DEF
#define HUFFMAN_DEF

//Will return the parent tree node
//Node from node_buffer till the return value is used
TreeNode_t* BuildHuffmanTree(TreeNodeBuffer_t* node_buffer, int char_occurences_length);

#endif
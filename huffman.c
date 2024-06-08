#include "stdio.h"
#include "stdlib.h"
#include "huffman.h"

TreeNode_t* BuildHuffmanTree(TreeNodeBuffer_t* node_buffer, int char_occurences_length){
    //our list head
    TreeNode_t* list_head = NULL;
    for(int i = 0; i < char_occurences_length; i++){
        list_head = AppendToList(list_head, node_buffer->node_buffer + i);
    }

    //Loop until there's only 1 entry to list
    while (list_head->next != NULL)
    {
        TreeNode_t* least_node = FindLeastInList(list_head);
        list_head = PopFromList(list_head, least_node);

        TreeNode_t* second_least_node = FindLeastInList(list_head);
        list_head = PopFromList(list_head, second_least_node);

        TreeNode_t* combined_node = CreateNode(node_buffer);
        combined_node->left = second_least_node;
        combined_node->right = least_node;
        combined_node->occurences = least_node->occurences + second_least_node->occurences;
        
        least_node->parent = combined_node;
        second_least_node->parent = combined_node;

        list_head = AppendToList(list_head, combined_node);
    }

    return list_head;
}
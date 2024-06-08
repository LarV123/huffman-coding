#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "treenode.h"
#include "huffman.h"
#include "decompressor.h"
#include "filehelper.h"

#define MAX_BUFFER_SIZE 100

typedef unsigned char uint8_t;

int Decompress(const char* file_name){

    FILE* file_ptr = fopen(file_name, "rb");

    if(file_ptr == NULL){
        printf("Cannot open file with the specified path");
        return -1;
    }

    TreeNode_t tree_buffer[1024];
    TreeNodeBuffer_t node_buffer;
    node_buffer.node_buffer = tree_buffer;
    node_buffer.max_buffer = 1024;
    node_buffer.current_index = 0;

    TreeNode_t** dfs_stack = malloc(sizeof(TreeNode_t*) * 100);
    int current_stack_index = 0;

    TreeNode_t* root_tree = CreateNode(&node_buffer);
    char last_character = fgetc(file_ptr);
    TreeNode_t* iter_tree = root_tree;

    int stack_safeguard = 1000;
    while(stack_safeguard--){

        char read_char = fgetc(file_ptr);

        if(read_char == last_character){
            TreeNode_t* blank_node = CreateNode(&node_buffer);
            blank_node->parent = iter_tree;

            if(iter_tree->left == NULL){
                iter_tree->left = blank_node;
                *(dfs_stack + current_stack_index) = iter_tree;
                current_stack_index++;
            }else {
                iter_tree->right = blank_node;
            }

            iter_tree = blank_node;
            continue;
        }

        if(read_char != last_character){
            TreeNode_t* char_node = CreateNode(&node_buffer);
            char_node->character = read_char;
            char_node->parent = iter_tree;
            last_character = read_char;
            if(iter_tree->left == NULL){
                iter_tree->left = char_node;
            }else {
                iter_tree->right = char_node;

                //end of the line, see stack
                if(current_stack_index == 0){
                    break;
                }

                iter_tree = *(dfs_stack + current_stack_index - 1);
                current_stack_index--;
            }
        }

    }

    if(stack_safeguard <= 0){
        puts("Corrupted file, failed to parse the huffman tree");
        fclose(file_ptr);
        return -1;
    }

    char* decompressed_file_name = malloc(sizeof(*decompressed_file_name) * (MAX_BUFFER_SIZE + 1));
    strncpy(decompressed_file_name, file_name, MAX_BUFFER_SIZE);
    decompressed_file_name[MAX_BUFFER_SIZE] = '\0';

    ChangeFileType(decompressed_file_name, "-uncompressed.txt", MAX_BUFFER_SIZE);
    FILE* decompressed_file_ptr = fopen(decompressed_file_name, "wb");

    char char_to_print = 0;
    uint8_t byte_to_read = 0;
    TreeNode_t* tree_iter = root_tree;
    do {
        byte_to_read = fgetc(file_ptr);
        for(int i = 0; i < 8; i++){
            if((byte_to_read >> (7-i)) & 1){
                tree_iter = tree_iter->right;
            }else{
                tree_iter = tree_iter->left;
            }
            if(tree_iter->character){
                if(tree_iter->character == EOF){
                    char_to_print = EOF;
                    break;
                }
                char_to_print = tree_iter->character;
                fputc(char_to_print, decompressed_file_ptr);
                tree_iter = root_tree;
            }
        }

    }while(char_to_print != EOF);

    printf("output in file %s", decompressed_file_name);

    fclose(file_ptr);
    fclose(decompressed_file_ptr);
    free(decompressed_file_name);

    return 0;
}
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "treenode.h"
#include "huffman.h"
#include "compressor.h"
#include "filehelper.h"

#define MAX_BUFFER_SIZE 100

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

void swap(uint8_t* a, uint8_t* b){
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

int Compress(const char* file_name){

    FILE* file_ptr = fopen(file_name, "rb");

    if(file_ptr == NULL){
        printf("Cannot open file with the specified path");
        return -1;
    }

    uint32_t occurences[256];

    memset(occurences, 0, sizeof(uint32_t) * 256);

    {
        char char_buffer;
        do{
            char_buffer = fgetc(file_ptr);
            uint8_t char_as_uint8 = char_buffer;
            occurences[char_as_uint8]++;
        }while(char_buffer != EOF);
    }

    uint8_t sorted_char_by_occurences[256];

    for(int i = 0; i < 256; i++){
        sorted_char_by_occurences[i] = i;
    }

    //Sorting char using quick sort in the stack without recursive
    {
        uint8_t stack_length = 0;
        uint8_t stack[256];
        int sorted_length = 0;

        while(sorted_length < 256){

            int partition_offset = sorted_length;

            int partition_length = 256-partition_offset;
            if(stack_length > 0){
                partition_length = stack[stack_length-1]-partition_offset;
            }

            if(partition_length == 0){
                sorted_length += 1;
                stack_length--;
                continue;
            }

            if(partition_length == 1){
                sorted_length += 1;
                continue;
            }

            uint8_t* char_array_partition = sorted_char_by_occurences + partition_offset;

            int partition_pivot_index = partition_length/2;

            //swap all iter
            swap(char_array_partition, char_array_partition + partition_pivot_index);
            partition_pivot_index = 0;

            for(int i = 1; i < partition_length; i++){
                if(occurences[char_array_partition[i]] > occurences[char_array_partition[partition_pivot_index]]){
                    swap(char_array_partition + i, char_array_partition + partition_pivot_index);
                    partition_pivot_index++;
                }
            }

            stack[stack_length++] = partition_offset + partition_pivot_index;
        }
    }

    int char_occurences_length = 0;
    for(int i = 0; i < 256; i++){
        if(occurences[sorted_char_by_occurences[i]] == 0){
            break;
        }
        char_occurences_length++;
    }

    TreeNode_t* tree_buffer = malloc(sizeof(TreeNode_t) * 1024);

    TreeNodeBuffer_t node_buffer;
    node_buffer.node_buffer = tree_buffer;
    node_buffer.max_buffer = 1024;
    node_buffer.current_index = 0;

    for(int i = 0; i < char_occurences_length; i++){
        TreeNode_t* new_node = CreateNode(&node_buffer);
        new_node->character = sorted_char_by_occurences[i];
        new_node->occurences = occurences[(uint8_t)(new_node->character)];
    }

    TreeNode_t* huffman_tree = BuildHuffmanTree(&node_buffer, char_occurences_length);
    (void)huffman_tree;
    
    {
        fseek(file_ptr, 0, SEEK_SET);
        char cur_character = fgetc(file_ptr);


        char* compress_file_name = (char*)malloc(sizeof(*compress_file_name) * (MAX_BUFFER_SIZE + 1));
        strncpy(compress_file_name, file_name, MAX_BUFFER_SIZE);
        compress_file_name[MAX_BUFFER_SIZE] = '\0';

        ChangeFileType(compress_file_name, ".compressed", MAX_BUFFER_SIZE);

        FILE* compressed_file_ptr = fopen(compress_file_name, "wb");

        int bit_length = 0;
        unsigned char compressed_buffer[1024];
        //init buffer to 0
        for(int i = 0; i < 1024; i++){
            compressed_buffer[i] = 0;
        }
        int byte_count = 0;

        while (true)
        {
            //writing to the buffer for each character will be backwards
            //because we start from the character node going up until the root node
            TreeNode_t* node = FindCharInList(cur_character, tree_buffer, char_occurences_length);
            int depth = FindNodeDeepInTree(node);
            int depth_iter = depth;

            uint8_t* buffer_ptr = compressed_buffer + bit_length / 8;
            while(node->parent != NULL){
                depth_iter--;

                uint8_t is_right = node->parent->right == node;
                uint8_t* buffer_to_write_ptr = buffer_ptr + (bit_length % 8 + depth_iter)/8;
                uint8_t write_pivot = (bit_length + depth_iter) % 8;
                *buffer_to_write_ptr |= is_right << (7-write_pivot);

                int offset_from_beginning = buffer_to_write_ptr - compressed_buffer;
                byte_count = offset_from_beginning + 1 > byte_count ? offset_from_beginning + 1 : byte_count;

                node = node->parent;
            }

            //depth_iter should be 0 here
            bit_length += depth;

            if(cur_character == EOF){
                break;
            }

            cur_character = fgetc(file_ptr);
        }

        //serialize huffman tree
        //just use the sorted occurences?
        //DFS Huffman Tree
        TreeNode_t** dfs_stack = (TreeNode_t**)malloc(sizeof(TreeNode_t*) * 100);
        int current_stack_index = 0;
        TreeNode_t* tree_iter = huffman_tree;
        char last_character = 0;
        int stack_safe_guard = 1000;
        while(stack_safe_guard--){

            if(current_stack_index > 100){
                puts("DFS Stack is exhausted");
                free(compress_file_name);
                free(dfs_stack);
                free(tree_buffer);
                return -1;
            }

            if(tree_iter->character != 0){
                fputc(tree_iter->character, compressed_file_ptr);
                last_character = tree_iter->character;

                if(current_stack_index == 0){
                    break;
                }

                tree_iter = *(dfs_stack + current_stack_index - 1);
                current_stack_index--;
                tree_iter = tree_iter->right;
                continue;
            }

            fputc(last_character, compressed_file_ptr);

            if(tree_iter->left != NULL){
                *(dfs_stack + current_stack_index) = tree_iter;
                current_stack_index += 1;
                tree_iter = tree_iter->left;
                continue;
            }
        }

        printf("output in file %s", compress_file_name);

        fwrite(compressed_buffer, sizeof(uint8_t), byte_count, compressed_file_ptr);

        fclose(compressed_file_ptr);
        free(dfs_stack);
        free(compress_file_name);
    }

    fclose(file_ptr);
    free(tree_buffer);

    return 0;
}
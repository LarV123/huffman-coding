#include "stdlib.h"
#include "treenode.h"

TreeNode_t* CreateNode(TreeNodeBuffer_t* buffer){
    if(buffer->current_index >= buffer->max_buffer){
        return NULL;
    }

    int new_index = buffer->current_index;
    buffer->current_index++;

    TreeNode_t* new_node = buffer->node_buffer + new_index;
    new_node->character = 0;
    new_node->occurences = 0;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->prev = NULL;

    return new_node;
}

TreeNode_t* FindLeastInList(TreeNode_t* list_head){
    if(list_head == NULL){
        return NULL;
    }

    TreeNode_t* min_node = list_head;
    list_head = list_head->next;
    while(list_head != NULL){
        if(min_node->occurences > list_head->occurences){
            min_node = list_head;
        }
        list_head = list_head->next;
    }

    return min_node;
}

//Returns the new head
TreeNode_t* PopFromList(TreeNode_t* list_head, TreeNode_t* to_pop_node){

    if(list_head == to_pop_node){
        list_head = list_head->next;
    }

    if(to_pop_node->prev != NULL){
        to_pop_node->prev->next = to_pop_node->next;
    }
    if(to_pop_node->next != NULL){
        to_pop_node->next->prev = to_pop_node->prev;
    }

    to_pop_node->next = NULL;
    to_pop_node->prev = NULL;

    return list_head;
}

TreeNode_t* AppendToList(TreeNode_t* list_head, TreeNode_t* new_node){
    if(list_head != NULL){
        list_head->prev = new_node;
    }
    new_node->next = list_head;

    return new_node;
}

TreeNode_t* FindCharInList(char character, TreeNode_t* iter, int length){
    while(length--){
        if(iter->character == character){
            return iter;
        }
        iter++;
    }

    return NULL;
}

int FindNodeDeepInTree(TreeNode_t* node){
    int iter = 0;
    while(node->parent != NULL){
        iter++;
        node = node->parent;
    }
    return iter;
}
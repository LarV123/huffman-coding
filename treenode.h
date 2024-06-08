#ifndef TREE_NODE_DEF
#define TREE_NODE_DEF
typedef struct TreeNode
{
    int occurences;
    char character;
    struct TreeNode* parent;
    struct TreeNode* left;
    struct TreeNode* right;
    struct TreeNode* prev;
    struct TreeNode* next;
} TreeNode_t;

typedef struct TreeNodeBuffer {
    TreeNode_t* node_buffer;
    int max_buffer;
    int current_index;
} TreeNodeBuffer_t;

TreeNode_t* CreateNode(TreeNodeBuffer_t* buffer);
TreeNode_t* FindLeastInList(TreeNode_t* list_head);
TreeNode_t* PopFromList(TreeNode_t* list_head, TreeNode_t* to_pop_node);
TreeNode_t* AppendToList(TreeNode_t* list_head, TreeNode_t* new_node);
TreeNode_t* FindCharInList(char character, TreeNode_t* iter, int length);
int FindNodeDeepInTree(TreeNode_t* node);

#endif
#include <stdio.h>
#include <stdlib.h>

const int MAX_WIDTH = 100;
const int MAX_NODE_COUNT = 100;
typedef struct Node {
    int data;
    int id;
    int parent_id;
    int count_subtrees;
    struct Node** subtrees;
} Node;

typedef struct Tree {
    int count_nodes;
    struct Node** nodes;
} Tree;

Node* create_node(int value, int id) 
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        return 1;
    }
    new_node->subtrees = (Node**)malloc(sizeof(Node*) * MAX_WIDTH);
    if (!new_node->subtrees) {
        return 1;
    }
    new_node->data = value;
    new_node->count_subtrees = 0;
    new_node->id = id;
    
    return new_node;
}

Tree* init_tree()
{
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (!tree) {
        return 1;
    }
    tree->nodes = (Node**)malloc(sizeof(Node*) * MAX_NODE_COUNT);
    if (!tree->nodes) {
        return 1;
    }
    tree->count_nodes = 0;
    return tree;
}

void add_root(Tree* tree, int val) {
    Node* new_node = create_node(val, 0);
    new_node->parent_id = -1;
    tree->count_nodes = 1;
    tree->nodes[0] = new_node;

}
void add_node(Tree* tree, int parent_id, int val)
{
    if (tree->count_nodes == MAX_NODE_COUNT) {
        printf("Too much nodes");
        return;
    }
    if (parent_id >= tree->count_nodes) {
        printf("error\n");
        return;
    }
    Node* new_node = create_node(val, tree->count_nodes);
    Node* parent = tree->nodes[parent_id];
    tree->nodes[tree->count_nodes] = new_node;
    tree->count_nodes++;
    
    parent->subtrees[parent->count_subtrees] = new_node;
    parent->count_subtrees++;
    new_node->parent_id = parent->id;
}

void delete_node(Tree* tree, int id)
{
    if (id >= tree->count_nodes) {
        printf("error\n");
        return;
    }
    Node* node = tree->nodes[id];
    while (node->count_subtrees > 0) {
        delete_node(tree, node->subtrees[0]->id);
    }
    if (node->parent_id != -1) {
        Node* parent = tree->nodes[node->parent_id];
        int index = -1;
        for (int i = 0; i < parent->count_subtrees; i++) {
            if (parent->subtrees[i] == node) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            for (int i = index; i < parent->count_subtrees - 1; i++) {
                parent->subtrees[i] = parent->subtrees[i + 1];
            }
            parent->count_subtrees--;
        }
        for (int i = id; i < tree->count_nodes - 1; i++) {
            tree->nodes[i] = tree->nodes[i + 1];
            tree->nodes[i]->id = i;
            for (int j = 0; j < tree->nodes[i]->count_subtrees; j++) {
                tree->nodes[i]->subtrees[j]->parent_id = i;
            }
        }
        tree->nodes[tree->count_nodes - 1] = NULL;
        tree->count_nodes--;
    } else {
        tree->count_nodes = 0;
    }
    free(node->subtrees);
    free(node);
    if (id == 0) {
        tree->nodes[0] = NULL;
    }
}

void delete_tree(Tree* tree) {
    delete_node(tree, 0);
    free(tree->nodes);
    free(tree);
}

void print_tree(Tree* tree, int id, int deep) {
    if (deep == 0 && tree->count_nodes == 0) {
        printf("No nodes\n");
        return;
    } 
    if (tree->nodes[id] == NULL) {
        return;
    }
    for (int i = 0; i < deep; i++) printf(" ");
    printf("%d(id %d)\n", tree->nodes[id]->data, id);
    if (tree->count_nodes == 0) {
        return;
    }
    for (int i = 0; i < tree->nodes[id]->count_subtrees; i++) {
        if (tree->nodes[id]->subtrees[i] != NULL) {
            print_tree(tree, tree->nodes[id]->subtrees[i]->id, deep + 1);
        }
    }
}

int count_depth(Tree* tree, int node_id, int cur_depth) {
    if (tree->count_nodes == 0) {
        return 0;
    }
    Node* node = tree->nodes[node_id];
    cur_depth += 1;
    int mx_sub_tree_depth = cur_depth;
    int sub_tree_depth;
    for (int i = 0; i < node->count_subtrees; i++) {
        sub_tree_depth = count_depth(tree, node->subtrees[i]->id, cur_depth);
        if (sub_tree_depth > mx_sub_tree_depth) {
            mx_sub_tree_depth = sub_tree_depth;
        }
    }

    return mx_sub_tree_depth;
}

int main(void) {
    Tree* tree = init_tree();
    int command, val, id;
    printf("1 - print tree\n2 - add node\n3 - delete node\n4 - count depth\n");
    do {
        printf("Enter command:\n");
        scanf("%d", &command);
        switch (command)
        {
        case 1:
            print_tree(tree, 0, 0);
            break;
        case 2:
            printf("Enter value:\n");
            scanf("%d", &val);
            if (tree->count_nodes == 0) {
                add_root(tree, val);
            }
            else {
                printf("Enter parent id:\n");
                scanf("%d", &id);
                add_node(tree, id, val);
            }
            break;
        case 3:
            printf("Enter node id:\n");
            scanf("%d", &id);
            delete_node(tree, id);
            break;
        case 4:
            printf("%d\n", count_depth(tree, 0, 0));
            break;
        case 0:
            break;
        default:
            printf("error\n");
            break;
        }
    } while (command != 0);
    delete_tree(tree);
    return 0;
}

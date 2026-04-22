#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_MAX_LEN 6

enum node_colors { RED, BLACK };

typedef struct Node {
    double value;
    char key[KEY_MAX_LEN + 1];
	enum node_colors color;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    
} Node;

Node* init_Node(double value, char* key, enum node_colors color) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    strncpy(node->key, key, KEY_MAX_LEN);
    node->key[KEY_MAX_LEN] = '\0';
    node->color = color;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void delete_Node(Node* node) {
    if (node == NULL) { return; }
    if (node->parent != NULL) {
        if (node == node->parent->right) {
            node->parent->right = NULL;
        } else if (node == node->parent->left) {
            node->parent->left = NULL;
        }
    }
    free(node);
    return;
}

void delete_Tree(Node* root) {
    if (root == NULL) { return; }
    if (root->left != NULL) { delete_Tree(root->left); }
    if (root->right != NULL) { delete_Tree(root->right); }
    free(root);
    return;
}

Node* grandparent(Node* node) {
    if (node == NULL) { return NULL; }
    if (node->parent == NULL) { return NULL; }
    return node->parent->parent;
}

Node* uncle(Node* node) {
    if (node == NULL) { return NULL; }
    if (node->parent == NULL) { return NULL; }
    if (grandparent(node) == NULL) { return NULL; }
    if (node->parent == grandparent(node)->left) {
        return grandparent(node)->right;
    }
    return grandparent(node)->left;
}

Node* rotate_left(Node* root, Node* node) {
    if (node == NULL) { return NULL; }
    if (node->right == NULL) { return NULL; }
    Node* right_son = node->right;
    node->right = right_son->left;
    if (node->right != NULL) {
        node->right->parent = node;
    }
    right_son->parent = node->parent;
    if (right_son->parent != NULL) {
        if (node == node->parent->left) {
            node->parent->left = right_son;
        } else {
            node->parent->right = right_son;
        }
    }
    right_son->left = node;
    node->parent = right_son;

    if (node == root) {
        return node->parent;
    }

    return root;
}

Node* rotate_right(Node* root, Node* node) {
    if (node == NULL) { return NULL; }
    if (node->left == NULL) { return NULL; }
    Node* left_son = node->left;
    node->left = left_son->right;
    if (node->left != NULL) {
        node->left->parent = node;
    }
    left_son->parent = node->parent;
    if (left_son->parent != NULL) {
        if (node == node->parent->left) {
            node->parent->left = left_son;
        } else {
            node->parent->right = left_son;
        }
    }
    left_son->right = node;
    node->parent = left_son;

    if (node == root) {
        return node->parent;
    }

    return root;
}

Node* insert_rebalance(Node* node, Node* root) {
    if (node == NULL) { return root; }
    if (node->parent == NULL) {
        node->color = BLACK;
        return node;
    }
    if (node->parent->color == BLACK) {
        return root;
    }
    Node* node_grandparent = grandparent(node);
    if (node_grandparent == NULL) { return NULL; }
    Node* node_uncle = uncle(node);
    enum node_colors uncle_color;
    if (node_uncle == NULL) {
        uncle_color = BLACK;
    } else {
        uncle_color = node_uncle->color;
    }

    if (uncle_color == RED) {
        node->parent->color = BLACK;
        node_uncle->color = BLACK;
        node_grandparent->color = RED;
        return insert_rebalance(node_grandparent, root);
    }
    if (node->parent == node_grandparent->left) {
        if (node == node->parent->left) {
            //левый-левый случай

            //правый поворот вокруг g
            Node* new_root = rotate_right(root, node_grandparent);
            if (new_root == NULL) {
                return NULL;
            }
            root = new_root;
            node->parent->color = BLACK;
            node_grandparent->color = RED;
        } else {
            //левый-правый случай

            //левый поворот вокруг P, правый поворот вокруг G
            Node* new_root = rotate_left(root, node->parent);
            if (new_root == NULL) {
                return NULL;
            }
            root = new_root;
            new_root = rotate_right(root, node_grandparent);
            if (new_root == NULL) {
                return NULL;
            }
            root = new_root;
            node->color = BLACK;
            node_grandparent->color = RED;
        }
    } else {
        if (node == node->parent->right) {
            //правый-правый случай

            //левый поворот вокруг g
            Node* new_root = rotate_left(root, node_grandparent);
            if (new_root == NULL) {
                return NULL;
            }
            root = new_root;
            node->parent->color = BLACK;
            node_grandparent->color = RED;
        } else {
            //правый-левый случай

            //правый поворот вокруг P, левый поворот вокруг G
            Node* new_root = rotate_right(root, node->parent);
            if (new_root == NULL) {
                return NULL;
            }
            root = new_root;
            new_root = rotate_left(root, node_grandparent);
            if (new_root == NULL) {
                return NULL;
            }
            root = new_root;
            node->color = BLACK;
            node_grandparent->color = RED;
        }
    }
    return root;
}

Node* insert_node(Node* root, char* key, double value) {
    Node* node = init_Node(value, key, RED);
    if (node == NULL) {
        return NULL;
    }
    if (root == NULL) {
        node->color = BLACK;
        return node;
    }
    Node* position = root;
    while (1) {
        if (strcmp(key, position->key) < 0) {
            if (position->left == NULL) {
                position->left = node;
                break;
            }
            position = position->left;
        }
        else if (strcmp(key, position->key) > 0) {
            if (position->right == NULL) {
                position->right = node;
                break;
            }
            position = position->right;
        } else {
            delete_Node(node);
            return NULL;
        }
    }
    node->parent = position;
    
    Node* new_root = insert_rebalance(node, root);
    if (new_root == NULL) {
        return NULL;
    }

    new_root->color = BLACK;
    return new_root;

}

Node* find_node(Node* root, char* key) {
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(key, root->key) == 0) {
        return root;
    } else if (strcmp(key, root->key) < 0) {
        return find_node(root->left, key);
    } else {
        return find_node(root->right, key);
    }
}

enum node_colors get_color(Node* node) {
    if (node == NULL) {
        return BLACK;
    }
    return node->color;
}

Node* remove_rebalance(Node* root, Node* parent, Node* brother, int is_side_left, enum node_colors color) {
    if (parent == NULL) {
        return root;
    }
    if (color == RED) {
        return root;
    }
    if (brother == NULL) {
        return root;
    }

    //Если был левым сыном
    if (is_side_left) {
        //чёрный брат
        if (brother->color == BLACK) {
            //у брата есть красный ребёнок
            
            //правый ребёнок красный
            if (get_color(brother->right) == RED) {
                brother->color = parent->color;
                parent->color = BLACK;
                brother->right->color = BLACK;
                Node* new_root = rotate_left(root, parent);
                if (new_root == NULL) {
                    return NULL;
                }
                return new_root;
            }

            //левый ребёнок красный
            if (get_color(brother->left) == RED) {
                brother->color = RED;
                brother->left->color = RED;
                Node* new_root = rotate_right(root, brother);
                if (new_root == NULL) {
                    return NULL;
                }
                root = new_root;
                return remove_rebalance(root, parent, parent->right, 1, BLACK);
            }

            //оба ребёнка чёрные
            brother->color = RED;
            if (parent->color == RED) {
                parent->color = BLACK;
                return root;
            }
            if (parent->parent != NULL) {
                Node* new_brother = NULL;
                if (parent == parent->parent->left) {
                    new_brother = parent->parent->right;
                } else {
                    new_brother = parent->parent->left;
                }
                return remove_rebalance(root, parent->parent, new_brother, parent == parent->parent->left, parent->color);
            } else {
                return root;
            }
        } else {
            //красный брат
            parent->color = RED;
            brother->color = BLACK;
            Node* new_root = rotate_left(root, parent);
            if (new_root == NULL) {
                return NULL;
            }
            root = new_root;
            return remove_rebalance(root, parent, parent->right, 1, color);
        }
    } else {
        //чёрный брат
        if (brother->color == BLACK) {
            //у брата есть красный ребёнок
            
            //левый ребёнок красный
            if (get_color(brother->left) == RED) {
                brother->color = parent->color;
                parent->color = BLACK;
                brother->left->color = BLACK;
                Node* new_root = rotate_right(root, parent);
                if (new_root == NULL) {
                    return NULL;
                }
                return new_root;
            }

            //правый ребёнок красный
            if (get_color(brother->right) == RED) {
                brother->color = RED;
                brother->right->color = RED;
                Node* new_root = rotate_left(root, brother);
                if (new_root == NULL) {
                    return NULL;
                }
                root = new_root;
                return remove_rebalance(root, parent, parent->left, 0, BLACK);
            }

            //оба ребёнка чёрные
            brother->color = RED;
            if (parent->color == RED) {
                parent->color = BLACK;
                return root;
            }
            if (parent->parent != NULL) {
                Node* new_brother = NULL;
                if (parent == parent->parent->left) {
                    new_brother = parent->parent->right;
                } else {
                    new_brother = parent->parent->left;
                }
                return remove_rebalance(root, parent->parent, new_brother, parent == parent->parent->left, parent->color);
            } else {
                return root;
            }
        } else {
            //красный брат
            parent->color = RED;
            brother->color = BLACK;
            Node* new_root = rotate_right(root, parent);
            if (new_root == NULL) {
                return NULL;
            }
            root = new_root;
            return remove_rebalance(root, parent, parent->left, 0, color);
        }
    }
    return root;
}

Node* remove_node(Node* root, Node* node) {
    if (node == NULL) {
        return NULL;
    }

    if (node->color == RED) {
        //Красный с 0 детьми
        if (node->left == NULL && node->right == NULL) {
            delete_Node(node);
            return root;
        }
        //Красный с 1 ребёнком невозможен т.к. иначе нарушается чёрная высота
    }

    //Красный/чёрный c 2 детьми
    if (node->left != NULL && node->right != NULL) {
        Node* replace = node->right;
        while (replace->left != NULL) {
            replace = replace->left;
        }
        strcpy(node->key, replace->key);
        node->value = replace->value;
        return remove_node(root, replace);
        //теперь гарантированно 0 или 1 детей
    }

    //Остались чёрные
    //1 ребёнок слева
    if (node->left != NULL) {
        strcpy(node->key, node->left->key);
        node->value = node->left->value;
        return remove_node(root, node->left);
    }
    //1 ребёнок справа
    if (node->right != NULL) {
        strcpy(node->key, node->right->key);
        node->value = node->right->value;
        return remove_node(root, node->right);
    }

    //чёрный 0 детей
    Node* parent = node->parent;
    Node* brother;
    int is_side_left = 0;
    if (node == parent->left) {
        brother = parent->right;
        is_side_left = 1;
    } else {
        brother = parent->left;
    }
    delete_Node(node);
    Node* new_root = remove_rebalance(root, parent, brother, is_side_left, BLACK);
    if (new_root == NULL) {
        return NULL;
    }
    new_root->color = BLACK;

    return new_root;
}

Node* remove_node_by_key(Node* root, char* key) {
    Node* node = find_node(root, key);
    if (node == NULL) {
        return NULL;
    }
    return remove_node(root, node);
}

void print_tree(FILE* file_output, Node* root, int deep) {
    if (root == NULL) {
        if (deep == 0) {
            fprintf(file_output, "Empty tree\n");
        }
        return;
    }
    for (int i = 0; i < deep; i++) {
        fprintf(file_output, "|");
    }
    if (deep != 0) {
        fprintf(file_output, "_");
    }
    if (root->parent != NULL) {
        if (root == root->parent->left) {
            fprintf(file_output, "L: ");
        } else {
            fprintf(file_output, "R: ");
        }
    }
    fprintf(file_output, "color: ");
    if (root->color == RED) {
        fprintf(file_output, "R ");
    } else {
        fprintf(file_output, "B ");
    }
    fprintf(file_output, "key: %s value: %lf\n", root->key, root->value);
    print_tree(file_output, root->left, deep + 1);
    print_tree(file_output, root->right, deep + 1);
}

int main(void) {
    FILE *file_input = fopen("input.txt", "r");
    if (file_input == NULL) {
        printf("error\n");
        return 1;
    }
    FILE *file_output = fopen("output.txt", "w");
    if (file_output == NULL) {
        fclose(file_input);
        printf("error\n");
        return 1;
    }
    int command = 0;
    char key[KEY_MAX_LEN];
    double value;
    Node* root = NULL;
    Node* new_root = NULL;
    Node* node = NULL;
    int error = 0;
    do {
        new_root = NULL;
        node = NULL;
        fscanf(file_input, "%d", &command);
        switch (command)
        {
        case 1:
            fscanf(file_input, "%6s %lf", key, &value);
            new_root = insert_node(root, key, value);
            if (new_root == NULL) {
                error = 1;
                break;
            }
            root = new_root;
            break;
        case 2:
            fscanf(file_input, "%6s", key);
            if (root == NULL) {
                fprintf(file_output, "Empty tree\n");
                break;
            }
            if (strcmp(root->key, key) == 0 && root->left == NULL && root->right == NULL) {
                delete_Tree(root);
                root = NULL;
                break;
            }
            new_root = remove_node_by_key(root, key);
            if (new_root == NULL) {
                error = 1;
                break;
            }
            root = new_root;
            break;
        case 3:
            print_tree(file_output, root, 0);
            break;
        case 4:
            fscanf(file_input, "%6s", key);
            node = find_node(root, key);
            if (node == NULL) {
                fprintf(file_output, "Key not exist\n");
                break;
            }
            fprintf(file_output, "Value: %lf\n", node->value);
            break;
        case 0:
            break;
        default:
            fprintf(file_output, "Unknown command\n");
            break;
        }
        if (error) {
            break;
        }
    } while (command != 0);
    if (error) {
        printf("error\n");
        fclose(file_input);
        fclose(file_output);
        delete_Tree(root);
        return 1;
    }
    fprintf(file_output, "exit\n");
    fclose(file_input);
    fclose(file_output);
    delete_Tree(root);
    return 0;
}
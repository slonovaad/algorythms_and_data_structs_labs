#include <stdio.h>
#include <stdlib.h>

const char OPERATORS[8] = {'(', ')', '-', '+', '*', '/', '~', '^'};

typedef struct Word {
    char* data;
    int len;
} Word;

Word* init_Word() {
    Word* word = (Word*)malloc(sizeof(Word));
    if (word == NULL) {
        return NULL;
    } 
    word->data = NULL;
    word->len = 0;
    return word;
}

void delete_Word(Word* word) {
    if (word == NULL) { return; }
    free(word->data);
    free(word);
}

int add_char_Word(Word* word, char c) {
    if (word == NULL) { return 1; }
    char* new_word_data = (char*)realloc(word->data, sizeof(char) * (word->len + 2));
    if (new_word_data == NULL) {
        delete_Word(word);
        return 1;
    }
    word->data = new_word_data;
    word->data[word->len] = c;
    word->data[word->len + 1] = '\0';
    word->len++;
    return 0;
}

Word* copy_Word(Word* word) {
    if (word == NULL) {
        return NULL;
    }
    Word* new_word = init_Word();
    if (new_word == NULL) {
        return NULL;
    }
    for (int i = 0; i < word->len; i++) {
        if (add_char_Word(new_word, word->data[i])) {
            return NULL;
        }
    }
    return new_word;
}

typedef struct WordMassive
{
    Word** data;
    int len;
} WordMassive;

WordMassive* init_WordMassive() {
    WordMassive* word_massive = (WordMassive*)malloc(sizeof(WordMassive));
    if (word_massive == NULL) { return NULL; }
    word_massive->data = NULL;
    word_massive->len = 0;
    return word_massive;
}

void delete_WordMassive(WordMassive* word_massive) {
    if (word_massive == NULL) { return; }
    for (int i = 0; i < word_massive->len; i++) {
        delete_Word(word_massive->data[i]);
    }
    free(word_massive->data);
    free(word_massive);
}

int add_word_WordMassive(WordMassive* word_massive, Word* word) {
    if (word_massive == NULL) { return 1; }
    Word** new_word_massive_data = (Word**)realloc(word_massive->data, sizeof(Word*) * (word_massive->len + 1));
    if (new_word_massive_data == NULL) {
        delete_WordMassive(word_massive);
        return 1;
    }
    word_massive->data = new_word_massive_data;
    word_massive->data[word_massive->len] = word;
    word_massive->len++;
    return 0;
}

Word* pop_word_WordMassive(WordMassive* word_massive) {
    if (word_massive == NULL) { return NULL; }
    if (word_massive-> len == 0) { return NULL; }
    Word* word = word_massive->data[word_massive->len - 1];
    word_massive->len--;
    return word;
}

typedef struct TreeNode
{
    Word* data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* init_TreeNode(Word* word) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) { return NULL; }
    node->data = word;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void delete_TreeNode(TreeNode* node) {
    if (node == NULL) { return; }
    delete_Word(node->data);
    delete_TreeNode(node->left);
    delete_TreeNode(node->right);
    free(node);
}
void delete_TreeNode_no_childrens(TreeNode* node) {
    if (node == NULL) { return; }
    delete_Word(node->data);
    free(node);
}

TreeNode* copy_Tree(TreeNode* node) {
    if (node == NULL) return NULL;
    Word* new_word = copy_Word(node->data);
    if (new_word == NULL) { return NULL; }
    TreeNode* new_node = init_TreeNode(new_word);
    if (new_node == NULL) {
        delete_Word(new_word);
        return NULL;
    }
    new_node->left = copy_Tree(node->left);
    new_node->right = copy_Tree(node->right);
    if ((node->left != NULL && new_node->left == NULL) || (node->right != NULL && new_node->right == NULL)) {
        delete_TreeNode(new_node);
        return NULL;
    }
    return new_node;
}

typedef struct TreeNodeMassive
{
    TreeNode** data;
    int len;
} TreeNodeMassive;

TreeNodeMassive* init_TreeNodeMassive() {
    TreeNodeMassive* massive = (TreeNodeMassive*)malloc(sizeof(TreeNodeMassive));
    if (massive == NULL) { return NULL; }
    massive->data = NULL;
    massive->len = 0;
    return massive;
}

void delete_TreeNodeMassive(TreeNodeMassive* massive) {
    if (massive == NULL) { return; }
    for (int i = 0; i < massive->len; i++) {
        delete_TreeNode(massive->data[i]);
    }
    free(massive->data);
    free(massive);
}

int add_node_NodeMassive(TreeNodeMassive* massive, TreeNode* node) {
    if (massive == NULL) { return 1; }
    TreeNode** new_massive_data = (TreeNode**)realloc(massive->data, sizeof(TreeNode*) * (massive->len + 1));
    if (new_massive_data == NULL) {
        delete_TreeNodeMassive(massive);
        return 1;
    }
    massive->data = new_massive_data;
    massive->data[massive->len] = node;
    massive->len++;
    return 0;
}

TreeNode* pop_node_TreeNodeMassive(TreeNodeMassive* massive) {
    if (massive == NULL) { return NULL; }
    if (massive-> len == 0) { return NULL; }
    TreeNode* node = massive->data[massive->len - 1];
    massive->len--;
    return node;
}

void print_Tree(TreeNode* root, int deep) {
    if (root == NULL) {
        if (deep == 0) {
            printf("Empty tree\n");
        }
        return;
    }
    if (root->data == NULL) {
        printf("error");
        return;
    }
    for (int i = 0; i < deep; i++) {
        printf("|");
    }
    if (deep != 0) {
        printf("_");
    }
    printf("%s\n", root->data->data);
    print_Tree(root->left, deep + 1);
    print_Tree(root->right, deep + 1);

}

int is_operator(char c) {
    for (int i = 0; i < 8; i++) {
        if (c == OPERATORS[i]) { return 1; }
    }
    return 0;
}

int is_operator_word(Word* word) {
    if (word == NULL) {
        return 0;
    }
    return (word->len == 1 && (is_operator(word->data[0])));
}

int priority(char c) {
    switch (c)
    {
    case '(': return 0;
    case ')': return 1;
    case '-': return 2;
    case '+': return 2;
    case '*': return 3;
    case '/': return 3;
    case '~': return 4;
    case '^': return 5;
    default: return -1;
    }
}

void print_expression(TreeNode* root, int deep) {
    if (root == NULL) {
        return;
    }
    if (!is_operator_word(root->data)) {
        printf("%s", root->data->data);
        return;
    }
    if (deep != 0) { printf("("); }
    if (root->data->data[0] != '~') {
        print_expression(root->left, deep + 1);
        printf("%s", root->data->data);
        print_expression(root->right, deep + 1);
    }
    else {
        printf("-");
        print_expression(root->left, deep + 1);
    }

    if (deep != 0) { printf(")"); }
    return;
}

WordMassive* to_postfix() {
    Word* word = NULL;
    Word* new_word = NULL;
    WordMassive* result = NULL;
    WordMassive* stack = NULL;
    char c;
    int temp;
    int count_operators = 0;
    int unary_counts = 0;
    int parenthesis_deep = 0;
    int can_operator = 0;
    int first_in_deep = 1;
    int unclosed_unary = 0;
    result = init_WordMassive();
    if (result == NULL) { 
        
        return NULL;
    }
    stack = init_WordMassive();
    if (stack == NULL) {
        delete_WordMassive(result);
        
        return NULL;
    }
    while ((temp = getchar()) != EOF && temp != '\n') {
        c = (char)temp;
        if (c == ' ') {
            if (word != NULL) {
                if (result->len != 0) {
                    if (can_operator) {
                        delete_Word(word);
                        delete_WordMassive(stack);
                        delete_WordMassive(result);
                        return NULL;
                    }
                }
                if (add_word_WordMassive(result, word)) {
                    delete_Word(word);
                    delete_WordMassive(stack);
                    
                    return NULL;
                }
                can_operator = 1;
            }
            word = NULL;
            continue;
        }
        if (is_operator(c)) {
            if (word != NULL) {
                if (add_word_WordMassive(result, word)) {
                    delete_Word(word);
                    delete_WordMassive(stack);
                    
                    return NULL;
                }
                can_operator = 1;
            }
            if (first_in_deep && c == '-') {
                c = '~';
                unary_counts++;
                unclosed_unary = 1;
            }
            word = init_Word();
            if (word == NULL) {
                delete_WordMassive(result);
                delete_WordMassive(stack);
                
                return NULL;
            }
            if (add_char_Word(word, c)) {
                delete_WordMassive(result);
                delete_WordMassive(stack);
                
                return NULL;
            }
            if (priority(c) == 0) {
                first_in_deep = 1;
                can_operator = 0;
                parenthesis_deep++;
                if (add_word_WordMassive(stack, word)) {
                    delete_Word(word);
                    delete_WordMassive(result);
                    
                    return NULL;
                }
            }
            else if (priority(c) == 1) {
                first_in_deep = 0;
                can_operator = 1;
                parenthesis_deep--;
                if (parenthesis_deep < 0 || unclosed_unary) {
                    delete_Word(word);
                    delete_WordMassive(stack);
                    delete_WordMassive(result);
                    
                    return NULL;
                }
                if (stack->len == 0) {
                    delete_Word(word);
                    delete_WordMassive(stack);
                    delete_WordMassive(result);
                    
                    return NULL;
                }
                while (stack->len > 0) {
                    if (priority(stack->data[stack->len - 1]->data[0]) == 0) {
                        new_word = pop_word_WordMassive(stack);
                        if (new_word == NULL) {
                            delete_Word(word);
                            delete_WordMassive(stack);
                            delete_WordMassive(result);
                            
                            return NULL;
                        }
                        delete_Word(new_word);
                        break;
                    }
                    new_word = pop_word_WordMassive(stack);
                    if (new_word == NULL) {
                        delete_Word(word);
                        delete_WordMassive(stack);
                        delete_WordMassive(result);
                        
                        return NULL;
                    }

                    if (add_word_WordMassive(result, new_word)) {
                        delete_Word(new_word);
                        delete_WordMassive(stack);
                        delete_Word(word);
                        
                        return NULL;
                    }
                }
                delete_Word(word);
                word = NULL;
            }
            else {
                if (c != '~' && !can_operator) {
                    if (word != NULL) { delete_Word(word); }
                    delete_WordMassive(result);
                    delete_WordMassive(stack);
                    
                    return NULL;
                }
                first_in_deep = 0;
                if (c != '~') { count_operators++; }
                can_operator = 0;
                if (stack->len == 0) {
                    if (add_word_WordMassive(stack, word)) {
                        delete_Word(word);
                        delete_WordMassive(result);
                        
                        return NULL;
                    }
                }
                else if (priority(c) > priority(stack->data[stack->len - 1]->data[0])) {
                    if (add_word_WordMassive(stack, word)) {
                        delete_Word(word);
                        delete_WordMassive(result);
                        
                        return NULL;
                    }
                }
                else {
                    while (stack->len > 0) {
                        if (priority(c) > priority(stack->data[stack->len - 1]->data[0])) {
                            break;
                        }
                        new_word = pop_word_WordMassive(stack);
                        if (new_word == NULL) {
                            delete_Word(new_word);
                            delete_Word(word);
                            delete_WordMassive(stack);
                            delete_WordMassive(result);
                            
                            return NULL;
                        }

                        if (add_word_WordMassive(result, new_word)) {
                            delete_Word(new_word);
                            delete_WordMassive(stack);
                            delete_Word(word);
                            
                            return NULL;
                        }
                    }
                    if (add_word_WordMassive(stack, word)) {
                        delete_Word(word);
                        delete_WordMassive(result);
                        
                        return NULL;
                    }
                }
            }
            word = NULL;
            new_word = NULL;
        }
        else {
            first_in_deep = 0;
            unclosed_unary = 0;
            if (word == NULL) {
                word = init_Word();
                if (word == NULL) {
                    delete_WordMassive(result);
                    delete_WordMassive(stack);
                    
                    return NULL;
                }
            }
            if (add_char_Word(word, c)) {
                delete_WordMassive(result);
                delete_WordMassive(stack);
                
                return NULL;
            }
        }
    }
    if (word != NULL) {
        if (add_word_WordMassive(result, word)) {
            delete_Word(word);
            delete_WordMassive(stack);
            
            return NULL;
        }
        word = NULL;
    }
    while (stack->len != 0) {
        word = pop_word_WordMassive(stack);
        if (word == NULL) {
            delete_WordMassive(result);
            delete_WordMassive(stack);
            
            return NULL;
        }
        if (add_word_WordMassive(result, word)) {
            delete_Word(word);
            delete_WordMassive(stack);
            
            return NULL;
        }
    }
    if ((result->len - unary_counts - 2 * count_operators != 1) || (parenthesis_deep != 0) || unclosed_unary) {
        delete_WordMassive(result);
        delete_WordMassive(stack);
        
        return NULL;
    }
    delete_WordMassive(stack);
    return result;
}

TreeNode* postfix_to_tree(WordMassive* postfix) {
    if (postfix == NULL) {
        return NULL;
    }
    TreeNodeMassive* stack = NULL;
    stack = init_TreeNodeMassive();
    if (stack == NULL) {
        return NULL;
    }
    Word* new_word = NULL;
    TreeNode* node = NULL;
    TreeNode* second_node = NULL;
    TreeNode* root = NULL;
    for (int i = 0; i < postfix->len; i++) {
        new_word = copy_Word(postfix->data[i]);
        if (new_word == NULL) {
            delete_TreeNodeMassive(stack);
            delete_TreeNode(node);
            delete_TreeNode(second_node);
            delete_TreeNode(root);
            return NULL;
        }
        if (!is_operator_word(postfix->data[i])) {
            node = init_TreeNode(new_word);
            if (node == NULL) {
                delete_TreeNodeMassive(stack);
                delete_TreeNode(second_node);
                delete_TreeNode(root);
                return NULL;
            }
            if (add_node_NodeMassive(stack, node)) {
                delete_TreeNode(node);
                delete_TreeNode(second_node);
                delete_TreeNode(root);
                return NULL;
            }
            node = NULL;
        }
        else {
            root = init_TreeNode(new_word);
            if (root == NULL) {
                delete_TreeNodeMassive(stack);
                delete_TreeNode(node);
                delete_TreeNode(second_node);
                return NULL;
            }
            
            node = pop_node_TreeNodeMassive(stack);
            if (node == NULL) {
                delete_TreeNodeMassive(stack);
                delete_TreeNode(second_node);
                delete_TreeNode(root);
                return NULL;
            }
            if (postfix->data[i]->data[0] != '~') {
                second_node = pop_node_TreeNodeMassive(stack);
                if (second_node == NULL) {
                    delete_TreeNodeMassive(stack);
                    delete_TreeNode(node);
                    delete_TreeNode(root);
                    return NULL;
                }
                root->left = second_node;
                root->right = node;
            }
            else {
                root->left = node;
            }
            if (add_node_NodeMassive(stack, root)) {
                delete_TreeNode(root);
                delete_TreeNode(node);
                delete_TreeNode(second_node);
                return NULL;
            }
            node = NULL;
            second_node = NULL;
            root = NULL;
        }
    }
    if (stack->len != 1) {
        delete_TreeNodeMassive(stack);
        return NULL;
    }
    root = pop_node_TreeNodeMassive(stack);
    if (root == NULL) {
        return NULL;
    }
    delete_TreeNodeMassive(stack);
    return root;
}

int fractions_to_one_floor(TreeNode* root) {
    if (root == NULL) { return 1; }
    if (root->data == NULL) { return 1; }
    if (root->left != NULL) {
        if (fractions_to_one_floor(root->left)) { return 1; }
    }
    if (root->right != NULL) {
        if (fractions_to_one_floor(root->right)) { return 1; }
    }
    if (root->data->data[0] != '/' && root->data->data[0] != '*') {
        return 0;
    }
    TreeNode* left_copy = copy_Tree(root->left);
    TreeNode* right_copy = copy_Tree(root->right);
    if (left_copy == NULL || right_copy == NULL) {
        delete_TreeNode(left_copy);
        delete_TreeNode(right_copy);
        return 1;
    }
    TreeNode* left_numerator = NULL;
    TreeNode* left_denumerator = NULL;
    TreeNode* right_numerator = NULL;
    TreeNode* right_denumerator = NULL;
    TreeNode* new_numerator = NULL;
    TreeNode* new_denumerator = NULL;

    Word* multiply = NULL;
    int need_delete_left_op = 0, need_delete_right_op = 0;

    if (root->data->data[0] == '*') {
        if (left_copy->data->data[0] == '/') {
            need_delete_left_op = 1;
            left_numerator = left_copy->left;
            left_denumerator = left_copy->right;
        }
        else {
            left_numerator = left_copy;
        }
        if (right_copy->data->data[0] == '/') {
            need_delete_right_op = 1;
            right_numerator = right_copy->left;
            right_denumerator = right_copy->right;
        }
        else {
            right_numerator = right_copy;
        }
        if (left_denumerator == NULL && right_denumerator == NULL) {
            delete_TreeNode(left_copy);
            delete_TreeNode(right_copy);
            return 0;
        }
        multiply = init_Word();
        if (multiply == NULL) {
            delete_TreeNode(left_copy);
            delete_TreeNode(right_copy);
            return 1;
        }
        if (add_char_Word(multiply, '*')) {
            delete_TreeNode(left_copy);
            delete_TreeNode(right_copy);
            return 1;
        }
        new_numerator = init_TreeNode(multiply);
        if (new_numerator == NULL) {
            delete_Word(multiply);
            delete_TreeNode(left_copy);
            delete_TreeNode(right_copy);
            return 1;
        }
        new_numerator->left = left_numerator;
        new_numerator->right = right_numerator;

        if (left_denumerator != NULL && right_denumerator == NULL) {
            new_denumerator = left_denumerator;
        } else if (left_denumerator == NULL && right_denumerator != NULL) {
            new_denumerator = right_denumerator;
        } else {
            multiply = init_Word();
            if (multiply == NULL) {
                delete_TreeNode(new_numerator);
                delete_TreeNode(left_copy);
                delete_TreeNode(right_copy);
                return 1;
            }
            if (add_char_Word(multiply, '*')) {
                delete_TreeNode(new_numerator);
                delete_TreeNode(left_copy);
                delete_TreeNode(right_copy);
                return 1;
            }
            new_denumerator = init_TreeNode(multiply);
            if (new_denumerator == NULL) {
                delete_Word(multiply);
                delete_TreeNode(new_numerator);
                delete_TreeNode(left_copy);
                delete_TreeNode(right_copy);
                return 1;
            }
            new_denumerator->left = left_denumerator;
            new_denumerator->right = right_denumerator;
        }
        Word* division = init_Word();
        if (division == NULL) {
            delete_TreeNode(new_numerator);
            if (new_denumerator != left_denumerator && new_denumerator != right_denumerator) delete_TreeNode(new_denumerator);
            if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
            if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
            return 1;
        }
        if (add_char_Word(division, '/')) {
            delete_TreeNode(new_numerator);
            if (new_denumerator != left_denumerator && new_denumerator != right_denumerator) delete_TreeNode(new_denumerator);
            if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
            if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
            return 1;
        }
        delete_Word(root->data);
        root->data = division;
        delete_TreeNode(root->left);
        delete_TreeNode(root->right);
        root->left = new_numerator;
        root->right = new_denumerator;
        if (need_delete_left_op) {
            delete_TreeNode_no_childrens(left_copy);
        }
        if (need_delete_right_op) {
            delete_TreeNode_no_childrens(right_copy);
        }
        return 0;
    }
    if (left_copy->data->data[0] == '/') {
        left_numerator = left_copy->left;
        left_denumerator = left_copy->right;
        need_delete_left_op = 1;
    }
    else {
        left_numerator = left_copy;
    }
    if (right_copy->data->data[0] == '/') {
        right_numerator = right_copy->left;
        right_denumerator = right_copy->right;
        need_delete_right_op = 1;
    }
    else {
        right_numerator = right_copy;
    }
    int new_num_created = 0;
    int new_den_created = 0;
    if (right_denumerator != NULL) {
        multiply = init_Word();
        if (multiply == NULL) {
            if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
            if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
            return 1;
        }
        if (add_char_Word(multiply, '*')) {
            if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
            if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
            return 1;
        }
        new_numerator = init_TreeNode(multiply);
        if (new_numerator == NULL) {
            delete_Word(multiply);
            if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
            if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
            return 1;
        }
        new_numerator->left = left_numerator;
        new_numerator->right = right_denumerator;
        new_num_created = 1;
    }
    else {
        new_numerator = left_numerator;
    }
    if (left_denumerator != NULL) {
        multiply = init_Word();
        if (multiply == NULL) {
            if (new_num_created) delete_TreeNode(new_numerator);
            if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
            if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
            return 1;
        }
        if (add_char_Word(multiply, '*')) {
            if (new_num_created) delete_TreeNode(new_numerator);
            if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
            if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
            return 1;
        }
        new_denumerator = init_TreeNode(multiply);
        if (new_denumerator == NULL) {
            delete_Word(multiply);
            if (new_num_created) delete_TreeNode(new_numerator);
            if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
            if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
            return 1;
        }
        new_denumerator->left = left_denumerator;
        new_denumerator->right = right_numerator;
        new_den_created = 1;
    }
    else {
        new_denumerator = right_numerator;
    }
    Word* division = init_Word();
    if (division == NULL) {
        if (new_num_created) delete_TreeNode(new_numerator);
        if (new_den_created) delete_TreeNode(new_denumerator);
        if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
        if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
        return 1;
    }
    if (add_char_Word(division, '/')) {
        if (new_num_created) delete_TreeNode(new_numerator);
        if (new_den_created) delete_TreeNode(new_denumerator);
        if (need_delete_left_op) delete_TreeNode_no_childrens(left_copy); else delete_TreeNode(left_copy);
        if (need_delete_right_op) delete_TreeNode_no_childrens(right_copy); else delete_TreeNode(right_copy);
        return 1;
    }
    delete_Word(root->data);
    root->data = division;
    delete_TreeNode(root->left);
    delete_TreeNode(root->right);
    root->left = new_numerator;
    root->right = new_denumerator;
    if (need_delete_left_op) {
        delete_TreeNode_no_childrens(left_copy);
    }
    if (need_delete_right_op) {
        delete_TreeNode_no_childrens(right_copy);
    }
    return 0;
}

int main(void) {
    WordMassive* result = to_postfix();
    if (result == NULL) {
        printf("error");
        return 1;
    }
    printf("Postfix:\n");
    for (int i = 0; i < result->len; i++) {
        printf("%s ", result->data[i]->data);
    }
    printf("\nOriginal tree:\n");
    TreeNode* root = postfix_to_tree(result);
    if (root == NULL) {
        delete_WordMassive(result);
        printf("error");
        return 1;
    }
    print_Tree(root, 0);
    printf("Modifyded tree:\n");
    if(fractions_to_one_floor(root)) {
        delete_WordMassive(result);
        delete_TreeNode(root);
        printf("error");
        return 1;
    }
    print_Tree(root, 0);
    printf("Modifyded expression:\n");
    print_expression(root, 0);
    delete_WordMassive(result);
    delete_TreeNode(root);
    return 0;
}

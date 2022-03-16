#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node {

    int value;
    int color; // Black = 0, Red = 1
    struct Node* parent;
    struct Node* left;
    struct Node* right;

} Node;

typedef struct rbTree {
    int numNodes;
    Node* root;
} rbTree;

void addPrint(rbTree* tree, int value);
void add(rbTree* tree, int value);
void rebalance(Node* current, rbTree* tree);
void printTree(rbTree* tree);
void prepPrint(Node* current, int depth, int maxDepth, int** array);
int toInt(char* num);

int main(int argc, char **argv) {

    rbTree* tree = calloc(1, sizeof(rbTree));

    for (int i = 1; i < argc; i++) {
        addPrint(tree, toInt(argv[i]));
    }

    return 0;
}

typedef enum imbalance {
    LL = 1, RR, LR, RL
} imbalance;

void prepPrint(Node* current, int depth, int maxDepth, int** array) {
    
    if (depth == maxDepth) {
        return;
    }

    int index = ++array[depth][0];
    if (current == NULL) {
        return;
    }

    Node* childL = current->left;
    Node* childR = current->right;
    int value = current->value;
    if (current->color == 1) { value *= -1; }
    array[depth][index] = value;
    
    prepPrint(childL, depth+1, maxDepth, array);
    prepPrint(childR, depth+1, maxDepth, array);
}

void printTree(rbTree* tree) {

    int treeHeight = (int) (2 * (log(tree->numNodes + 1) / log(2)));

    // Initialize the array
    int** array = calloc(treeHeight, sizeof(int*));
    for (int i = 0; i < treeHeight; i++) {

        int arrayRowLength = (int) pow(2, treeHeight-1)+1;
        array[i] = calloc(arrayRowLength, sizeof(int));

        for (int j = 1; j < arrayRowLength; j++) {
            array[i][j] = 1000;
        }
    }
    
    prepPrint(tree->root, 0, treeHeight, array);
    
    printf("RB Tree\n");
    for (int i = 0; i < treeHeight; i++) {

        int nodesOnRow = (int) pow(2, i);
        for (int j = 0; j < nodesOnRow; j++) {

            int padding;

            if (j == 0) {
                padding = (int) pow(2, treeHeight - i) - 2;
            } else {
                padding = (int) pow(2, treeHeight - i + 1) - 3;
            }

            for (int k = 0; k < padding; k++) {
                printf(" ");
            }

            int value = array[i][j+1];
            if (value == 1000) {
                printf("___");
            } else {
                printf("%3d", array[i][j+1]);
            }
            
            
        }

        printf("\n");
    }

}

void rebalance(Node* current, rbTree* tree) {

    // Relevant Nodes
    Node* childL = current->left;
    Node* childR = current->right;
    Node* sibling;
    Node* parent = current->parent;
    Node* uncle;
    Node* grandfather;
    Node* greatGrandfather;
    int ggIsLeft = 0;

    // Current is the root
    if (parent == NULL) {
        return;
    }

    // Parent is black; no conflict
    if (parent->color == 0) {
        return;
    }

    // Get the great/grandparent
    grandfather = parent->parent;
    greatGrandfather = grandfather->parent;

    // Find which child grandfather is
    if (greatGrandfather != NULL && grandfather->value < greatGrandfather->value) {
        ggIsLeft = 1;
    }
    
    // Determine the imbalance type (LL, LR, RR, RL)
    imbalance imbal;
    if (parent->value < grandfather->value) {
        uncle = grandfather->right;

        if (current->value < parent->value) {
            imbal = LL;
            sibling = parent->right;
        } else {
            imbal = LR;
            sibling = parent->left;
        }
    } else {
        uncle = grandfather->left;

        if (current->value > parent->value) {
            imbal = RR;
            sibling = parent->left;
        } else {
            imbal = RL;
            sibling = parent->right;
        }
    }

    // If the uncle is red
    if (uncle != NULL && uncle->color == 1) {

        uncle->color = 0;
        parent->color = 0;
        grandfather->color = 1;
        tree->root->color = 0;
        return rebalance(grandfather, tree);
    }

    if (imbal == LL) {

        sibling->parent = grandfather;
        parent->parent = greatGrandfather;
        grandfather->parent = parent;

        parent->right = grandfather;
        grandfather->left = sibling;
    }
    if (imbal == RR) {

        sibling->parent = grandfather;
        parent->parent = greatGrandfather;
        grandfather->parent = parent;

        parent->left = grandfather;
        grandfather->right = sibling;
    }
    if (imbal == LR) {

        current->parent = grandfather->parent;
        if (childL != NULL) { childL->parent = parent; }
        if (childR != NULL) { childR->parent = grandfather; }
        parent->parent = current;
        grandfather->parent = current;

        current->left = parent;
        current->right = grandfather;
        parent->right = childL;
        grandfather->left = childR;
    }
    if (imbal == RL) {

        current->parent = grandfather->parent;
        if (childL != NULL) { childL->parent = grandfather; }
        if (childR != NULL) { childR->parent = parent; }
        parent->parent = current;
        grandfather->parent = current;

        current->left = grandfather;
        current->right = parent;
        parent->left = childR;
        grandfather->right = childL;
    }

    if (imbal == LL || imbal == RR) {
        if (greatGrandfather != NULL) {  
            if (ggIsLeft) {
                greatGrandfather->left = parent;
            } else {
                greatGrandfather->right = parent;
            }
        } else {
            tree->root = parent;
        }

        parent->color = 0;
    }
    
    if (imbal == LR || imbal == RL) {
        if (greatGrandfather != NULL) {  
            if (ggIsLeft) {
                greatGrandfather->left = current;
            } else {
                greatGrandfather->right = current;
            }
        } else {
            tree->root = current;
        }

        current->color = 0;
    }

    grandfather->color = 1;
    tree->root->color = 0;
    rebalance(grandfather, tree);

}

void add(rbTree* tree, int value) {

    tree->numNodes++;

    Node* current = tree->root;
    Node* new_node = calloc(1, sizeof(Node));
    new_node->value = value;
    new_node->color = 1;

    if (current == NULL) {
        tree->root = new_node;
        new_node->color = 0;
        return;
    }

    // Loop for recursive placement locating
    while (1) {

        // Check if the value is to the left
        if (value < current->value) {

            // The left child doesn't exist
            if (current->left == NULL) {
                
                // Create the new left child
                new_node->parent = current;
                current->left = new_node;
                break;
            
            // The left child exists
            } else {
                current = current->left;
            }

        // The value is on the right
        } else {

            // The right child doesn't exist
            if (current->right == NULL) {
                
                // Create the new right child
                new_node->parent = current;
                current->right = new_node;
                break;

            // The right child exists
            } else {
                current = current->right;
            }

        }

    }

    rebalance(new_node, tree);

}

void addPrint(rbTree* tree, int value) {

    add(tree, value);
    printTree(tree);
}

int toInt(char* num) {
    int length = strlen(num);
    int rtn = 0;

    for (int i = 0; i < length; i++) {
        
        rtn = rtn*10 + (num[i] - '0');
    }

    return rtn;
}
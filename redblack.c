#include <stdio.h>

typedef struct Node {

    int value;
    int color; // Black = 0, Red = 1
    struct Node* parent;
    struct Node* left;
    struct Node* right;

} Node;

void add(Node* root, int value);
void rebalance(Node* current);

int main() {

    Node* root = malloc(sizeof(Node));
    root->color = 0;

    return 0;
}

typedef enum imbalance {
    LL = 1, RR, LR, RL
} imbalance;

void rebalance(Node* current) {

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
    if (grandfather->value < greatGrandfather->value) {
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
        return rebalance(grandfather);
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
        }

        current->color = 0;
    }

    grandfather->color = 1;
    rebalance(grandfather);

}

void add(Node* root, int value) {

    Node* current = root;

    // Loop for recursive placement locating
    while (1) {

        // Check if the value is to the left
        if (value < root->value) {

            // The left child doesn't exist
            if (root->left == NULL) {
                
                // Create the new left child
                Node* new_left = malloc(sizeof(Node));
                new_left->value = value;
                new_left->color = 1;
                new_left->parent = root;
                root->left = new_left;
                break;
            
            // The left child exists
            } else {
                current = root->left;
            }

        // The value is on the right
        } else {

            // The right child doesn't exist
            if (root->right == NULL) {
                
                // Create the new right child
                Node* new_right = malloc(sizeof(Node));
                new_right->value = value;
                new_right->color = 1;
                new_right->parent = root;
                root->right = new_right;
                break;

            // The right child exists
            } else {
                current = root->right;
            }

        }

    }

}
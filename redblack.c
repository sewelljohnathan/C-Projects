#include <stdio.h>

typedef struct Node {

    int value;
    int color; // Black = 0, Red = 1
    Node* parent;
    Node* left;
    Node* right;

} Node;

void add(Node* root, int value);


int main() {

    Node* root = malloc(sizeof(Node));
    root->color = 0;

    return 0;
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
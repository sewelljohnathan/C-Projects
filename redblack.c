#include <stdio.h>

typedef struct Node {

    int color; // Black = 0, Red = 1
    Node* parent;
    Node* left;
    Node* right;

} Node;


int main() {

    Node* root = malloc(sizeof(Node));
    root->color = 0;
    root->parent = NULL;
    root->left = NULL;
    root->right = NULL;

    return 0;
}
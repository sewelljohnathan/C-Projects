#include <stdio.h>

typedef struct Node {

    int color; // Black = 0, Red = 1
    Node* parent;
    Node* left;
    Node* right;

} Node;


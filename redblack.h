// Struct for the Node
typedef struct Node {

    int value;
    int color; // Black = 0, Red = 1
    struct Node* parent;
    struct Node* left;
    struct Node* right;

} Node;

// Struct for the R-B Tree
typedef struct rbTree {

    int numNodes;
    Node* root;

} rbTree;

// Imbalance type for tree rotations
typedef enum imbalance {
    LL = 1, RR, LR, RL

} imbalance;

// Combines the add and printTree methods into one
void addPrint(rbTree* tree, int value);

// Adds a new node to the tree based on a value
void add(rbTree* tree, int value);

// Rebalances the tree to ensure is satisfies red-black rules
void rebalance(Node* current, rbTree* tree);

// Print the tree
void printTree(rbTree* tree);

// A helper method for printTree that organizes node values by row
void prepPrint(Node* current, int depth, int maxDepth, int** array);

// Converts a string to an int
int toInt(char* num);
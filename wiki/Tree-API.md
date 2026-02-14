# Tree API

The Tree data structure provides a generic n-ary tree implementation where each node can have multiple children.

## Overview

The Tree uses a TreeNode structure where each node can store any type of data and maintain a list of child nodes. Children are stored using a singly linked list, allowing each node to have an arbitrary number of children.

## Creating a Tree Node

```c
#include <cbarroso/tree.h>

TreeNode *node = TreeNode__new(void *value, size_t valueSize);
```

### Parameters

- `value` - Pointer to the value to store in the node
- `valueSize` - Size of the value in bytes

### Returns

- Pointer to the newly created TreeNode on success
- `NULL` on failure (memory allocation error)

### Example

```c
// Create a tree node with an integer value
int data = 42;
TreeNode *node = TreeNode__new(&data, sizeof(int));
if (!node) {
    fprintf(stderr, "Failed to create tree node\n");
    return 1;
}
```

## Inserting Child Nodes

```c
int8_t TreeNode__insert(TreeNode *self, TreeNode *newChild);
```

### Parameters

- `self` - Pointer to the parent TreeNode
- `newChild` - Pointer to the child TreeNode to insert

### Returns

- `0` (CBR_SUCCESS) on success
- `-1` (CBR_ERROR) on error

### Example

```c
int parentValue = 1;
int childValue = 2;

TreeNode *parent = TreeNode__new(&parentValue, sizeof(int));
TreeNode *child = TreeNode__new(&childValue, sizeof(int));

int8_t result = TreeNode__insert(parent, child);
if (result != CBR_SUCCESS) {
    fprintf(stderr, "Failed to insert child node\n");
}
```

## Accessing Node Data

### Node Structure

```c
typedef struct TreeNode {
    void *value;                       // Pointer to the stored value
    size_t valueSize;                  // Size of the value in bytes
    SinglyLinkedListNode *childrenHead; // First child node
    SinglyLinkedListNode *childrenTail; // Last child node
    size_t numOfChildren;              // Number of children
} TreeNode;
```

### Traversing Children

```c
TreeNode *parent = /* ... */;

// Iterate through all children
SinglyLinkedListNode *current = parent->childrenHead;
while (current != NULL) {
    TreeNode *child = (TreeNode *)current->value;
    // Process child node
    int childValue = *(int *)child->value;
    printf("Child value: %d\n", childValue);
    
    current = current->next;
}
```

## Deleting a Tree

```c
void TreeNode__del(TreeNode *self);
```

### Parameters

- `self` - Pointer to the root TreeNode to delete

### Behavior

This function recursively deletes the entire tree starting from the given node:
- Traverses all children recursively
- Frees all child nodes
- Frees the node's value
- Frees the node itself

**Important**: This function is safe to call with `NULL`.

### Example

```c
TreeNode *root = TreeNode__new(&value, sizeof(int));
// ... build tree ...

// Delete entire tree
TreeNode__del(root);
```

## Memory Management

**Important Memory Notes:**

1. **Value Storage**: TreeNode makes a **copy** of the value you provide. The original value can be freed or go out of scope after node creation.

2. **Ownership**: When you insert a child node, the tree takes ownership. Don't manually free child nodes - `TreeNode__del` on the parent will handle all children recursively.

3. **Cleanup**: Always call `TreeNode__del` on the root node when done to prevent memory leaks.

## Complete Example

### Example 1: Simple Tree

```c
#include <stdio.h>
#include <cbarroso/tree.h>

int main(void) {
    // Create root node
    int rootValue = 1;
    TreeNode *root = TreeNode__new(&rootValue, sizeof(int));
    
    // Create and add children
    int child1Value = 2;
    int child2Value = 3;
    int child3Value = 4;
    
    TreeNode *child1 = TreeNode__new(&child1Value, sizeof(int));
    TreeNode *child2 = TreeNode__new(&child2Value, sizeof(int));
    TreeNode *child3 = TreeNode__new(&child3Value, sizeof(int));
    
    TreeNode__insert(root, child1);
    TreeNode__insert(root, child2);
    TreeNode__insert(root, child3);
    
    // Print number of children
    printf("Root has %zu children\n", root->numOfChildren);
    
    // Traverse and print children
    SinglyLinkedListNode *current = root->childrenHead;
    while (current != NULL) {
        TreeNode *child = (TreeNode *)current->value;
        printf("Child value: %d\n", *(int *)child->value);
        current = current->next;
    }
    
    // Cleanup - deletes entire tree
    TreeNode__del(root);
    return 0;
}
```

### Example 2: Nested Tree Structure

```c
#include <stdio.h>
#include <string.h>
#include <cbarroso/tree.h>

typedef struct {
    char name[50];
    int age;
} Person;

int main(void) {
    // Create family tree
    Person grandparent = {"John", 70};
    Person parent1 = {"Alice", 45};
    Person parent2 = {"Bob", 43};
    Person child1 = {"Charlie", 20};
    Person child2 = {"Diana", 18};
    
    TreeNode *gp = TreeNode__new(&grandparent, sizeof(Person));
    TreeNode *p1 = TreeNode__new(&parent1, sizeof(Person));
    TreeNode *p2 = TreeNode__new(&parent2, sizeof(Person));
    TreeNode *c1 = TreeNode__new(&child1, sizeof(Person));
    TreeNode *c2 = TreeNode__new(&child2, sizeof(Person));
    
    // Build tree structure
    TreeNode__insert(gp, p1);
    TreeNode__insert(gp, p2);
    TreeNode__insert(p1, c1);
    TreeNode__insert(p1, c2);
    
    // Print tree structure
    Person *gpData = (Person *)gp->value;
    printf("Grandparent: %s (%d)\n", gpData->name, gpData->age);
    printf("  Children: %zu\n", gp->numOfChildren);
    
    SinglyLinkedListNode *current = gp->childrenHead;
    while (current != NULL) {
        TreeNode *parent = (TreeNode *)current->value;
        Person *pData = (Person *)parent->value;
        printf("    %s (%d) has %zu children\n", 
               pData->name, pData->age, parent->numOfChildren);
        current = current->next;
    }
    
    // Cleanup
    TreeNode__del(gp);
    return 0;
}
```

### Example 3: File System Tree

```c
#include <stdio.h>
#include <string.h>
#include <cbarroso/tree.h>

typedef struct {
    char name[100];
    int isDirectory;
} FileNode;

void printTree(TreeNode *node, int depth) {
    FileNode *data = (FileNode *)node->value;
    
    // Print indentation
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    printf("%s %s\n", data->isDirectory ? "📁" : "📄", data->name);
    
    // Recursively print children
    SinglyLinkedListNode *current = node->childrenHead;
    while (current != NULL) {
        TreeNode *child = (TreeNode *)current->value;
        printTree(child, depth + 1);
        current = current->next;
    }
}

int main(void) {
    // Create file system structure
    FileNode root = {"root", 1};
    FileNode srcDir = {"src", 1};
    FileNode testsDir = {"tests", 1};
    FileNode mainFile = {"main.c", 0};
    FileNode utilsFile = {"utils.c", 0};
    FileNode testFile = {"test_main.c", 0};
    
    TreeNode *rootNode = TreeNode__new(&root, sizeof(FileNode));
    TreeNode *srcNode = TreeNode__new(&srcDir, sizeof(FileNode));
    TreeNode *testsNode = TreeNode__new(&testsDir, sizeof(FileNode));
    TreeNode *mainNode = TreeNode__new(&mainFile, sizeof(FileNode));
    TreeNode *utilsNode = TreeNode__new(&utilsFile, sizeof(FileNode));
    TreeNode *testNode = TreeNode__new(&testFile, sizeof(FileNode));
    
    // Build directory tree
    TreeNode__insert(rootNode, srcNode);
    TreeNode__insert(rootNode, testsNode);
    TreeNode__insert(srcNode, mainNode);
    TreeNode__insert(srcNode, utilsNode);
    TreeNode__insert(testsNode, testNode);
    
    // Print the tree
    printTree(rootNode, 0);
    
    // Cleanup
    TreeNode__del(rootNode);
    return 0;
}
```

## Use Cases

The Tree data structure is ideal for:

- **Abstract Syntax Trees (AST)** - Representing parsed code structure
- **File System Hierarchies** - Modeling directories and files
- **Organization Charts** - Representing hierarchical relationships
- **Decision Trees** - Implementing decision-making algorithms
- **DOM Trees** - Representing HTML/XML document structure
- **Game Object Hierarchies** - Managing parent-child relationships in game engines
- **Menu Systems** - Nested menu structures

## Performance Characteristics

- **Node Creation**: O(1)
- **Insert Child**: O(1) - children are added to the tail
- **Access Children**: O(n) where n is the number of children
- **Delete Tree**: O(n) where n is the total number of nodes
- **Memory**: O(n) where n is the total number of nodes

## Notes

- Each node's children are stored in a singly linked list, maintaining insertion order
- The tree supports arbitrary depth and arbitrary number of children per node
- Deletion is recursive and handles the entire subtree
- The implementation is thread-unsafe; external synchronization is required for concurrent access

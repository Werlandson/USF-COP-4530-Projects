/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: Tree ADT implemented using a singly linked list with helper functions for printing ancestor, descendant, and sibling
 * relationships, tree type, size, number of nodes, number of internal nodes, and number of external nodes.
 * */
#pragma once

#include "LinkedList.h"

#include <string>
#include <unordered_map>
#include <vector>

// Tree implemented using a singly linked list
class LinkedTree
{
public:
    // Tree node with edge labels and a linked list of children
    class TreeNode
    {
    public:
        std::string content;
        std::string incomingEdgeLabel;
        TreeNode *parent;
        LinkedList<TreeNode *> children;
        int preorderPosition;

        // Initializes TreeNode properties in LinkedTree
        TreeNode(const std::string &value, TreeNode *parentNode, const std::string &edgeLabel, int position);
    };

    // Linked tree constructor
    LinkedTree();
    // Linked tree destructor
    ~LinkedTree();

    // Prevents copying of LinkedTrees
    LinkedTree(const LinkedTree &) = delete;
    LinkedTree &operator=(const LinkedTree &) = delete;

    // Clears all nodes and resets the tree
    void clear();

    // Rreates or replaces the root node
    TreeNode *setRoot(const std::string &content, int preorderPosition = 0);

    // Adds a new child node under the provided parent
    TreeNode *addChild(TreeNode *parent, const std::string &edgeLabel, const std::string &content, int preorderPosition);

    // Retrieves the root node pointer
    const TreeNode *root() const;

    // Looks up a node by its preorder position from the input file
    const TreeNode *nodeByPosition(int preorderPosition) const;

    // Traversal helper function
    std::vector<const TreeNode *> preorderNodes() const;

    // Returns tree size
    int size() const;
    // Returns tree height
    int height() const;
    // Returns number of internal nodes
    int internalNodeCount() const;
    // Returns number of external nodes
    int externalNodeCount() const;

    // Returns first valid ancestor of given node
    const TreeNode *oneAncestor(const TreeNode *node) const;
    // Returns first valid descendant of given node
    const TreeNode *oneDescendant(const TreeNode *node) const;
    // Returns first valid sibling of given node
    const TreeNode *oneSibling(const TreeNode *node) const;

    // Returns boolean indicating whether the tree is binary
    bool isBinaryTree() const;
    // Returns boolean indicating whether the tree is proper
    bool isProperBinaryTree() const;
    // Returns boolean indicating whether the tree is perfect
    bool isPerfectBinaryTree() const;
    // Returns boolean indicating whether the tree is balanced
    bool isBalancedBinaryTree() const;

private:
    TreeNode *rootNode_;
    int nodeCount_;
    std::unordered_map<int, TreeNode *> nodesByPosition_;

    // Recursively deletes a subtree rooted at the given node
    void deleteSubtree(TreeNode *node);

    // Performs recursive preorder traversal and appends nodes to output
    void preorderConst(const TreeNode *node, std::vector<const TreeNode *> &out) const;

    // Returns subtree height where leaf height is 0 and empty is -1
    int height(const TreeNode *node) const;

    // Counts non-leaf nodes in the subtree
    int internalCount(const TreeNode *node) const;

    // Counts leaf nodes in the subtree
    int externalCount(const TreeNode *node) const;

    // Checks whether every node in the subtree has at most two children
    bool isBinary(const TreeNode *node) const;

    // Checks whether every internal node has exactly two children
    bool isProper(const TreeNode *node) const;

    // Verifies that all leaves appear at the same depth
    bool leavesSameDepth(const TreeNode *node, int depth, int &expectedLeafDepth) const;

    // Computes height while validating subtree balance condition
    bool balancedHeight(const TreeNode *node, int &outHeight) const;
};

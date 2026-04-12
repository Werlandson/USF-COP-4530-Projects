/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: Tree ADT implemented using a singly linked list with helper functions for printing ancestor, descendant, and sibling
 * relationships, tree type, size, number of nodes, number of internal nodes, and number of external nodes.
 * */
#include "LinkedTree.h"

#include <algorithm>
#include <cmath>

// Initializes TreeNode properties in LinkedTree
LinkedTree::TreeNode::TreeNode(
    const std::string &value,
    TreeNode *parentNode,
    const std::string &edgeLabel,
    int position)
    : content(value), incomingEdgeLabel(edgeLabel), parent(parentNode), preorderPosition(position) {}

// Linked tree constructor
LinkedTree::LinkedTree() : rootNode_(nullptr), nodeCount_(0) {}
// Linked tree destructor
LinkedTree::~LinkedTree()
{
    clear();
}

// Clears all nodes and resets the tree
void LinkedTree::clear()
{
    // Clears nodes in linked tree and preorder mappings in nodesByPosition
    deleteSubtree(rootNode_);
    rootNode_ = nullptr;
    nodeCount_ = 0;
    nodesByPosition_.clear();
}

// Creates or replaces the root node
LinkedTree::TreeNode *LinkedTree::setRoot(const std::string &content, int preorderPosition)
{
    // Clears tree and updates with new node
    clear();
    rootNode_ = new TreeNode(content, nullptr, "", preorderPosition);
    nodeCount_ = 1;
    nodesByPosition_[preorderPosition] = rootNode_;
    return rootNode_;
}

// Adds a new child node under the provided parent
LinkedTree::TreeNode *LinkedTree::addChild(
    TreeNode *parent,
    const std::string &edgeLabel,
    const std::string &content,
    int preorderPosition)
{
    // If parent does not exist, return to avoid remaining logic
    if (parent == nullptr)
    {
        return nullptr;
    }

    TreeNode *child = new TreeNode(content, parent, edgeLabel, preorderPosition);
    parent->children.push_back(child);
    ++nodeCount_;
    nodesByPosition_[preorderPosition] = child;
    return child;
}

// Retrieves the root node pointer
const LinkedTree::TreeNode *LinkedTree::root() const
{
    return rootNode_;
}

// Looks up a node by its preorder position from the input file
const LinkedTree::TreeNode *LinkedTree::nodeByPosition(int preorderPosition) const
{
    // Finds node by preorder position
    const auto it = nodesByPosition_.find(preorderPosition);

    // If node isn't found, return nullptr
    if (it == nodesByPosition_.end())
    {
        return nullptr;
    }
    return it->second;
}

// Traversal helper function
std::vector<const LinkedTree::TreeNode *> LinkedTree::preorderNodes() const
{
    std::vector<const TreeNode *> nodes;
    preorderConst(rootNode_, nodes);
    return nodes;
}

// Returns tree size
int LinkedTree::size() const
{
    return nodeCount_;
}

// Returns tree height
int LinkedTree::height() const
{
    return height(rootNode_);
}

// Returns number of internal nodes
int LinkedTree::internalNodeCount() const
{
    return internalCount(rootNode_);
}

// Returns number of external nodes
int LinkedTree::externalNodeCount() const
{
    return externalCount(rootNode_);
}

// Returns first valid ancestor of given node
const LinkedTree::TreeNode *LinkedTree::oneAncestor(const TreeNode *node) const
{
    // Prevents attempted accesses on a nullptr
    if (node == nullptr)
    {
        return nullptr;
    }
    return node->parent;
}

// Returns first valid descendant of given node
const LinkedTree::TreeNode *LinkedTree::oneDescendant(const TreeNode *node) const
{
    // Prevents attempted accesses on a nullptr
    if (node == nullptr || node->children.empty())
    {
        return nullptr;
    }
    const Node<TreeNode *> *head = node->children.getHead();
    return head == nullptr ? nullptr : head->elem;
}

// Returns first valid sibling of given node
const LinkedTree::TreeNode *LinkedTree::oneSibling(const TreeNode *node) const
{
    // Prevents accesses on a nullptr or an attempted access to the sibling of a root node
    if (node == nullptr || node->parent == nullptr)
    {
        return nullptr;
    }
    const LinkedList<TreeNode *> &siblings = node->parent->children;
    // Returns first valid sibling
    for (const Node<TreeNode *> *it = siblings.getHead(); it != nullptr; it = it->next)
    {
        const TreeNode *sibling = it->elem;
        if (sibling != node)
        {
            return sibling;
        }
    }
    return nullptr;
}

// Returns boolean indicating whether the tree is binary
bool LinkedTree::isBinaryTree() const
{
    return isBinary(rootNode_);
}

// Returns boolean indicating whether the tree is proper
bool LinkedTree::isProperBinaryTree() const
{
    // Checks if it is a binary tree before checking if it is a proper tree
    if (!isBinaryTree())
    {
        return false;
    }
    return isProper(rootNode_);
}

// Returns boolean indicating whether the tree is perfect
bool LinkedTree::isPerfectBinaryTree() const
{
    // Checks if it is a binary tree before checking if it is a perfect binary tree
    if (!isBinaryTree())
    {
        return false;
    }
    int expectedLeafDepth = -1;
    return leavesSameDepth(rootNode_, 0, expectedLeafDepth) && isProper(rootNode_);
}

// Returns boolean indicating whether the tree is balanced
bool LinkedTree::isBalancedBinaryTree() const
{
    // Checks if it is a binary tree before checking if it is a balanced binary tree
    if (!isBinaryTree())
    {
        return false;
    }
    int computedHeight = 0;
    return balancedHeight(rootNode_, computedHeight);
}

// Recursively deletes a subtree rooted at the given node
void LinkedTree::deleteSubtree(TreeNode *node)
{
    if (node == nullptr)
    {
        return;
    }
    // Recursively calls deleteSubtree until it reaches an external node, upon which it begins deleting nodes
    for (const Node<TreeNode *> *it = node->children.getHead(); it != nullptr; it = it->next)
    {
        deleteSubtree(it->elem);
    }
    delete node;
}

// Performs recursive preorder traversal and appends nodes to output
void LinkedTree::preorderConst(const TreeNode *node, std::vector<const TreeNode *> &out) const
{
    if (node == nullptr)
    {
        return;
    }
    // Adds parent node to preorder vector before visiting children
    out.push_back(node);
    for (const Node<TreeNode *> *it = node->children.getHead(); it != nullptr; it = it->next)
    {
        preorderConst(it->elem, out);
    }
}

// Returns subtree height where leaf height is 0 and empty is -1
int LinkedTree::height(const TreeNode *node) const
{
    if (node == nullptr)
    {
        return -1;
    }
    if (node->children.empty())
    {
        return 0;
    }
    int maxChildHeight = 0;
    // Finds the node by iterating through each node, adding one to the max height for each recursive call, and taking the max depth of each path
    for (const Node<TreeNode *> *it = node->children.getHead(); it != nullptr; it = it->next)
    {
        maxChildHeight = std::max(maxChildHeight, height(it->elem));
    }
    return maxChildHeight + 1;
}

// Counts non-leaf nodes in the subtree
int LinkedTree::internalCount(const TreeNode *node) const
{
    // If tree points to nullptr or children is empty, return 0
    if (node == nullptr || node->children.empty())
    {
        return 0;
    }
    // Otherwise, the node is internal
    int count = 1;
    // Recursively call internalCount for each child of the node
    for (const Node<TreeNode *> *it = node->children.getHead(); it != nullptr; it = it->next)
    {
        count += internalCount(it->elem);
    }
    return count;
}

// Counts leaf nodes in the subtree
int LinkedTree::externalCount(const TreeNode *node) const
{
    if (node == nullptr)
    {
        return 0;
    }
    // If there are no children, the node is external
    if (node->children.empty())
    {
        return 1;
    }
    int count = 0;
    // Recursively call externalCount for each node
    for (const Node<TreeNode *> *it = node->children.getHead(); it != nullptr; it = it->next)
    {
        count += externalCount(it->elem);
    }
    return count;
}

// Checks whether every node in the subtree has at most two children
bool LinkedTree::isBinary(const TreeNode *node) const
{
    if (node == nullptr)
    {
        return true;
    }
    // If node has more than two children, return false
    if (node->children.getSize() > 2)
    {
        return false;
    }
    // Recursively call isBinary for every child of every node
    for (const Node<TreeNode *> *it = node->children.getHead(); it != nullptr; it = it->next)
    {
        if (!isBinary(it->elem))
        {
            return false;
        }
    }
    // If each node does not have more than 2 children, the tree is binary
    return true;
}

// Checks whether every internal node has exactly two children
bool LinkedTree::isProper(const TreeNode *node) const
{
    if (node == nullptr)
    {
        return true;
    }
    // If node does not have 0 or 2 children, return false
    if (!node->children.empty() && node->children.getSize() != 2)
    {
        return false;
    }
    // Recursively call isProper for every child of every node
    for (const Node<TreeNode *> *it = node->children.getHead(); it != nullptr; it = it->next)
    {
        if (!isProper(it->elem))
        {
            return false;
        }
    }
    // If each node has 0 or 2 children, the tree is proper
    return true;
}

// Verifies that all leaves appear at the same depth
bool LinkedTree::leavesSameDepth(const TreeNode *node, int depth, int &expectedLeafDepth) const
{
    if (node == nullptr)
    {
        return true;
    }
    if (node->children.empty())
    {
        // Upon reaching the first external node, set expectedLeafDepth to depth
        if (expectedLeafDepth == -1)
        {
            expectedLeafDepth = depth;
            return true;
        }
        // For subsequent children, return whether depth equals expectedLeafDepth
        return expectedLeafDepth == depth;
    }
    for (const Node<TreeNode *> *it = node->children.getHead(); it != nullptr; it = it->next)
    {
        // Iterate depth by 1 for each recursive call
        if (!leavesSameDepth(it->elem, depth + 1, expectedLeafDepth))
        {
            return false;
        }
    }
    return true;
}

// Computes height while validating subtree balance condition
bool LinkedTree::balancedHeight(const TreeNode *node, int &outHeight) const
{
    if (node == nullptr)
    {
        outHeight = -1;
        return true;
    }
    // If there are no children, checks are complete for the given subtree
    if (node->children.empty())
    {
        outHeight = 0;
        return true;
    }

    // Will hold recursive heights of first and second child (binary: at most two)
    int leftHeight = -1;
    int rightHeight = -1;

    const Node<TreeNode *> *head = node->children.getHead();
    const TreeNode *firstChild = head == nullptr ? nullptr : head->elem;
    // If their is no firstChild or the left subtree is imbalanced, return false
    if (firstChild == nullptr || !balancedHeight(firstChild, leftHeight))
    {
        return false;
    }
    // If there are two children, repeat the check for right subtree
    if (node->children.getSize() == 2)
    {
        const Node<TreeNode *> *second = (head == nullptr) ? nullptr : head->next;
        const TreeNode *secondChild = second == nullptr ? nullptr : second->elem;
        if (secondChild == nullptr || !balancedHeight(secondChild, rightHeight))
        {
            return false;
        }
    }

    // If there is only one child, treat the missing side as height -1 so |left - right| <= 1 can be checked
    if (node->children.getSize() == 1)
    {
        rightHeight = -1;
    }

    // If the heights of the two sides differ by more than one, the tree is imbalanced
    if (std::abs(leftHeight - rightHeight) > 1)
    {
        return false;
    }

    // This node's height is one more than the taller subtree
    outHeight = std::max(leftHeight, rightHeight) + 1;
    return true;
}

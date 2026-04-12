/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Descriptions: Writes information about a tree to the given file path according to the project description format.
 * */
#include "TreeReport.h"

#include <fstream>
#include <ostream>
#include <sstream>

// Writes a complete tree report to the given output path
bool TreeReport::writeAboutTreeFile(const LinkedTree &tree, const std::string &outputPath, std::string &errorMessage)
{
    // Opens file for writing
    std::ofstream outputFile(outputPath.c_str(), std::ios::trunc);
    // If there is an error opening the file, terminates execution
    if (!outputFile.is_open())
    {
        errorMessage = "Could not open output file: " + outputPath;
        return false;
    }

    // Outputs tree information according to the project description
    outputFile << "Tree Visualization\n";
    writeVisualization(tree.root(), 0, outputFile);
    outputFile << "\n";

    outputFile << "Tree Properties\n";
    outputFile << "Root node: " << (tree.root() == nullptr ? "None" : tree.root()->content) << "\n";
    outputFile << "Number of internal nodes: " << tree.internalNodeCount() << "\n";
    outputFile << "Number of external (leaf) nodes: " << tree.externalNodeCount() << "\n";
    outputFile << "Tree height: " << tree.height() << "\n";

    const std::vector<const LinkedTree::TreeNode *> nodes = tree.preorderNodes();
    outputFile << "Internal nodes (preorder): " << joinNodeContents(nodes, false) << "\n";
    outputFile << "External nodes (preorder): " << joinNodeContents(nodes, true) << "\n";
    outputFile << "\n";

    outputFile << "Binary Tree Analysis\n";
    const bool binary = tree.isBinaryTree();
    outputFile << "Is it a Binary Tree? " << yesNo(binary) << "\n";
    if (binary)
    {
        outputFile << "Proper Tree: " << yesNo(tree.isProperBinaryTree()) << "\n";
        outputFile << "Perfect Tree: " << yesNo(tree.isPerfectBinaryTree()) << "\n";
        outputFile << "Balanced Tree: " << yesNo(tree.isBalancedBinaryTree()) << "\n";
    }

    return true;
}

// Prints tree structure according to project description
void TreeReport::writeVisualization(const LinkedTree::TreeNode *node, int level, std::ostream &output)
{
    if (node == nullptr)
    {
        return;
    }

    // Prints information according to node level
    if (level == 0)
    {
        output << node->content << "\n";
    }
    else
    {
        output << std::string(level * 2, '-') << "[" << node->incomingEdgeLabel << "] " << node->content << "\n";
    }

    // Recursively calls function for each node
    for (const Node<LinkedTree::TreeNode *> *it = node->children.getHead(); it != nullptr; it = it->next)
    {
        writeVisualization(it->elem, level + 1, output);
    }
}

// Maps true/false to yes/no
std::string TreeReport::yesNo(bool value)
{
    return value ? "Yes" : "No";
}

// Builds a comma-separated list of node contents from preorder order
std::string TreeReport::joinNodeContents(const std::vector<const LinkedTree::TreeNode *> &nodes, bool externalOnly)
{
    // Stream that accumulates the comma-separated output string
    std::ostringstream joined;
    // True until the first matching node is appended (controls comma placement)
    bool first = true;
    // Walk preorder vector in order
    for (std::vector<const LinkedTree::TreeNode *>::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        const LinkedTree::TreeNode *node = *it;
        // Leaf has no children in the linked child list
        const bool isExternal = node->children.empty();
        // Skip if user only wants leaves but this is internal, or only internal but this is a leaf
        if ((externalOnly && !isExternal) || (!externalOnly && isExternal))
        {
            continue;
        }

        // Insert comma before every node after the first included one
        if (!first)
        {
            joined << ", ";
        }
        // Append this node's display text
        joined << node->content;
        first = false;
    }
    // No nodes matched the filter: assignment requires the literal "None"
    return first ? "None" : joined.str();
}

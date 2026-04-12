/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Descriptions: Writes information about a tree to the given file path according to the project description format.
 * */
#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "LinkedTree.h"

// Writes information about a given tree in the given output file name
class TreeReport
{
public:
    // Writes a complete tree report to the given output path
    static bool writeAboutTreeFile(const LinkedTree &tree, const std::string &outputPath, std::string &errorMessage);

private:
    // Prints tree structure according to project description
    static void writeVisualization(const LinkedTree::TreeNode *node, int level, std::ostream &output);
    // Maps true/false to yes/no
    static std::string yesNo(bool value);
    // Builds a comma-separated list of node contents from preorder order
    static std::string joinNodeContents(const std::vector<const LinkedTree::TreeNode *> &nodes, bool externalOnly);
};

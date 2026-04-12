/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: Reads a formatted .txt file into a LinkedTree data structure. Includes helper functions for
 * formatting and organization.
 * */
#include "TreeLoader.h"

#include <fstream>
#include <map>
#include <sstream>

// Reads a formatted tree file and builds the linked tree structure
bool TreeLoader::loadFromFile(const std::string &inputPath, LinkedTree &tree, std::string &errorMessage)
{
    // Open file at given path for reading
    std::ifstream inputFile(inputPath.c_str());
    // If open fails, report and stop
    if (!inputFile.is_open())
    {
        errorMessage = "Could not open input file: " + inputPath;
        return false;
    }

    // Discard any previous tree state before loading
    tree.clear();

    // Read first line as root text
    std::string rootLine;
    // If there is no first line, the file is empty
    if (!std::getline(inputFile, rootLine))
    {
        errorMessage = "Input file is empty.";
        return false;
    }

    // Strip surrounding whitespace from root line
    const std::string rootContent = trim(rootLine);
    // Root must not be blank after trim
    if (rootContent.empty())
    {
        errorMessage = "Root node content cannot be empty.";
        return false;
    }

    // Build root at preorder position 0; map level 0 to that node for linking children
    LinkedTree::TreeNode *rootNode = tree.setRoot(rootContent, 0);
    std::map<int, LinkedTree::TreeNode *> latestNodeAtLevel;
    latestNodeAtLevel[0] = rootNode;

    // Process each remaining line as a non-root node
    std::string line;
    int lineNumber = 1;
    // Read line-by-line until the end of the file
    while (std::getline(inputFile, line))
    {
        ++lineNumber;
        // Skip blank lines without failing
        if (trim(line).empty())
        {
            continue;
        }

        // Fields from file: level, preorder position, edge label, node text.
        std::string levelText;
        std::string positionText;
        std::string edgeLabel;
        std::string nodeContent;
        // Line must contain exactly four tab-separated fields.
        if (!splitTabFields(line, levelText, positionText, edgeLabel, nodeContent))
        {
            std::ostringstream builder;
            builder << "Invalid format on line " << lineNumber << ". Expected 4 tab-separated fields.";
            errorMessage = builder.str();
            return false;
        }

        // Trim each field for reliable parsing and display
        const std::string levelTrimmed = trim(levelText);
        const std::string positionTrimmed = trim(positionText);
        const std::string edgeTrimmed = trim(edgeLabel);
        const std::string contentTrimmed = trim(nodeContent);
        // Level, position, and content are required; edge may be empty
        if (levelTrimmed.empty() || positionTrimmed.empty() || contentTrimmed.empty())
        {
            std::ostringstream builder;
            builder << "Missing required field on line " << lineNumber << ".";
            errorMessage = builder.str();
            return false;
        }

        // Parse level and preorder position as integers
        int level = -1;
        int preorderPosition = -1;
        std::istringstream levelReader(levelTrimmed);
        std::istringstream positionReader(positionTrimmed);
        levelReader >> level;
        positionReader >> preorderPosition;
        // Level must be at least 1 (non-root); position must be non-negative; parse must consume field
        if (!levelReader || !positionReader || level < 1 || preorderPosition < 0)
        {
            std::ostringstream builder;
            builder << "Invalid numeric data on line " << lineNumber << ".";
            errorMessage = builder.str();
            return false;
        }

        // Parent is the most recent node at level - 1 (path from root)
        const std::map<int, LinkedTree::TreeNode *>::iterator parentIt = latestNodeAtLevel.find(level - 1);
        // If there is no parent at expected level, the file is invalid or out-of-order
        if (parentIt == latestNodeAtLevel.end())
        {
            std::ostringstream builder;
            builder << "Could not find parent for node on line " << lineNumber << ".";
            errorMessage = builder.str();
            return false;
        }

        // Attach new node under parent with edge label and preorder id
        LinkedTree::TreeNode *parentNode = parentIt->second;
        LinkedTree::TreeNode *addedNode = tree.addChild(parentNode, edgeTrimmed, contentTrimmed, preorderPosition);
        if (addedNode == nullptr)
        {
            std::ostringstream builder;
            builder << "Failed to add node on line " << lineNumber << ".";
            errorMessage = builder.str();
            return false;
        }

        // This node is now the rightmost node at its depth for future children
        latestNodeAtLevel[level] = addedNode;

        // Remove deeper levels so sibling subtrees do not reuse stale parents
        std::map<int, LinkedTree::TreeNode *>::iterator cleanupIt = latestNodeAtLevel.upper_bound(level);
        while (cleanupIt != latestNodeAtLevel.end())
        {
            std::map<int, LinkedTree::TreeNode *>::iterator toErase = cleanupIt;
            ++cleanupIt;
            latestNodeAtLevel.erase(toErase);
        }
    }

    return true;
}

// Extracts content information given the location of each tab
bool TreeLoader::splitTabFields(const std::string &line, std::string &level, std::string &position,
                                std::string &edgeLabel, std::string &nodeContent)
{
    // Find first tab
    const std::size_t firstTab = line.find('\t');
    if (firstTab == std::string::npos)
    {
        return false;
    }

    // Find second tab
    const std::size_t secondTab = line.find('\t', firstTab + 1);
    if (secondTab == std::string::npos)
    {
        return false;
    }

    // Find third tab
    const std::size_t thirdTab = line.find('\t', secondTab + 1);
    if (thirdTab == std::string::npos)
    {
        return false;
    }

    // Extract content given location of each tab
    level = line.substr(0, firstTab);
    position = line.substr(firstTab + 1, secondTab - firstTab - 1);
    edgeLabel = line.substr(secondTab + 1, thirdTab - secondTab - 1);
    nodeContent = line.substr(thirdTab + 1);
    return true;
}

// Trims a string
std::string TreeLoader::trim(const std::string &value)
{
    // Advance start past leading whitespace
    std::size_t start = 0;
    while (start < value.size() && (value[start] == ' ' || value[start] == '\t' || value[start] == '\r'))
    {
        ++start;
    }

    // If it is all whitespace, value is an empty string
    if (start == value.size())
    {
        return "";
    }

    // Shrink end past trailing whitespace
    std::size_t end = value.size() - 1;
    while (end > start && (value[end] == ' ' || value[end] == '\t' || value[end] == '\r'))
    {
        --end;
    }

    return value.substr(start, end - start + 1);
}

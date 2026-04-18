/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: Reads a formatted .txt file into a LinkedTree data structure. Includes helper functions for
 * formatting and organization.
 * */
#include "TreeLoader.h"

#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

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

    // Parse first line as either plain root text or root record with level/position metadata
    std::string rootContent;
    int rootLevel = 0;
    int rootPosition = 0;
    if (!parseRootLine(rootLine, rootContent, rootLevel, rootPosition))
    {
        errorMessage = "Invalid root line format.";
        return false;
    }
    // Root must not be blank after parsing
    if (rootContent.empty())
    {
        errorMessage = "Root node content cannot be empty.";
        return false;
    }
    // For metadata-based files, root is expected at level 0
    if (rootLevel != 0)
    {
        errorMessage = "Root level must be 0.";
        return false;
    }

    // Build root and map level 0 to that node for linking children
    LinkedTree::TreeNode *rootNode = tree.setRoot(rootContent, rootPosition);
    std::map<int, LinkedTree::TreeNode *> latestNodeAtLevel;
    latestNodeAtLevel[0] = rootNode;

    // Represents one non-root row parsed from the file
    struct ParsedRow
    {
        int level;
        int preorderPosition;
        std::string edgeLabel;
        std::string content;
        int sourceLine;
    };

    // Track used preorder positions to prevent collisions with root/other nodes
    std::set<int> usedPositions;
    usedPositions.insert(rootPosition);

    // Parse each remaining line as a non-root node row
    std::vector<ParsedRow> parsedRows;
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

        // Fields from file: level, preorder position, edge label, node text
        std::string levelText;
        std::string positionText;
        std::string edgeLabel;
        std::string nodeContent;
        // Line must contain level, position, edge label, and node content
        if (!splitNodeFields(line, levelText, positionText, edgeLabel, nodeContent))
        {
            std::ostringstream builder;
            builder << "Invalid format on line " << lineNumber << ". Expected level, position, edge, and content.";
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

        // Position must be unique across entire tree
        if (usedPositions.find(preorderPosition) != usedPositions.end())
        {
            std::ostringstream builder;
            builder << "Duplicate preorder position on line " << lineNumber << ".";
            errorMessage = builder.str();
            return false;
        }
        usedPositions.insert(preorderPosition);

        ParsedRow row;
        row.level = level;
        row.preorderPosition = preorderPosition;
        row.edgeLabel = edgeTrimmed;
        row.content = contentTrimmed;
        row.sourceLine = lineNumber;
        parsedRows.push_back(row);
    }

    // Ensure rows are processed in preorder-position order even if file rows are shuffled
    std::sort(parsedRows.begin(), parsedRows.end(), [](const ParsedRow &a, const ParsedRow &b)
              { return a.preorderPosition < b.preorderPosition; });

    // Build actual links after rows are ordered by preorder position
    for (std::vector<ParsedRow>::const_iterator it = parsedRows.begin(); it != parsedRows.end(); ++it)
    {
        const ParsedRow &row = *it;
        // Parent is the most recent node at level - 1
        const std::map<int, LinkedTree::TreeNode *>::iterator parentIt = latestNodeAtLevel.find(row.level - 1);
        // If there is no parent at expected level, row hierarchy is invalid
        if (parentIt == latestNodeAtLevel.end())
        {
            std::ostringstream builder;
            builder << "Could not find parent for node on line " << row.sourceLine << ".";
            errorMessage = builder.str();
            return false;
        }

        // Attach node under parent with stored edge label and content
        LinkedTree::TreeNode *parentNode = parentIt->second;
        LinkedTree::TreeNode *addedNode = tree.addChild(parentNode, row.edgeLabel, row.content, row.preorderPosition);
        if (addedNode == nullptr)
        {
            std::ostringstream builder;
            builder << "Failed to add node on line " << row.sourceLine << ".";
            errorMessage = builder.str();
            return false;
        }

        // Current row becomes the latest node at its depth
        latestNodeAtLevel[row.level] = addedNode;
        // Remove deeper levels so sibling subtrees do not reuse stale parents
        std::map<int, LinkedTree::TreeNode *>::iterator cleanupIt = latestNodeAtLevel.upper_bound(row.level);
        while (cleanupIt != latestNodeAtLevel.end())
        {
            std::map<int, LinkedTree::TreeNode *>::iterator toErase = cleanupIt;
            ++cleanupIt;
            latestNodeAtLevel.erase(toErase);
        }
    }

    return true;
}

// Parses the first line, supporting project description and test file formats
bool TreeLoader::parseRootLine(const std::string &line, std::string &rootContent, int &rootLevel, int &rootPosition)
{
    // Trim root line to normalize parsing behavior
    const std::string trimmedLine = trim(line);
    // Empty root line is invalid
    if (trimmedLine.empty())
    {
        return false;
    }

    // Attempt metadata-style root format: level position root-content
    std::istringstream parser(trimmedLine);
    int parsedLevel = -1;
    int parsedPosition = -1;
    if (parser >> parsedLevel >> parsedPosition)
    {
        // Remaining text after two integers is the root question/content
        std::string remainder;
        std::getline(parser, remainder);
        const std::string trimmedRemainder = trim(remainder);
        if (!trimmedRemainder.empty())
        {
            rootLevel = parsedLevel;
            rootPosition = parsedPosition;
            rootContent = trimmedRemainder;
            return true;
        }
    }

    // Fallback format: entire line is root content with default level/position
    rootLevel = 0;
    rootPosition = 0;
    rootContent = trimmedLine;
    return true;
}

// Extracts node fields from either tab-separated or space-separated lines
bool TreeLoader::splitNodeFields(const std::string &line, std::string &level, std::string &position,
                                 std::string &edgeLabel, std::string &nodeContent)
{
    // Handle tab-separated rows: level<TAB>position<TAB>edge<TAB>content
    const std::size_t firstTab = line.find('\t');
    if (firstTab != std::string::npos)
    {
        const std::size_t secondTab = line.find('\t', firstTab + 1);
        if (secondTab == std::string::npos)
        {
            return false;
        }

        const std::size_t thirdTab = line.find('\t', secondTab + 1);
        if (thirdTab == std::string::npos)
        {
            return false;
        }

        level = line.substr(0, firstTab);
        position = line.substr(firstTab + 1, secondTab - firstTab - 1);
        edgeLabel = line.substr(secondTab + 1, thirdTab - secondTab - 1);
        nodeContent = line.substr(thirdTab + 1);
        return true;
    }

    // Handle space-separated rows: level position edge content-with-spaces
    std::istringstream parser(line);
    if (!(parser >> level >> position >> edgeLabel))
    {
        return false;
    }

    std::string remainder;
    std::getline(parser, remainder);
    nodeContent = trim(remainder);
    if (nodeContent.empty())
    {
        return false;
    }
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

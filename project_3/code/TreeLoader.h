/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: Reads a formatted .txt file into a LinkedTree data structure. Includes helper functions for
 * formatting and organization.
 * */
#pragma once

#include <string>

#include "LinkedTree.h"

// Reads data from a formatted .txt file into a LinkedTree
class TreeLoader
{
public:
    // Reads a formatted tree file and builds the linked tree structure
    static bool loadFromFile(const std::string &inputPath, LinkedTree &tree, std::string &errorMessage);

private:
    // Extracts content information given the location of each tab
    static bool splitTabFields(const std::string &line, std::string &level, std::string &position,
                               std::string &edgeLabel, std::string &nodeContent);
    // Trims a string
    static std::string trim(const std::string &value);
};

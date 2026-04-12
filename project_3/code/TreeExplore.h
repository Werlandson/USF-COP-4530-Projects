/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: Allows interaction with a LinkedTree. The user enters a tree position, and the program
 * prints node content, ancestor, descendant, and sibling information.
 * */
#pragma once

#include "LinkedTree.h"

// Allows interaction with LinkedTree data structure
class TreeExplore
{
public:
    // Starts an interactive prompt for node exploration.
    static void run(const LinkedTree &tree);
};

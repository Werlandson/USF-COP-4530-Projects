/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: Allows interaction with a LinkedTree. The user enters a tree position, and the program
 * prints node content, ancestor, descendant, and sibling information.
 * */
#include "TreeExplore.h"
#include "LinkedTree.h"

#include <iostream>
#include <sstream>
#include <string>

// Starts an interactive prompt for node exploration.
void TreeExplore::run(const LinkedTree &tree)
{
    std::string input;
    // Prompts user until they enter exit
    while (true)
    {
        std::cout << "Which node would you like to explore (enter position or \"exit\"): ";
        if (!std::getline(std::cin, input))
        {
            std::cout << "Goodbye!" << std::endl;
            return;
        }
        // If user enters exit, print "Goodbye!" and terminate execution
        if (input == "exit")
        {
            std::cout << "Goodbye!" << std::endl;
            return;
        }
        // Parses input string
        std::istringstream parser(input);
        int position = -1;
        parser >> position;
        // Ensures input is valid
        if (!parser || !parser.eof())
        {
            std::cout << "Invalid input. Please try again." << std::endl;
            continue;
        }

        const LinkedTree::TreeNode *node = tree.nodeByPosition(position);
        // If node does not exist, informs user of error
        if (node == nullptr)
        {
            std::cout << "Invalid input. Please try again." << std::endl;
            continue;
        }

        const LinkedTree::TreeNode *ancestor = tree.oneAncestor(node);
        const LinkedTree::TreeNode *descendant = tree.oneDescendant(node);
        const LinkedTree::TreeNode *sibling = tree.oneSibling(node);

        std::cout << "Node's content: " << node->content << std::endl;
        std::cout << "Ancestor: " << (ancestor == nullptr ? "None" : ancestor->content) << std::endl;
        std::cout << "Descendant: " << (descendant == nullptr ? "None" : descendant->content) << std::endl;
        std::cout << "Sibling: " << (sibling == nullptr ? "None" : sibling->content) << std::endl;
    }
}

/* Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: File responsible for execution. Initializes file path according to command line arguments or user input.
 * Reads tree .txt file given path name, writes information about the tree to about_tree.txt, and allows the user to
 * print information about the loaded tree through interaction with the program */
#include <iostream>
#include <string>

#include "LinkedTree.h"
#include "TreeExplore.h"
#include "TreeLoader.h"
#include "TreeReport.h"

int main(int argc, char *argv[])
{
    std::string inputPath;
    // If there is a .txt command line argument, assign it to input path
    if (argc >= 2)
    {
        inputPath = argv[1];
        // Otherwise, prompt the user for file path name
    }
    else
    {
        std::cout << "Enter input file path: ";
        std::getline(std::cin, inputPath);
    }

    LinkedTree tree;
    std::string errorMessage;
    // Print error if tree fails to load
    if (!TreeLoader::loadFromFile(inputPath, tree, errorMessage))
    {
        std::cerr << errorMessage << std::endl;
        return 1;
    }
    // Print error if program fails to write the tree
    if (!TreeReport::writeAboutTreeFile(tree, "about_tree.txt", errorMessage))
    {
        std::cerr << errorMessage << std::endl;
        return 1;
    }

    TreeExplore::run(tree);
    return 0;
}

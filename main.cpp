#include <iostream>
#include "AVL.h"
#include <sstream>

// Prints tree all nice and pretty
// Adrian Schneider - https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
// since we can only print right and down, we present the binary tree horizontally
// I cannot take full credit for this implementation
std::string printTree(AVL* node, std::ostringstream& oss,std::string prefix, bool bIsLeft)
{
    if(node != nullptr)
    {
        oss << prefix << (bIsLeft ? "├──" : "└──" );
        oss << node->data << '\n';

        printTree(node->left,oss,prefix + (bIsLeft ? "│   " : "    "), true);
        printTree(node->right,oss,prefix + (bIsLeft ? "│   " : "    "), false);
    }
    return oss.str();
}

std::string printTree(AVL* node)
{
    std::ostringstream oss;
    return printTree(node,oss,"",false);
}

int main()
{
	AVL* tree = new AVL(50);

	tree = tree->insertNode(tree, 30);
	tree = tree->insertNode(tree, 20);
	tree = tree->insertNode(tree, 40);
	tree = tree->insertNode(tree, 45);

	std::cout << printTree(tree);
	std::cout << "\nHEIGHT OF tree: " << tree->getHeight(tree) << std::endl;

	return 0;
}


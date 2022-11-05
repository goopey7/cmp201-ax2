#include <iostream>
#include "AVL.h"
#include <sstream>

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

	std::cout << printTree(tree);
	std::cout << "\nHEIGHT OF tree: " << tree->getHeight(tree) << std::endl;

	return 0;
}


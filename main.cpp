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
	AVL tree(50);
	tree.insertNode(&tree,20);
	tree.insertNode(&tree,15);
	tree.insertNode(&tree,30);
	tree.insertNode(&tree,26);
	tree.insertNode(&tree,24);
	tree.insertNode(&tree,25);
	tree.insertNode(&tree,27);
	tree.insertNode(&tree,40);
	tree.insertNode(&tree,45);
	tree.insertNode(&tree,145);


    tree.deleteNode(&tree,30);
	std::cout << printTree(&tree);


	return 0;
}


#include "AVL.h"
#include <sstream>

/*
std::string printTree(AVL* node, std::ostringstream& oss,std::string prefix, bool bIsLeft)
{
    if(node != nullptr)
    {
        oss << (bIsLeft ? "├──" : "└──" );
        oss << node->data << '\n';

        printTree(node,oss,prefix + (bIsLeft ? "│   " : "    "), true);
        printTree(node,oss,prefix + (bIsLeft ? "│   " : "    "), false);
    }
    return oss.str();
}

std::string printTree(AVL* node)
{
    std::ostringstream oss;
    return printTree(node,oss,"",false);
}
*/

void AVL::preOrder(AVL* r)
{
}

void AVL::inOrder(AVL* r)
{
}

int AVL::getBalance(AVL* r)
{
	return	0;
}

int AVL::getHeight(AVL* r)
{
	return	0;
}

AVL* AVL::insertNode(AVL* r, int key)
{
	//TODO REBALANCE AFTER INSERTION
    if(r == nullptr)
        return nullptr;

	if(r->data == key)
	{
		return nullptr;
	}
	else if(key < r->data)
	{
        if(r->left != nullptr)
        {
            return insertNode(r->left,key);
        }
        else
        {
            r->left = new AVL(key);
            return r->left;
        }
	}
	else
	{
        if(r->right != nullptr)
        {
            return insertNode(r->right,key);
        }
        else
        {
            r->right = new AVL(key);
            return r->right;
        }
	}
}

/*
// returns an int instead of a reference because this deletes the successor node for us
// all in one fell swoop
int findSuccessor(AVL* node)
{
    if(node->left == nullptr)
    {
        int successor = node->data;
        node->deleteNode(node,successor);
        return successor;
    }
    return findSuccessor(node->left);
}
*/

AVL* findSuccessor(AVL* node)
{
    if(node->left == nullptr)
    {
        return node;
    }
    return findSuccessor(node->left);
}

AVL* AVL::deleteNode(AVL* r, int key)
{
    // TODO REBALANCE AFTER DELETION

    // base case we hit a leaf, so not found, return nothing
    if(r == nullptr)
    {
        return nullptr;
    }

    if(key < r->data) // key is smaller so look left
    {
        return deleteNode(r->left,key);
    }
    else if(key > r->data) // key is larger so look right
    {
        return deleteNode(r->right,key);
    }
    else // key matches. We want to delete this node
    {
        // Case 1: Deleting a leaf node
        if(r->left == nullptr && r->right == nullptr)
        {
            delete r;
            return nullptr;
        }
        // Case 2: Deleting a node with a single child
        else if(r->left == nullptr || r->right == nullptr)
        {
            AVL* child = r->left == nullptr ? r->right : r->left;
            delete r;
            return child;
        }
        // Case 3: Deleting a node with two children
        else
        {
            // find and delete successor - go right once and then go left like there's no tomorrow
            AVL* successor = r->right;
            int lefts = 0;
            while(successor->left != nullptr)
            {
                successor = successor->left;
                lefts++;
            }

            // copy successor data over to the node we were removing
            r->data = successor->data;

            // delete the successor
            AVL* parent;
            if(lefts > 0)
            {
                parent = r->right;
                for(int i=0; i < lefts-1; i++)
                {
                    parent = parent->left;
                }
                parent->left = deleteNode(parent->left,r->data);
                return parent->left;
            }
            else // we only went right once for successor, so therefore root is direct parent
            {
                parent = r;
                parent->right = deleteNode(parent->right, r->data);
                return parent->right;
            }
        }
    }
}

AVL* AVL::leftRotate(AVL* r)
{
	return nullptr;
}

AVL* AVL::rightRotate(AVL* r)
{
	return nullptr;
}

int AVL::max(int a, int b)
{
    return a > b ? a : b;
}


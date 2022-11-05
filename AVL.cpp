#include "AVL.h"
#include <sstream>
#include <iostream>

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

void preOrderRec(AVL* r, std::ostringstream& oss)
{
    if(r == nullptr)
        return;
    oss << r->data << " , ";
    preOrderRec(r->left,oss);
    preOrderRec(r->right,oss);
}
void AVL::preOrder(AVL* r)
{
    std::ostringstream oss;
    std::cout << "PRE ORDER: {";
    preOrderRec(r,oss);
    std::cout << oss.str().substr(0,oss.str().size()-2);
    std::cout << "}\n";
}

void inOrderRec(AVL* r, std::ostringstream& oss)
{
    if(r == nullptr)
        return;
    inOrderRec(r->left,oss);
    oss << r->data << " , ";
    inOrderRec(r->right,oss);
}

void AVL::inOrder(AVL* r)
{
    std::cout << "IN ORDER: {";
    std::ostringstream oss;
    inOrderRec(r,oss);
    std::cout << oss.str().substr(0,oss.str().size()-2);
    std::cout << "}\n";
}

int AVL::getBalance(AVL* r)
{
    return std::abs(getHeight(r->right) - getHeight(r->left));
}

int AVL::getHeight(AVL* r)
{
    // constant time
    return r->height;
}

std::pair<AVL*,int> insertNodeRec(AVL* r, int key, int levels)
{
	//TODO REBALANCE AFTER INSERTION
    if(r == nullptr)
    {
        return {nullptr,0};
    }

    if(r->data == key)
	{
        return {nullptr,0};
	}

	if(key < r->data)
	{
        if(r->left != nullptr)
        {
            std::pair<AVL*,int> p = insertNodeRec(r->left,key,levels+1);
            if(r->height < p.second - levels)
            {
                r->height = p.second - levels;
            }
            return p;
        }
        else
        {
            r->height++;
            r->left = new AVL(key);
            return {r,levels+2};
        }
	}
	else
	{
        if(r->right != nullptr)
        {
            std::pair<AVL*,int> p = insertNodeRec(r->right,key,levels+1);
            if(r->height < p.second - levels)
            {
                r->height = p.second - levels;
            }
            return p;
        }
        else
        {
            r->height++;
            r->right = new AVL(key);
            return {r,levels+2};
        }
	}
    return {r,0};
}

AVL* AVL::insertNode(AVL* r, int key)
{
    insertNodeRec(r,key,0);
    return r;
}

/*
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


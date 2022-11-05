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
    return getHeight(r->right) - getHeight(r->left);
}

int AVL::getHeight(AVL* r)
{
    if(r == nullptr)
    {
        return 0;
    }
    // constant time
    return r->height;
}

void rebalanceHelper(AVL*& r)
{
    // figure out which case
    int bf = r->getBalance(r);
    int childBf = (r->left == nullptr) ? r->getBalance(r->right) : r->getBalance(r->left);

    // Case 1: Right Right
    if(bf > 0 && childBf > 0)
    {
        r = r->leftRotate(r);
    }
    // Case 2: Left Left
    else if(bf < 0 && childBf < 0)
    {
        r = r->rightRotate(r);
    }
    // Case 3: Left Right
    else if(bf < 0 && childBf > 0)
    {
        r->left = r->leftRotate(r->left);
        r = r->rightRotate(r);
    }
    // Case 4: Right Left
    else if(bf > 0 && childBf < 0)
    {
        r = r->rightRotate(r->right);
        r = r->leftRotate(r);
    }
}

void rebalance(AVL*& r)
{
    if(std::abs(r->getBalance(r)) > 1)
    {
        rebalanceHelper(r);
    }
}

void rebalanceSweep(AVL*& r)
{
    if(r==nullptr)
        return;
    rebalance(r);
    rebalanceSweep(r->left);
    rebalanceSweep(r->right);
}

std::pair<AVL*,int> insertNodeRec(AVL* r, int key, int levels)
{
    if(r == nullptr)
    {
        return {nullptr,0};
    }

    if(r->data == key)
	{
        return {nullptr,0};
	}

    if(r->right == nullptr && r->left == nullptr)
    {
        r->height++;
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
            r->right = new AVL(key);
            return {r,levels+2};
        }
	}
    return {r,0};
}

AVL* AVL::insertNode(AVL* r, int key)
{
    insertNodeRec(r,key,0);
    rebalanceSweep(r);
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

std::pair<AVL*,int> deleteRec(AVL*& r, int key, int levels, AVL*& parent)
{
    // base case we hit a leaf, so not found, return nothing
    if(r == nullptr)
    {
        return {nullptr,0};
    }

    if(key < r->data) // key is smaller so look left
    {
        std::pair<AVL*,int> p = deleteRec(r->left,key,levels+1,r);
        if(p.second > 0)
        {
            r->height--;
            p.second--;
        }
        return p;
    }
    else if(key > r->data) // key is larger so look right
    {
        std::pair<AVL*,int> p = deleteRec(r->right,key,levels+1,r);
        if(p.second > 0)
        {
            r->height--;
            p.second--;
        }
        return p;
    }
    else // key matches. We want to delete this node
    {
        // Case 1: Deleting a leaf node
        if(r->left == nullptr && r->right == nullptr)
        {
            delete r;
            r = nullptr;
            return {nullptr,levels};
        }
        // Case 2: Deleting a node with a single child
        else if(r->left == nullptr || r->right == nullptr)
        {
            AVL* child = r->left == nullptr ? r->right : r->left;
            AVL*& parentPointerToChild = r->data < parent->data ? parent->left : parent->right;
            delete r;
            r = nullptr;
            parentPointerToChild = child;
            return {child,levels};
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
                std::pair<AVL*,int> p = deleteRec(parent->left,r->data,levels+1,r);
                parent->left = p.first;
                return {parent->left,0};
            }
            else // we only went right once for successor, so therefore root is direct parent
            {
                parent = r;
                std::pair<AVL*,int> p = deleteRec(parent->right, r->data,levels+1,r);
                parent->right = p.first;
                return {parent->right,0};
            }
        }
    }
}

AVL* AVL::deleteNode(AVL* r, int key)
{
    AVL* parent = nullptr;
    deleteRec(r,key,0,parent);
    rebalanceSweep(r);
    return r;
}

AVL* AVL::leftRotate(AVL* r)
{
    AVL* newRoot = r->right;
    AVL* subTreeToTransplant = newRoot->left;

    newRoot->left = r;
    r->right = subTreeToTransplant;

    //update root height
    r->height = getHeight(r);

    //update new root height
    newRoot->height = getHeight(newRoot);

    return newRoot;
}

AVL* AVL::rightRotate(AVL* r)
{
    AVL* newRoot = r->left;
    AVL* subTreeToTransplant = newRoot->right;

    newRoot->right = r;
    r->left = subTreeToTransplant;

    //update root height
    r->height = getHeight(r);

    //update new root height
    newRoot->height = getHeight(newRoot);

    return newRoot;
}

int AVL::max(int a, int b)
{
    return a > b ? a : b;
}


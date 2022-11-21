// CMP201 - AX2 - AVL.cpp
// Solved by Sam Collier - 2100259

#include "AVL.h"
#include <sstream>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

// Adrian Schneider - https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c

// It's way easier to print the binary tree sideways.
// when using std::cout we can't go back on ourselves
// we can print to the right and down, but that's it.
// So as we go right we go down more levels. And we make a new line to represent left and right

// I cannot take full credit for this implementation
std::string printTree(AVL* node, std::ostringstream& oss,std::string prefix, bool bIsLeft)
{
    if(node != nullptr)
    {
        // may have issues displaying nice on windows
        // but I use linux so I don't have any problems
        oss << prefix << (bIsLeft ? "├──" : "└──" );
        oss << node->data << "h" << node->height << '\n';

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

// I use a recursive helper function just to make formatting the end string easier
// I can just substring off the last comma and add a closing } before printing the
// string stream out
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
    if(r == nullptr)
    {
        return 0;
    }
    return getHeight(r->right) - getHeight(r->left);
}

// yeah that's right. Height doesn't even get calculated here
// height gets modified every single time there's a rebalance, insertion, or deletion
// so all we gotta do now is return it
int AVL::getHeight(AVL* r)
{
    if(r == nullptr)
    {
        return 0;
    }
    return r->height;
}

// rebalances a given a root node
void rebalance(AVL*& r)
{
	// figure out which case
	int bf = r->getBalance(r);
	int leftBf = (r->left == nullptr) ? -1 : r->getBalance(r->left);
	int rightBf = (r->right == nullptr) ? -1 : r->getBalance(r->right);
	// the rotations we end up doing depend on which child we go with
	// whichever child has the furthest balance factor is the one that we'll go with
	int childBf = std::abs(leftBf) > std::abs(rightBf) ? leftBf : rightBf;

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
        r->right = r->rightRotate(r->right);
        r = r->leftRotate(r);
    }
    else if(childBf == 0)
    {
        // choose tallest child
        if(r->getHeight(r->left) > r->getHeight(r->right))
        {
            r = r->rightRotate(r);
        }
        else
        {
            r = r->leftRotate(r);
        }
    }
}

// goes through checking and rebalancing everything in the tree
void rebalanceSweep(AVL*& r)
{
    if(r==nullptr)
        return;
    if(std::abs(r->getBalance(r)) > 1)
    {
        //std::cout << "REBALANCING!\n";
        //std::cout << "=========================\n";
        //std::cout << printTree(r);
        //std::cout << "*************************\n";
        rebalance(r);
        r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));
        //std::cout << printTree(r);
        //std::cout << "=========================\n";
    }
    rebalanceSweep(r->left);
    r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));
    rebalanceSweep(r->right);
    r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));
}

// cheeky helper function
// this allows us to keep track of whether
// or not we are going up or down the call stack
AVL* insertNodeRec(AVL* r, int key, bool& bHeadingUp)
{

	// base case - shouldn't ever happen but just in case - no insertion
    if(r == nullptr)
    {
        return r;
    }

	// found a match - no duplicates allowed - no insertion
    if(r->data == key)
	{
        return r;
	}

	// go left because what we want to insert is smaller
	if(key < r->data)
	{
		// if there's something on the left already
		// gonna need to recurse downwards another level
        if(r->left != nullptr)
        {
            r->left = insertNodeRec(r->left,key,bHeadingUp);
            r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));

            // if we're heading up the call stack
            // and we are unbalanced
            if(bHeadingUp && std::abs(r->getBalance(r)) > 1)
            {
                rebalance(r);
                // we only need to rebalance once
                // setting this boolean to false will prevent
                // any additional rebalances while going up the stack
                bHeadingUp = false;
            }

            return r;
        }
		// there's nothing on the left
		// so we're good to insert there
        else
        {
            r->left = new AVL(key);
            r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));
            bHeadingUp = true;
            return r;
        }
	}
	// go right - (Same ordeal as going left)
	else
	{
        if(r->right != nullptr)
        {
            r->right = insertNodeRec(r->right,key,bHeadingUp);
            r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));

            if(bHeadingUp && std::abs(r->getBalance(r)) > 1)
            {
                rebalance(r);
                bHeadingUp = false;
            }

            return r;
        }
        else
        {
            r->right = new AVL(key);
            r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));
            bHeadingUp = true;
            return r;
        }
	}
    return r;
}

// gonna use a cheeky helper function to do the actual insertion
AVL* AVL::insertNode(AVL* r, int key)
{
    bool bHeadingUp = false;
    r = insertNodeRec(r,key,bHeadingUp);
    //rebalanceSweep(r);
    return r;
}

// Another cheeky helper function
// allowing us to rebalance when heading up
AVL* deleteRec(AVL* r, int key, bool& bGoingUp)
{
    // base case we hit a leaf, so not found, return nothing
    if(r == nullptr)
    {
        return r;
    }

	// key is smaller so look left
    if(key < r->data)
    {
        r->left = deleteRec(r->left,key,bGoingUp);
        if(r->left != nullptr)
        {
            r->left->height = 1 + r->max(r->getHeight(r->left->left),r->getHeight(r->left->right));
        }

        if(bGoingUp && std::abs(r->getBalance(r)) > 1)
        {
            rebalance(r);
        }

        bGoingUp = true;
        return r;
    }
	// same ordeal as left for the right side
    else if(key > r->data) // key is larger so look right
    {
        r->right = deleteRec(r->right,key,bGoingUp);
        if(r->right != nullptr)
        {
            r->right->height = 1 + r->max(r->getHeight(r->right->right),r->getHeight(r->right->left));
        }

        if(bGoingUp && std::abs(r->getBalance(r)) > 1)
        {
            rebalance(r);
        }

        bGoingUp = true;
        return r;
    }
    else // key matches. We want to delete this node
    {
        // Case 1: Deleting a leaf node
        if(r->left == nullptr && r->right == nullptr)
        {
            delete r;
            r = nullptr;
            bGoingUp = true;
            return r;
        }
        // Case 2: Deleting a node with a single child
        else if(r->left == nullptr || r->right == nullptr)
        {
            AVL* child = r->left == nullptr ? r->right : r->left;

            delete r;

            bGoingUp = true;
            return child;
        }
        // Case 3: Deleting a node with two children
        else
        {
            // get successor
            AVL* successor = r->right;
            while(successor->left != nullptr)
            {
                successor = successor->left;
            }

            // copy data over
            r->data = successor->data;

            // delete successor
            r->right = deleteRec(r->right,successor->data,bGoingUp);

            r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));

            if(bGoingUp && std::abs(r->getBalance(r)) > 1)
            {
                rebalance(r);
            }

            bGoingUp = true;
            return r;
        }
    }
}

AVL* AVL::deleteNode(AVL* r, int key)
{
    bool bGoingUp = false;
    r = deleteRec(r,key,bGoingUp);
    //rebalanceSweep(r);
    return r;
}

AVL* AVL::leftRotate(AVL* r)
{
    //std::cout << "LEFT ROTATION!\n";
    //std::cout << "==============\n";
    //std::cout << printTree(r);

    AVL* newRoot = r->right;
    AVL* subTreeToTransplant = newRoot->left;

    newRoot->left = r;
    r->right = subTreeToTransplant;

    int newRootHeightBeforeChange = newRoot->height;

    r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));
    newRoot->height = 1 + r->max(newRoot->getHeight(newRoot->left),newRoot->getHeight(newRoot->right));

    //std::cout << "**************\n";
    //std::cout << printTree(newRoot);
    //std::cout << "==============\n";

    return newRoot;
}

AVL* AVL::rightRotate(AVL* r)
{
    //std::cout << "RIGHT ROTATION!\n";
    //std::cout << "==============\n";
    //std::cout << printTree(r);

    AVL* newRoot = r->left;
    AVL* subTreeToTransplant = newRoot->right;

    newRoot->right = r;
    r->left = subTreeToTransplant;

    int newRootHeightBeforeChange = newRoot->height;

    r->height = 1 + r->max(r->getHeight(r->left),r->getHeight(r->right));
    newRoot->height = 1 + r->max(newRoot->getHeight(newRoot->left),newRoot->getHeight(newRoot->right));

    //std::cout << "**************\n";
    //std::cout << printTree(newRoot);
    //std::cout << "==============\n";

    return newRoot;
}

int AVL::max(int a, int b)
{
    return a > b ? a : b;
}

int getHeightRec(AVL* r)
{
    if(r == nullptr)
    {
        return 0;
    }
    return 1 + std::max(getHeightRec(r->left), getHeightRec(r->right));
}

bool isBalanced(AVL* r)
{
    // get balance regardless of how height is in struct
    int bf = getHeightRec(r->right) - getHeightRec(r->left);
    if(std::abs(bf) <= 1)
    {
        return true;
    }
    return false;
}

// Utility function: Get two time points and return a string of nanoseconds between them.
std::string timeDelta(std::chrono::system_clock::time_point t, std::chrono::system_clock::time_point t2)
{
	long timeDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t).count();
	return std::to_string(timeDelta) + " ns";
}

void testInsert(int amountOfTreesToTest)
{
    //std::vector<std::pair<std::string,std::string>> results;
    AVL* tree = nullptr;
    for(int randomTests=0; randomTests < amountOfTreesToTest; randomTests++)
    {
        tree = new AVL(-1);
        // random elements inserted in ascending order
        int numElements = rand() % 200000;
        auto startTime = std::chrono::high_resolution_clock::now();
        for(int i=0; i<numElements;i++)
        {
            tree = tree->insertNode(tree, i);
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        //std::cout << printTree(tree);
        std::cout << "\nNumber of Elements inserted: " << numElements << std::endl;
        std::cout << "HEIGHT OF TREE: " << tree->getHeight(tree) << std::endl;
        std::cout << "TRUE HEIGHT OF TREE: " << getHeightRec(tree) << std::endl;
        std::cout << (isBalanced(tree) ? "TREE IS BALANCED\n" : "TREE IS NOT BALANCED\n");
        std::cout << "TREE BALANCE FACTOR: " << tree->getBalance(tree) << std::endl;
        std::cout << timeDelta(startTime,endTime) << " elapsed\n";

        //std::ostringstream oss;
        //oss << timeDelta(startTime,endTime);
        //results.push_back({std::to_string(numElements),oss.str()});

        delete tree;
    }
    /*
    std::ostringstream oss;
    for(int i=0;i<results.size();i++)
    {
        oss << "[" << results[i].first << "," << results[i].second.substr(0,results[i].second.size()-3) << "],\n";
    }
    std::cout << oss.str();
    */
}
int main()
{
    srand(time(0));
	AVL* tree = nullptr;

    //testInsert(300);

    // test delete
    /*
    {
        tree = new AVL(-1);
        int numElements = 30;
        auto startTime = std::chrono::high_resolution_clock::now();
        for(int i=0; i<numElements;i++)
        {
            tree = tree->insertNode(tree, i);
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        tree = tree->deleteNode(tree, 29);
        tree = tree->deleteNode(tree, 27);
        tree = tree->deleteNode(tree, 25);
        tree = tree->deleteNode(tree, 23);
        tree = tree->deleteNode(tree, 21);
        tree = tree->deleteNode(tree, 19);
        tree = tree->deleteNode(tree, 17);
        tree = tree->deleteNode(tree, 15);
        tree = tree->deleteNode(tree, 16);
        tree = tree->deleteNode(tree, 20);
        tree = tree->deleteNode(tree, 24);
        tree = tree->deleteNode(tree, 28);
        tree = tree->deleteNode(tree, -1);
        tree = tree->deleteNode(tree, 1);
        tree = tree->deleteNode(tree, 3);
        tree = tree->deleteNode(tree, 5);
        std::cout << printTree(tree);
        std::cout << "\nNumber of Elements inserted: " << numElements << std::endl;
        std::cout << "HEIGHT OF TREE: " << tree->getHeight(tree) << std::endl;
        std::cout << "TRUE HEIGHT OF TREE: " << getHeightRec(tree) << std::endl;
        std::cout << (isBalanced(tree) ? "TREE IS BALANCED\n" : "TREE IS NOT BALANCED\n");
        std::cout << "TREE BALANCE FACTOR: " << tree->getBalance(tree) << std::endl;
        std::cout << timeDelta(startTime,endTime) << " elapsed\n";

        //std::ostringstream oss;
        //oss << timeDelta(startTime,endTime);
        //results.push_back({std::to_string(numElements),oss.str()});

        delete tree;
    }
    */

	return 0;
}


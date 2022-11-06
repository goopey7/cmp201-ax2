#include "AVL.h"
#include <sstream>
#include <iostream>

// recusively print nodes root, left, right
void preOrderRec(AVL* r, std::ostringstream& oss)
{
    if(r == nullptr)
        return;
    oss << r->data << " , ";
    preOrderRec(r->left,oss);
    preOrderRec(r->right,oss);
}

// recusively print nodes left, root, right
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

// yeah that's right. Height doesn't even get calculated here
// height gets modified every single time theres a rebalance, insertion, or deletion
// so all we gotta do now is return it
int AVL::getHeight(AVL* r)
{
    if(r == nullptr)
    {
        return 0;
    }
    // constant time
    return r->height;
}

// rebalances a given a root node
void rebalanceHelper(AVL*& r)
{
	// figure out which case
	int bf = r->getBalance(r);
	int leftBf = (r->left == nullptr) ? -1 : r->getBalance(r->left);
	int rightBf = (r->right == nullptr) ? -1 : r->getBalance(r->right);
	// the the rotations we end up doing depend on which child we go with
	// whichever child has the larger balance factor is the one that we'll go with
	int childBf = std::abs(leftBf) > std::abs(rightBf) ? leftBf : rightBf;

	// If there are only 4 chains of rotations that can happen
	// there are also only 4 different ways heights can change
	// so instead of modifying height in the rotation function,
	// we just have to manually modify height depending on each case

	// in order to determine how the height changes just draw it
	// and play out the rotations

	// all height calculations everywhere are constant time with this approach

    // Case 1: Right Right
    if(bf > 0 && childBf > 0)
    {
        r->height-=2;
        r = r->leftRotate(r);
    }
    // Case 2: Left Left
    else if(bf < 0 && childBf < 0)
    {
        r->height-=2;
        r = r->rightRotate(r);
    }
    // Case 3: Left Right
    else if(bf < 0 && childBf > 0)
    {
        r->height-=2;
        r->left->right->height++;
        r->left = r->leftRotate(r->left);
        r = r->rightRotate(r);
    }
    // Case 4: Right Left
    else if(bf > 0 && childBf < 0)
    {
        r->height-=2;
        r->right->left->height++;
        r->right = r->rightRotate(r->right);
        r = r->leftRotate(r);
    }
}

// just checks if we should rebalance and calls helper if so
void rebalance(AVL*& r)
{
    if(std::abs(r->getBalance(r)) > 1)
    {
        rebalanceHelper(r);
    }
}

// goes through checking and rebalancing everything in the tree
void rebalanceSweep(AVL*& r)
{
    if(r==nullptr)
        return;
    rebalance(r);
    rebalanceSweep(r->left);
    rebalanceSweep(r->right);
}

// cheeky helper function
// this allows us to keep track of levels of recursion
// and everytime we enter a level of recursion we go one level
// down in the tree
// so we can calculate height on the fly
std::pair<AVL*,int> insertNodeRec(AVL* r, int key, int levels)
{
	// base case - shouldn't ever happen but just in case - no insertion
    if(r == nullptr)
    {
        return {nullptr,0};
    }

	// found a match - no duplicates allowed - no insertion
    if(r->data == key)
	{
        return {nullptr,0};
	}

	// if we made it this far, we are 100% gonna insert something
	// if we are gonna insert down a new level increment height
    if(r->right == nullptr && r->left == nullptr)
    {
        r->height++;
    }

	// go left because what we want to insert is smaller
	if(key < r->data)
	{
		// if there's something on the left already
		// gonna need to recurse downwards another level
        if(r->left != nullptr)
        {
            std::pair<AVL*,int> p = insertNodeRec(r->left,key,levels+1);
			// we inserted something below us
			// therefore we must change the height
			// p.second is the amount of levels down which the insertion took place
			// so if we subtract our current recursion level, we can get what our height should be
            if(r->height < p.second - levels)
            {
                r->height = p.second - levels;
            }
            return p;
        }
		// there's nothing on the left
		// so we're good to insert there
        else
        {
            r->left = new AVL(key);
            return {r,levels+2};
			// levels + 2 because first function call is level 0
			// and then also add 1 because we just added something one further level down
        }
	}
	// go right - (Same ordeal as going left)
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

// gonna use a cheeky helper function to do the actual insertion
AVL* AVL::insertNode(AVL* r, int key)
{
    insertNodeRec(r,key,0);
    rebalanceSweep(r);
    return r;
}

// Another cheeky helper function
// this allows me to modify the pointer passed in
// also keeps track of levels and allows us to easily refer to the parent
std::pair<AVL*,int> deleteRec(AVL*& r, int key, int levels, AVL*& parent)
{
    // base case we hit a leaf, so not found, return nothing
    if(r == nullptr)
    {
        return {nullptr,0};
    }

	// key is smaller so look left
    if(key < r->data)
    {
        std::pair<AVL*,int> p = deleteRec(r->left,key,levels+1,r);
		// we just deleted something below us
		// p.second represents how many times we have to decrement height up the way
        if(p.second > 0)
        {
            r->height--;
            p.second--;
        }
        return p;
    }
	// same ordeal as left for the right side
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
            r = nullptr; // pointer is passed by reference
            return {nullptr,levels};
        }
        // Case 2: Deleting a node with a single child
        else if(r->left == nullptr || r->right == nullptr)
        {
			// quick and slick ternary operation to get the child
            AVL* child = r->left == nullptr ? r->right : r->left;
			// using our parent, get their pointer to us
            AVL*& parentPointerToChild = r->data < parent->data ? parent->left : parent->right;

			// delete 
            delete r;
            r = nullptr;

			// have parent point to what was our child
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

    return newRoot;
}

AVL* AVL::rightRotate(AVL* r)
{
    AVL* newRoot = r->left;
    AVL* subTreeToTransplant = newRoot->right;

    newRoot->right = r;
    r->left = subTreeToTransplant;

    return newRoot;
}

int AVL::max(int a, int b)
{
    return a > b ? a : b;
}


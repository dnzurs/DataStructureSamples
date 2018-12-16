/* @Author
Student Name: Nebi Deniz Uras
Student ID : 504181527
Date: 24.12.2018 */

//============================================================================//
//
//	File Name		: hw5.cpp
//	Description		: Data Structures Assignment 5
//
//	Important Notes	:
//
//============================================================================//

//============================================================================//
//=============================== INCLUDES ===================================//
//============================================================================//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>

//============================================================================//
//==========================  MACRO DEFINITIONS ==============================//
//============================================================================//

//============================================================================//
//=========================== TYPE DEFINITIONS ===============================//
//============================================================================//

struct Node {
	int number;
	Node *left;
	Node *right;
};

struct Tree {
	Node *root;
	int treeLevel;
	int nodeCount;
	int sumVal;
	char *fileName;
	void create();
	void close();
	void emptyTree(Node *);
	void Tree::insert(Node *nodePtr, int val);
	void traverse_preorder(Node *);
	void readFromFile();
};

//============================================================================//
//========================== FUNCTION PROTOTYPES =============================//
//============================================================================//

//============================================================================//
//============================ GLOBAL VARIABLES ==============================//
//============================================================================//

//============================================================================//
//============================ PUBLIC FUNCTIONS ==============================//
//============================================================================//
int main(int argc, char ** argv)
{
	Tree tree;

	if (argc != 2)
	{
		std::cout << "ERROR : Invalid Argument\n" << std::endl;
	}
	else
	{
		tree.fileName = argv[1];

		tree.create();
	}

#if _WIN32
	system("Pause");
#endif	
	return 0;
}

//============================================================================//
//============================ PRIVATE FUNCTIONS =============================//
//============================================================================//

/******************************* FUNCTIONS OF TREE ****************************/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::create()
{
	root = NULL;		// create empty tree
	nodeCount = 0;		// initialize nodeCount to 0
	treeLevel = 0;		// initialize treeLevel to 0
	readFromFile();		// read from file and initialize tree
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::close()
{
	nodeCount = 0;
	treeLevel = 0;
	emptyTree(root);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::emptyTree(Node *nodePtr)
{
	if (nodePtr)
	{
		if (nodePtr->left != NULL) 
		{
			emptyTree(nodePtr->left);
			nodePtr->left = NULL;
		}

		if (nodePtr->right != NULL) 
		{
			emptyTree(nodePtr->right);
			nodePtr->right = NULL;
		}

		delete nodePtr;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::insert(Node *nodePtr, int val)
{
	if (nodePtr == NULL)
	{
		nodePtr = new Node;

		nodePtr->number = val;
		nodePtr->left = NULL;
		nodePtr->right = NULL;

		nodeCount++;

		return;
	}
	else
	{
		insert(nodePtr->left, val);
		insert(nodePtr->right, val);
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::traverse_preorder(Node *nodePtr)
{

}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::readFromFile()
{
	FILE *fd = NULL;

	fd = fopen(fileName, "r");
	if (fd)
	{
		int tempVal = 0;
		int lastVal = 0;

		while (1)
		{
			// read holeDepths
			fscanf(fd, "%d", &tempVal);

			if (feof(fd) && lastVal == tempVal)
			{
				break;
			}

			if (tempVal != 0)
			{
				insert(root, tempVal);
				lastVal = tempVal;
			}
		}
	}
	else
	{
		std::cout << "ERROR : File not opened\n" << std::endl;
	}
}

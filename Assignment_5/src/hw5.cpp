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
#define MEMBER_COUNT_OF_TREE		(250)
#define MEMBER_COUNT_OF_SUM_ARRAY	(50)

//============================================================================//
//=========================== TYPE DEFINITIONS ===============================//
//============================================================================//

struct Node {
	int data;
	int index;
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
	void emptyTree(Node *nodePtr);
	void insert(int data, int index);
	void traverse_preorder(Node *nodePtr);
	void readFromFile();
	Node* findNode(int index, Node *sourceNode);
	Node* newNode(int data, int index);
	void printResult();
	void checkResult();
};

//============================================================================//
//========================== FUNCTION PROTOTYPES =============================//
//============================================================================//

//============================================================================//
//============================ GLOBAL VARIABLES ==============================//
//============================================================================//
Node *searchNode = NULL; // node ptr for searching parent

int sumArray[MEMBER_COUNT_OF_SUM_ARRAY] = { 0 };
int sumArrayCount = 0;
int tempSum = 0;

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
		
		// find left path according to sum
		tempSum = tree.root->data;
		sumArray[sumArrayCount++] = tempSum;

		tree.traverse_preorder(tree.root->left);

		tree.printResult();

		// find right path according to sum
		memset(sumArray, 0, sizeof(sumArray));
		sumArrayCount = 0;

		tempSum = tree.root->data;
		sumArray[sumArrayCount++] = tempSum;

		tree.traverse_preorder(tree.root->right);

		tree.printResult();

		// free memory
		tree.close();
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
void Tree::insert(int data, int index)
{
	if (root == NULL && index == 1)
	{
		root = newNode(data, index);
	}
	else if (index == 2)
	{
		root->left = newNode(data, index);
	}
	else if (index == 3)
	{
		root->right = newNode(data, index);
	}
	else
	{
		Node *tempNode = NULL;
		int tempIndex = 0;

		// clear last temp node
		searchNode = NULL;

		// find parent index of data
		if (index % 2 == 0)
		{
			tempIndex = index;
		}
		else
		{
			tempIndex = index - 1;
		}
		tempIndex /= 2;

		// find parent node of next data according to index
		tempNode = findNode(tempIndex, root);

		// insert data to right or left path according to odd or even
		if (index % 2 == 0)
		{
			tempNode->left = newNode(data, index);
		}
		else
		{
			tempNode->right = newNode(data, index);
		}
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::traverse_preorder(Node *nodePtr)
{
	// if sum is equal to sum val, return for printing the true path
	if (tempSum == sumVal || nodePtr == NULL)
	{
		return;
	}

	// if sum is greater than sum val, decreased last node data
	if (tempSum > sumVal)
	{
		tempSum -= nodePtr->data;
		sumArray[sumArrayCount--] = nodePtr->data;
		return;
	}

	// if sum is less than sum val, go to next node of tree
	if (tempSum < sumVal)
	{
		if (nodePtr != NULL)
		{
			tempSum += nodePtr->data;
			sumArray[sumArrayCount++] = nodePtr->data;
		}

		traverse_preorder(nodePtr->left);
		traverse_preorder(nodePtr->right);
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::readFromFile()
{
	FILE *fd = NULL;

	fd = fopen(fileName, "r");
	if (fd)
	{
		int tempArray[MEMBER_COUNT_OF_TREE] = { 0 };
		int tempData = 0;
		int lastData = 0;

		while (1)
		{
			// read from file
			fscanf(fd, "%d", &tempData);

			if (feof(fd) && lastData == tempData)
			{
				// set sum value for calculation
				sumVal = tempArray[tempArray[0]];

				// clear sum val index
				tempArray[tempArray[0]] = 0;
				tempArray[0]--;				

				// set tree level
				treeLevel = tempArray[0] / 2;

				// set node count
				nodeCount = tempArray[0];

				break;
			}

			// set temp array with members of tree while reading from file
			if (tempData != 0)
			{
				tempArray[0]++;
				lastData = tempData;
				tempArray[tempArray[0]] = tempData;
			}
		}

		// insert data to tree
		for (int i = 1; i <= tempArray[0]; i++)
		{
			insert(tempArray[i], i);
		}
	}
	else
	{
		std::cout << "ERROR : File not opened\n" << std::endl;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Node* Tree::newNode(int data, int index)
{
	Node *temp = new Node;

	temp->data = data;
	temp->index = index;
	temp->left = temp->right = NULL;

	return temp;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Node* Tree::findNode(int index, Node *sourceNode)
{
	if (searchNode != NULL)
	{
		return searchNode;
	}

	if (sourceNode != NULL)
	{
		if (index == sourceNode->index)
		{
			searchNode = sourceNode;
			return searchNode;
		}

		searchNode = findNode(index, sourceNode->left);
		searchNode = findNode(index, sourceNode->right);
	}

	return searchNode;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::printResult()
{
	checkResult();

	if (sumArrayCount == 0)
	{
		std::cout << "No Path Found" << std::endl;
	}
	else
	{
		std::cout << "Path Found: ";
		for (int i = 0; i < sumArrayCount; i++)
		{
			std::cout << sumArray[i] << " ";
		}
		std::cout << std::endl;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Tree::checkResult()
{
	int tempSum = 0;

	for (int i = 0; i < sumArrayCount; i++)
	{
		tempSum += sumArray[i];
	}

	if (tempSum != sumVal)
	{
		memset(sumArray, 0, sizeof(sumArray));
		sumArrayCount = 0;
	}
}
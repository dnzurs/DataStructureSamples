/* @Author
Student Name: Nebi Deniz Uras
Student ID : 504181527
Date: 12.11.2018 */

//============================================================================//
//
//	File Name		: hw4.cpp
//	Description		: Data Structures Assignment 4
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

// nodes of the list
struct Node {
	int data;
	Node *next;
};

struct queueAnt {
	int count;
	Node *front;
	Node *back;
	void create();
	void close();
	void enqueue(int newData);
	int dequeue();
	bool isempty();
};

struct stackAnt {
	int count;
	Node *head;
	void create();
	void close();
	void push(int newData);
	int  pop();
	bool isEmpty();
};

struct Ants {
	queueAnt ants;
	queueAnt holeDepths;
	stackAnt hole;
	void ReadFile(char *fileName);
	void ShowContents(bool);
	void CrossRoad();
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
	Ants a;

	a.ReadFile(argv[1]); // store the number of ants and depths of holes
	std::cout << "The initial Ant sequence is: ";
	a.ShowContents(1); // list ant sequence ( initially : 1, 2, ... , N)
	std::cout << "The depth of holes are: ";
	a.ShowContents(0); // list depth of holes
	a.CrossRoad();
	std::cout << "The final Ant sequence is: ";
	a.ShowContents(1);

	// free memory
	a.ants.close();
	a.hole.close();
	a.holeDepths.close();

#if _WIN32
	system("Pause");
#endif	
	return 0;
}

//============================================================================//
//============================ PRIVATE FUNCTIONS =============================//
//============================================================================//

/*************************** FUNCTIONS OF ANT STRUCTS *************************/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Ants::ReadFile(char *fileName)
{
	FILE *fd = NULL;

	fd = fopen(fileName, "r");
	if (fd)
	{
		int tempVal = 0;
		int lastHoleVal = 0;

		// create queues and stack
		ants.create();
		holeDepths.create();
		hole.create();

		// read count of ants
		fscanf(fd, "%d", &tempVal);

		// fill ants queue
		for (int i = 1; i <= tempVal; i++)
		{
			ants.enqueue(i);
		}

		tempVal = 0;

		// fill holeDepths queue
		while (1)
		{
			// read holeDepths
			fscanf(fd, "%d", &tempVal);

			if (feof(fd) && lastHoleVal == tempVal)
			{
				break;
			}

			if (tempVal != 0)
			{
				holeDepths.enqueue(tempVal);
				lastHoleVal = tempVal;
			}
		}

		// fill hole stack
		while(!holeDepths.isempty())
		{
			hole.push(holeDepths.dequeue());
		}
	}
	else
	{
		std::cout << "ERROR : File not opened\n" << std::endl;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Ants::ShowContents(bool selectShowingQueue)
{
	if (selectShowingQueue) // show list ant sequence 
	{
		int *tempAntQueue = NULL;
		int tempCount = ants.count;

		tempAntQueue = new int[tempCount];

		// print ants queue 
		for (int i = 0; i < tempCount; i++)
		{
			tempAntQueue[i] = ants.dequeue();
			std::cout << tempAntQueue[i] << " ";
		}

		std::cout << std::endl;

		// fill ants queue again
		for (int i = 0; i < tempCount; i++)
		{
			ants.enqueue(tempAntQueue[i]);
		}

		delete tempAntQueue;
	}
	else  // show list holeDepths sequence
	{
		int *tempHoleStack = NULL;
		int tempCount = hole.count;

		tempHoleStack = new int[tempCount];

		// print ants queue 
		for (int i = 0; i < tempCount; i++)
		{
			tempHoleStack[i] = hole.pop();
		}

		for (int i = tempCount; i > 0; i--)
		{
			std::cout << tempHoleStack[i - 1] << " ";
		}

		std::cout << std::endl;

		// fill ants queue again
		for (int i = 0; i < tempCount; i++)
		{
			hole.push(tempHoleStack[i]);
		}

		delete tempHoleStack;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Ants::CrossRoad()
{
	while (!hole.isEmpty())
	{
		int *tempAntQueueInHole = NULL;
		int countAntsInHole = hole.pop();
		
		tempAntQueueInHole = new int[countAntsInHole];

		// ants come to hole
		for (int i = 0; i < countAntsInHole; i++)
		{
			tempAntQueueInHole[i] = ants.dequeue();
		}

		// ants go from hole
		for (int i = 0; i < countAntsInHole; i++)
		{
			ants.enqueue(tempAntQueueInHole[i]);
		}
	}
}

/*************************** FUNCTIONS OF queueAnt STRUCTS ********************/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void queueAnt::create()
{
	front = NULL; 
	back = NULL;
	count = 0;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void queueAnt::close()
{
	Node *p;

	while (front) 
	{
		p = front;
		front = front->next;
		delete p;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void queueAnt::enqueue(int newData)
{
	Node *newnode = new Node;

	newnode->data = newData;
	newnode->next = NULL;

	if (isempty())
	{ 
		back = newnode;
		front = back;
	}
	else 
	{
		back->next = newnode;
		back = newnode;
	}

	count++;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int  queueAnt::dequeue()
{
	Node *topnode;
	int temp;

	topnode = front;
	front = front->next;
	temp = topnode->data;

	delete topnode;
	count--;

	return temp;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool queueAnt::isempty()
{
	return front == NULL;
}

/*************************** FUNCTIONS OF stackAnt STRUCTS ********************/
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void stackAnt::create()
{
	head = NULL;
	count = 0;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void stackAnt::close()
{
	Node *p;

	while (head)
	{
		p = head;
		head = head->next;
		delete p;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void stackAnt::push(int newData)
{
	Node *newnode = new Node;

	newnode->data = newData;
	newnode->next = head;
	head = newnode;

	count++;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int  stackAnt::pop()
{
	Node *topnode;
	int temp;

	topnode = head;
	head = head->next;
	temp = topnode->data;

	delete topnode;
	count--;

	return temp;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool stackAnt::isEmpty()
{
	return head == NULL;
}

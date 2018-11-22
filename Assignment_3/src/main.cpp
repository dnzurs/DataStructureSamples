//============================================================================//
//
//	File Name	: assignment3.cpp
//	Create Date	: 03.11.2018
//	Designers	: Nebi Deniz Uras
//	Number 	    : 504181527
//	Description	: Data Structures Homework 3
//
//	Important Notes:
//
//============================================================================//
#define _CRT_SECURE_NO_WARNINGS
//============================================================================//
//=============================== INCLUDES ===================================//
//============================================================================//
#include <iostream>

//============================================================================//
//==========================  MACRO DEFINITIONS ==============================//
//============================================================================//
#define FILE_NAME_LENGTH	(256)

//============================================================================//
//=========================== TYPE DEFINITIONS ===============================//
//============================================================================//
struct Node {
	int data;
	Node *next;
};

struct Deck{
	Node *head;
	void create();
	void close();
	void push(int val);
	int  pop();
	bool isempty();
	int fillDeck(int val);
};

//============================================================================//
//========================== FUNCTION PROTOTYPES =============================//
//============================================================================//
bool isAvailableVal(int val);
void printErrorMessage(const char *msg);

//============================================================================//
//============================ GLOBAL VARIABLES ==============================//
//============================================================================//

//============================================================================//
//============================ PUBLIC FUNCTIONS ==============================//
//============================================================================//

int main(int argc, char *argv[])
{
	char fileName[FILE_NAME_LENGTH] = { 0 };
	int playerDeckCount = 0; // second value of first line
	int tableDeckCount = 0;	// first value of first line
	int retVal = -1;
	Deck tableDeck;
	Deck binDeck;
	Deck firstPlayerDeck;
	Deck secondPlayerDeck;
	FILE *fd = NULL;

	if (argc != 2)
	{
		printErrorMessage("ERROR : Invalid Argument\n");
		return -1;
	}

	// get name of files that will be read
	memcpy(fileName, argv[1], strlen(argv[1]));

	fd = fopen(fileName, "r");
	if (fd)
	{
		int tempVal = 0;

		fscanf(fd, "%d%d", &tableDeckCount, &playerDeckCount);
		if (tableDeckCount == 0 || playerDeckCount == 0)
		{
			printErrorMessage("ERROR : Read wrong Value from file(it is not integer)");
			return -1;
		}

		// fill table deck
		for (int i = 0; i < tableDeckCount; i++)
		{
			fscanf(fd, "%d", &tempVal);

			retVal = tableDeck.fillDeck(tempVal);
			if (retVal == -1)
			{
				break;
			}
		}

		// fill first player's deck
		if (retVal == 0)
		{
			for (int i = 0; i < playerDeckCount; i++)
			{
				fscanf(fd, "%d", &tempVal);

				retVal = firstPlayerDeck.fillDeck(tempVal);
				if (retVal == -1)
				{
					break;
				}
			}
		}

		// fill second player's deck
		if (retVal == 0)
		{
			for (int i = 0; i < playerDeckCount; i++)
			{
				fscanf(fd, "%d", &tempVal);

				retVal = secondPlayerDeck.fillDeck(tempVal);
				if (retVal == -1)
				{
					break;
				}
			}
		}

		fclose(fd);

		if (retVal == -1)
		{
			printErrorMessage("ERROR : Read wrong Value from file(it is not integer)");
			return -1;
		}

		// start game

	}
	else
	{
		printErrorMessage("ERROR : File not opened\n");
		return -1;
	}


#if _WIN32
	system("Pause");
#endif	
	return 0;
}

//============================================================================//
//============================ PRIVATE FUNCTIONS =============================//
//============================================================================//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void printErrorMessage(const char *msg)
{
	std::cout << *msg << std::endl;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool isAvailableVal(int val)
{
	if (val == 0)
	{
		return false;
	}

	if (!(-1000000 < val || val < 1000000))
	{
		return false;
	}

	return true;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Deck::create()
{
	head = NULL;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Deck::close()
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
void Deck::push(int val)
{
	Node *newNode = new Node;

	newNode->data = val;
	newNode->next = head;
	head = newNode;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int Deck::pop()
{
	Node *topNode;
	int temp;

	topNode = head;
	head = head->next;
	temp = topNode->data;

	delete topNode;

	return temp;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool Deck::isempty()
{
	return head == NULL;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int Deck::fillDeck(int val)
{
	return head == NULL;
}

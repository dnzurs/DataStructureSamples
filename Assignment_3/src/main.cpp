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
#include <string>

//============================================================================//
//==========================  MACRO DEFINITIONS ==============================//
//============================================================================//
#define FILE_NAME_LENGTH	(256)
#define SUCCESS				(0)
#define FAILURE				(-1)

//============================================================================//
//=========================== TYPE DEFINITIONS ===============================//
//============================================================================//
struct Node {
	int data;
	Node *next;
};

struct Deck{
	Node *head;
	int count;
	void create();
	void close();
	void push(int val);
	int  pop();
	bool isempty();
	int fillDeck(int val);
};

struct GameDecks {
	Deck tableDeck;
	Deck binDeck;
	Deck firstPlayerDeck;
	Deck secondPlayerDeck;
	char winningPlayer;
	char currPlayer;
};

//============================================================================//
//========================== FUNCTION PROTOTYPES =============================//
//============================================================================//
int initializeDecks(GameDecks *decks, const char *fileName);

//============================================================================//
//============================ GLOBAL VARIABLES ==============================//
//============================================================================//

//============================================================================//
//============================ PUBLIC FUNCTIONS ==============================//
//============================================================================//

int main(int argc, char *argv[])
{
	char fileName[FILE_NAME_LENGTH] = { 0 };
	int retVal = SUCCESS;
	GameDecks decks;


	if (argc != 2)
	{
		std::cout << "ERROR : Invalid Argument\n" << std::endl;
		retVal = FAILURE;
	}
	else
	{
		// get name of files that will be read
		memcpy(fileName, argv[1], strlen(argv[1]));

		// fill all decks
		retVal = initializeDecks(&decks, fileName);

		// play game
		
	}

#if _WIN32
	system("Pause");
#endif	
	if (retVal == FAILURE)
	{
		return -1;
	}
	return 0;
}

//============================================================================//
//============================ PRIVATE FUNCTIONS =============================//
//============================================================================//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int initializeDecks(GameDecks *decks, const char *fileName)
{
	int playerDeckCount = 0; // second value of first line
	int tableDeckCount = 0;	// first value of first line
	int retVal = SUCCESS;
	FILE *fd = NULL;

	// create decks
	decks->tableDeck.create();
	decks->binDeck.create();
	decks->firstPlayerDeck.create();
	decks->secondPlayerDeck.create();

	fd = fopen(fileName, "r");
	if (fd)
	{
		int tempVal = 0;

		// read deck member's count
		fscanf(fd, "%d%d", &tableDeckCount, &playerDeckCount);
		if (tableDeckCount == 0 || playerDeckCount == 0)
		{
			retVal = FAILURE;
		}

		// fill table deck
		if (retVal == SUCCESS)
		{
			for (int i = 0; i < tableDeckCount; i++)
			{
				fscanf(fd, "%d", &tempVal);

				retVal = decks->tableDeck.fillDeck(tempVal);
				if (retVal == FAILURE)
				{
					break;
				}
			}
		}

		// fill first player's deck
		if (retVal == SUCCESS)
		{
			for (int i = 0; i < playerDeckCount; i++)
			{
				fscanf(fd, "%d", &tempVal);

				retVal = decks->firstPlayerDeck.fillDeck(tempVal);
				if (retVal == FAILURE)
				{
					break;
				}
			}
		}

		// fill second player's deck
		if (retVal == SUCCESS)
		{
			for (int i = 0; i < playerDeckCount; i++)
			{
				fscanf(fd, "%d", &tempVal);

				retVal = decks->secondPlayerDeck.fillDeck(tempVal);
				if (retVal == FAILURE)
				{
					break;
				}
			}
		}

		fclose(fd);

		if (retVal == FAILURE)
		{
			std::cout << "ERROR : Read wrong Value from file(it is not integer)\n" << std::endl;

			// free if decks have members
			decks->tableDeck.close();
			decks->binDeck.close();
			decks->firstPlayerDeck.close();
			decks->secondPlayerDeck.close();
		}
	}
	else
	{
		std::cout << "ERROR : File not opened\n" << std::endl;
		retVal = FAILURE;
	}

	return retVal;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Deck::create()
{
	head = NULL;
	count = 0;
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

	count = 0;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Deck::push(int val)
{
	Node *newNode = new Node;

	newNode->data = val;
	newNode->next = head;
	head = newNode;
	count++;
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
	count--;

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
	if (!(val < -1000000 || val > 1000000 || val == 0))
	{
		return FAILURE;
	}

	push(val);

	return SUCCESS;
}

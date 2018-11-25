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

//============================================================================//
//=============================== INCLUDES ===================================//
//============================================================================//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>

//============================================================================//
//==========================  MACRO DEFINITIONS ==============================//
//============================================================================//
#define FILE_NAME_LENGTH	(256)
#define SUCCESS				(0)
#define FAILURE				(-1)

#define MAX_DECK_MEMBER_COUNT		(1000)
#define MIN_DECK_MEMBER_COUNT		(0)

#define MAX_DECK_DATA_VALUE			(1000000)
#define MIN_DECK_DATA_VALUE			(-1000000)

//============================================================================//
//=========================== TYPE DEFINITIONS ===============================//
//============================================================================//
// Member of Stack Decks
struct Node {
	int data;
	Node *next;
};

// Stack of Decks
struct Deck{
	Node *head;
	int count;
	void create();
	void close();
	void push(int val);
	int  pop();
	bool isEmpty();
	int fillDeck(int val);
};

// Struct of All Decks of Game
struct GameDecks {
	Deck tableDeck;
	Deck binDeck;
	Deck firstPlayerDeck;
	Deck secondPlayerDeck;
	int winningPlayer;
};

//============================================================================//
//========================== FUNCTION PROTOTYPES =============================//
//============================================================================//
int initializeDecks(GameDecks *decks, const char *fileName);
int playCardGame(GameDecks *decks);

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

		if (retVal == SUCCESS)
		{
			// play game
			playCardGame(&decks);

			// print winning player
			if (decks.winningPlayer > 0)
			{
				std::cout << decks.winningPlayer << ". player wins." << std::endl;
			}
			else
			{
				std::cout << "No player wins." << std::endl;
			}

			// free members
			decks.tableDeck.close();
			decks.binDeck.close();
			decks.firstPlayerDeck.close();
			decks.secondPlayerDeck.close();
		}
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
int playCardGame(GameDecks *decks)
{
	int retVal = SUCCESS;
	int cardFromTableDeck = 0;
	char player = 1; // first player starts firstly
	Deck *currPlayer = &decks->firstPlayerDeck; // first player starts firstly
	Deck *nextPlayer = &decks->secondPlayerDeck;

	// initialize winning player as first player. It will be updated at end of function. 
	decks->winningPlayer = 1;

	if (decks->tableDeck.count > 0)
	{
		while (true)
		{
			// get card from tableDeck
			cardFromTableDeck = decks->tableDeck.pop();

			// if card is less than 0, current player gives card to next player
			if (cardFromTableDeck < 0)
			{
				int counter = -cardFromTableDeck;
				int cardValueOfCurrDeck = 0;
				int cardValueOfNextDeck = 0;

				while (counter--)
				{
					// get card from current player's deck
					cardValueOfCurrDeck = currPlayer->pop();

					// get card from next player's deck
					cardValueOfNextDeck = nextPlayer->pop();

					if (cardValueOfCurrDeck > cardValueOfNextDeck)
					{
						// firstly push top value of next deck to next deck again
						nextPlayer->push(cardValueOfNextDeck);

						// secondly push top value of current deck to next deck
						nextPlayer->push(cardValueOfCurrDeck);
					}
					else
					{
						// firstly push top value of next deck to next deck again 
						nextPlayer->push(cardValueOfNextDeck);

						// secondly push top value of current deck to bin deck
						decks->binDeck.push(cardValueOfCurrDeck);
					}

					// if current player's deck is finished, the game is finished
					if (currPlayer->isEmpty())
					{
						break;
					}
				}
			}

			// ToDo : Bu kýsým olmayabilir
			// if card is more than 0, current player gets card from next player
			if (cardFromTableDeck > 0)
			{
				int cardValueOfCurrDeck = 0;
				int cardValueOfNextDeck = 0;

				while (cardFromTableDeck--)
				{
					// get card from next player's deck
					cardValueOfNextDeck = nextPlayer->pop();

					// get card from current player's deck
					cardValueOfCurrDeck = currPlayer->pop();
					
					if (cardValueOfNextDeck > cardValueOfCurrDeck)
					{
						// firstly push top value of current deck to curr deck again
						currPlayer->push(cardValueOfCurrDeck);

						// secondly push top value of next deck to curr deck
						currPlayer->push(cardValueOfNextDeck);
					}
					else
					{
						// firstly push top value of next deck to curr deck again 
						currPlayer->push(cardValueOfCurrDeck);

						// secondly push top value of next deck to bin deck
						decks->binDeck.push(cardValueOfCurrDeck);
					}

					// if next player's deck is finished, the game is finished
					if (nextPlayer->isEmpty())
					{
						break;
					}
				}
			}
			// ToDo : Bu kýsým olmayabilir

			// change current player
			if (player == 1)
			{
				currPlayer = &decks->secondPlayerDeck;
				nextPlayer = &decks->firstPlayerDeck;
				player = 2;
			}
			else
			{
				currPlayer = &decks->firstPlayerDeck;
				nextPlayer = &decks->secondPlayerDeck;
				player = 1;
			}

			// if any deck is empty, game is finished
			if (decks->tableDeck.isEmpty() || decks->firstPlayerDeck.isEmpty() || decks->secondPlayerDeck.isEmpty())
			{
				break;
			}
		}
	}

	// update winning player, if second player wins or there is no winning player
	if (decks->firstPlayerDeck.count > decks->secondPlayerDeck.count)
	{
		decks->winningPlayer = 2;
	}
	else if (decks->firstPlayerDeck.count == decks->secondPlayerDeck.count)
	{
		decks->winningPlayer = 0;
	}

	return retVal;
}
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

		// deck member count must be between 0 and 1000
		if ( !(MIN_DECK_MEMBER_COUNT <= tableDeckCount && tableDeckCount <= MAX_DECK_MEMBER_COUNT) || 
			 !(MIN_DECK_MEMBER_COUNT <= playerDeckCount && playerDeckCount <= MAX_DECK_MEMBER_COUNT) )
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
			std::cout << "ERROR : The reading value is not within the specified range\n" << std::endl;

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
bool Deck::isEmpty()
{
	return head == NULL;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int Deck::fillDeck(int val)
{
	if (val < MIN_DECK_DATA_VALUE || val > MAX_DECK_DATA_VALUE || val == 0)
	{
		return FAILURE;
	}

	push(val);

	return SUCCESS;
}

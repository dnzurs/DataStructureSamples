/* @Author
* Student Name: Nebi Deniz Uras
* Student ID : 504181527
* Date: 08.11.2018 */

//============================================================================//
//
//	File Name	: assignment2.cpp
//	Create Date	: 01.11.2018
//	Designers	: Nebi Deniz Uras
//	Number 	    : 504181527
//	Description	: Data Structures Homework 2
//
//	Important Notes:
//
//============================================================================//

//============================================================================//
//=============================== INCLUDES ===================================//
//============================================================================//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <locale.h>

//============================================================================//
//==========================  MACRO DEFINITIONS ==============================//
//============================================================================//
#define FILE_NAME_LENGTH	(256)
#define SPACE_CHARACTER		(' ')	// space character for empty grid

#define FILE_OPERATION		(0) // TODO : will be deleted
#define DEBUG				(1) // TODO : will be deleted

//============================================================================//
//=========================== TYPE DEFINITIONS ===============================//
//============================================================================//
//------------------------------------------------------------------------------
struct occur_node {
	char character;
	occur_node *next;
	int occurence;
};
//------------------------------------------------------------------------------
struct vocab_node {
	char character;
	vocab_node *next;
	occur_node *list;
};
//------------------------------------------------------------------------------
struct vocab_list {
	vocab_node *head;
	void create();
	void print();
	void add_char(char );
	void add_occurence(char , char );
	int get_occurence(char );
	int get_union_occurence (char , char );
};
//------------------------------------------------------------------------------
struct language_model {
	vocab_list *vocabularylist;
	void readData (const char *);
	double calculateProbability (char, char);
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

int main(int argc, char *argv[])
{
	char firstChar = 0;
	char secondChar = 0;
	char inputFile[FILE_NAME_LENGTH] = {0};
	language_model model;
	
	// allocate memory for model
	model.vocabularylist = (vocab_list *)malloc(sizeof(vocab_list));

	// change encode to work with Turkish charachter
	setlocale(LC_ALL, "Turkish");

#if FILE_OPERATION
	if (argc < 3)
	{
		std::cout << "Invalid Operation\n";
		return -1;
	}

	// get name of files that will be read, set charachters
	memcpy(inputFile, argv[1], strlen(argv[1]));
	firstChar = argv[2];
	secondChar = argv[3];
#else
	strcpy(inputFile, "C:/Users/USER/Desktop/GitHub/DataStructuresSamples/DataStructuresSamples/Assignment_2/src/assignment2_test/input.txt");
	firstChar = 'a';
	secondChar = 'r';
#endif 

	// read data and create model
	model.readData(inputFile);

	model.vocabularylist->print();

	// show probability of charachters
	std::cout << "\n";
	std::cout << "Occurence : " << model.vocabularylist->get_occurence(firstChar) << "\n";
	std::cout << "Co-Occurence : " << model.vocabularylist->get_union_occurence(firstChar, secondChar) << "\n";
	std::cout << model.calculateProbability(firstChar, secondChar) << "\n";

	system("Pause");
    return 0;
}

//============================================================================//
//============================ PRIVATE FUNCTIONS =============================//
//============================================================================//

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void vocab_list::create()
{
	this->head = (vocab_node *)malloc(sizeof(vocab_node));

	this->head->character = 0;
	this->head->next = nullptr;
	this->head->list = nullptr;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void vocab_list::print()
{
	vocab_node *currVocabNode = this->head;
	occur_node *currOccurNode = currVocabNode->list;

	while (true)
	{
		if (currVocabNode->character != 0)
		{
			std::cout << currVocabNode->character << ":\n";

			while (true)
			{
				if (currVocabNode->list != nullptr)
				{
					std::cout << "\t<" << currOccurNode->character << "," << currOccurNode->occurence << ">";
				}
				else
				{
					break;
				}

				std::cout << "\n";

				if (currOccurNode->next == nullptr)
				{
					break;
				}

				currOccurNode = currOccurNode->next;
			}
		}

		if (currVocabNode->next == nullptr)
		{
			break;
		}

		currVocabNode = currVocabNode->next;
		currOccurNode = currVocabNode->list;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void vocab_list::add_char(char character)
{
	// to insert first value 
	if (this->head->character == 0 && this->head->next == nullptr && this->head->list == nullptr)
	{
		this->head->character = character;
	}
	else
	{
		bool isCharExist = false;
		vocab_node *currVocabNode = this->head;

		while (true)
		{
			if (character == currVocabNode->character)
			{
				isCharExist = true;
				break;
			}

			if (currVocabNode->next == nullptr)
			{
				break;
			}

			currVocabNode = currVocabNode->next;
		}

		if (!isCharExist)
		{
			currVocabNode = this->head;

			while (true)
			{
				if (character == currVocabNode->character)
				{
					break;
				}
				else
				{
					if (isalpha(character))
					{
						if (!isalpha(currVocabNode->character) || character < currVocabNode->character)
						{
							vocab_node *newNode = (vocab_node *)malloc(sizeof(vocab_node));

							newNode->character = currVocabNode->character;
							newNode->list = currVocabNode->list;
							newNode->next = currVocabNode->next;

							currVocabNode->character = character;
							currVocabNode->list = nullptr;
							currVocabNode->next = newNode;

							break;
						}
						else if (character > currVocabNode->character && currVocabNode->next == nullptr)
						{
							currVocabNode->next = (vocab_node *)malloc(sizeof(vocab_node));

							currVocabNode->next->character = character;
							currVocabNode->next->next = nullptr;
							currVocabNode->next->list = nullptr;

							break;
						}
					}
					else // for punctuation marks
					{
						if (currVocabNode->next == nullptr)
						{
							currVocabNode->next = (vocab_node *)malloc(sizeof(vocab_node));

							currVocabNode->next->character = character;
							currVocabNode->next->next = nullptr;
							currVocabNode->next->list = nullptr;

							break;
						}
					}
				}

				currVocabNode = currVocabNode->next;
			}
		}
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void vocab_list::add_occurence(char firstChar, char secondChar)
{
	vocab_node *currVocabNode = this->head;
	bool isFinished = false;

	while (true)
	{
		if (currVocabNode->character == firstChar)
		{
			if (currVocabNode->list == nullptr)
			{
				currVocabNode->list = (occur_node *)malloc(sizeof(occur_node));

				currVocabNode->list->character = secondChar;
				currVocabNode->list->occurence = 1;
				currVocabNode->list->next = nullptr;

				break;
			}
			else
			{
				occur_node *currOccurNode = currVocabNode->list;

				while (true)
				{
					if (currOccurNode->character == secondChar)
					{
						currOccurNode->occurence++;

						isFinished = true;
						break;
					}
					else
					{
						if (currOccurNode->next == nullptr)
						{
							currOccurNode->next = (occur_node *)malloc(sizeof(occur_node));

							currOccurNode->next->character = secondChar;
							currOccurNode->next->occurence = 1;
							currOccurNode->next->next = nullptr;

							isFinished = true;
							break;
						}
					}

					currOccurNode = currOccurNode->next;
				}
			}
		}

		if (isFinished == true)
		{
			break;
		}

		currVocabNode = currVocabNode->next;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int vocab_list::get_occurence(char character)
{
	vocab_node *currVocabNode = this->head;
	occur_node *currOccurNode = nullptr;
	int occurence = 0;

	// find character in model
	while (true)
	{
		if (currVocabNode->character == character)
		{
			currOccurNode = currVocabNode->list;
			break;
		}

		currVocabNode = currVocabNode->next;
	}

	// if there is character, find occurence
	if (currOccurNode != nullptr)
	{
		while (true)
		{
			occurence += currOccurNode->occurence;

			if (currOccurNode->next == nullptr)
			{
				break;
			}

			currOccurNode = currOccurNode->next;
		}
	}

	return occurence;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int vocab_list::get_union_occurence(char firstChar, char secondChar)
{
	vocab_node *currVocabNode = this->head;
	occur_node *currOccurNode = nullptr;
	int co_occurence = 0;

	// find first character in model
	while (true)
	{
		if (currVocabNode->character == firstChar)
		{
			currOccurNode = currVocabNode->list;
			break;
		}

		currVocabNode = currVocabNode->next;
	}

	// if there is occurence with second character, find co_occurence
	if (currOccurNode != nullptr)
	{
		while (true)
		{
			if (currOccurNode->character == secondChar)
			{
				co_occurence = currOccurNode->occurence;
			}
			
			if (currOccurNode->next == nullptr)
			{
				break;
			}

			currOccurNode = currOccurNode->next;
		}
	}

	return co_occurence;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void language_model::readData(const char *inputFile)
{
	int retVal = 0;
	FILE *fd = nullptr;

	fd = fopen(inputFile, "r");
	if (fd != nullptr)
	{
		char currChar = 0;
		char prevChar = 0;

		// create vocabulary list
		this->vocabularylist->create();

		while (true)
		{
			fread(&currChar, sizeof(currChar), 1, fd);

			if (feof(fd))
			{
				break;
			}

			// if there is newline character, continue for next
			if (currChar == '\n')
			{
				continue;
			}

			// if character is an alphetical character(small or big), to low this character (as A -> a, a -> a)
			if (isalpha(currChar))
			{
				currChar = tolower(currChar);
			}

			// add character to model
			this->vocabularylist->add_char(currChar);

			// if character is a first character of file, continue to read next one. But if it is second, add occurunce to model
			if (currChar != 0 && prevChar != 0)
			{
				this->vocabularylist->add_occurence(prevChar, currChar);
			}

			// update previous character as current character for next step
			prevChar = currChar;
		}

		fclose(fd);
	}
	else
	{
		printf("Could not open file\n");
	}

#if DEBUG
	vocab_node *currNode = this->vocabularylist->head;

	while (true)
	{
		std::cout << currNode->character << " ";

		if (currNode->next == nullptr)
		{
			break;
		}

		currNode = currNode->next;
	}

	std::cout << "\n";
#endif
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
double language_model::calculateProbability(char firstChar, char secondChar)
{
	return static_cast<double>(this->vocabularylist->get_union_occurence(firstChar, secondChar)) / (double)(this->vocabularylist->get_occurence(firstChar));
}

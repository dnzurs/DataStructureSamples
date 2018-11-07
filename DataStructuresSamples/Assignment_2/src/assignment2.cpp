/* @Author
* Student Name: Nebi Deniz Uras
* Student ID : 504181527
* Date: 08.11.2018 */

//============================================================================//
//
//	File Name	: assignment2.cpp
//	Description	: Data Structures Homework 2
//
//============================================================================//

//============================================================================//
//=============================== INCLUDES ===================================//
//============================================================================//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

//============================================================================//
//==========================  MACRO DEFINITIONS ==============================//
//============================================================================//
#define FILE_NAME_LENGTH			(256)
#define TURKISH_CHAR_COUNT			(12) 
#define TURKISH_CHAR_COUNT_CMDLINE	(7)  
#define CHARACTER_COUNT				(32) 

#define DEBUG						(1) // If it is 1, the unique letters of model are shown

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
//------------------------------------------------------------------------------
struct char_set {
	char byte[2];
	char charCode;
};
//------------------------------------------------------------------------------
struct charTable {
	char character;
	char index;
	char charVal;
};
//============================================================================//
//========================== FUNCTION PROTOTYPES =============================//
//============================================================================//

//============================================================================//
//============================ GLOBAL VARIABLES ==============================//
//============================================================================//
// Map of Turkish Special Characters
static char_set specialTurkishCharSet[TURKISH_CHAR_COUNT] =
{
	{ {0x00, 0xc7}, -1 }, // Ç
	{ {0x00, 0xe7}, -1 }, // ç
	{ {0x01, 0x1e}, -2 }, // Ð
	{ {0x01, 0x1f}, -2 }, // ð
	{ {0x01, 0x30}, 0x69 }, // Ý
	{ {0x01, 0x31}, -3 }, // ý
	{ {0x00, 0xd6}, -4 }, // Ö
	{ {0x00, 0xf6}, -4 }, // ö
	{ {0x01, 0x5e}, -5 }, // Þ
	{ {0x01, 0x5f}, -5 }, // þ
	{ {0x00, 0xdc}, -6 }, // Ü
	{ {0x00, 0xfc}, -6 }, // ü
};

// Map of Turkish Special Characters for Command Line
static char_set specialTurkishCharSetCmdLine[TURKISH_CHAR_COUNT_CMDLINE] =
{
	{ { -57, -25},	-1 },   // Ç - ç
	{ { -48, -16 }, -2 },   // Ð - ð
	{ { -35, 0 },   0x69 }, // Ý
	{ { -3, 0 },	-3 },   // ý
	{ { -42, -10 }, -4 },   // Ö - ö
	{ { -34, -2 },  -5 },   // Þ - þ
	{ { -36, -4 },  -6 },   // Ü - ü
};

// Alphetical Sort Map for Characters
static charTable characterTable[CHARACTER_COUNT] =
{
	{ 'a',		-32,	0x61 },	// 
	{ 'b',		-31,	0x62 },	// 
	{ 'c',		-30,	0x63 },	// 
	{ 'ç',		-29,	-1 },	// 
	{ 'd',		-28,	0x64 },	// 
	{ 'e',		-27,	0x65 },	// 
	{ 'f',		-26,	0x66 },	// 
	{ 'g',		-25,	0x67 },	// 
	{ 'ð',		-24,	-2 },	// 
	{ 'h',		-23,	0x68 }, // 
	{ 'ý',		-22,	-3 },   // 
	{ 'i',		-21,	0x69 }, // 
	{ 'j',		-20,	0x6A }, // 
	{ 'k',		-19,	0x6B }, // 
	{ 'l',		-18,	0x6C }, // 
	{ 'm',		-17,	0x6D }, // 
	{ 'n',		-16,	0x6E }, // 
	{ 'o',		-15,	0x6F }, //
	{ 'ö',		-14,	-4 },   // 
	{ 'p',		-13,	0x70 }, // 
	{ 'q',		-12,	0x71 }, // 
	{ 'r',		-11,	0x72 }, // 
	{ 's',		-10,	0x73 }, // 
	{ 'þ',		-9,		-5 },   // 
	{ 't',		-8,		0x74 }, // 
	{ 'u',		-7,		0x75 }, // 
	{ 'ü',		-6,		-6 },   // 
	{ 'v',		-5,		0x76 }, // 
	{ 'w',		-4,		0x77 }, // 
	{ 'x',		-3,		0x78 }, // 
	{ 'y',		-2,		0x79 }, // 
	{ 'z',		-1,		0x7A }, //
};

//============================================================================//
//============================ PRIVATE FUNCTIONS =============================//
//============================================================================//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static bool isTurkishCharacter(char character)
{
	for (int i = 0; i < TURKISH_CHAR_COUNT; i++)
	{
		if (specialTurkishCharSet[i].byte[1] == character)
		{
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static char getTurkishCharCode(char *character)
{
	for (int i = 0; i < TURKISH_CHAR_COUNT; i++)
	{
		if (specialTurkishCharSet[i].byte[0] == character[0] && specialTurkishCharSet[i].byte[1] == character[1])
		{
			return specialTurkishCharSet[i].charCode;
		}
	}

	return 0;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static char getIndexOfChar(char character)
{
	for (int i = 0; i < CHARACTER_COUNT; i++)
	{
		if (character == characterTable[i].charVal)
		{
			return characterTable[i].index;
		}
	}

	return character;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static char getCharWithIndex(char index)
{
	for (int i = 0; i < CHARACTER_COUNT; i++)
	{
		if (index == characterTable[i].index)
		{
			return characterTable[i].character;
		}
	}

	return index;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static char getTurkishIndexOfCharFromCmdLine(char character)
{
	char charCode = 0;
	char index = character;


	for (int i = 0; i < TURKISH_CHAR_COUNT_CMDLINE; i++)
	{
		if (character == specialTurkishCharSetCmdLine[i].byte[0] || character == specialTurkishCharSetCmdLine[i].byte[1])
		{
			charCode = specialTurkishCharSetCmdLine[i].charCode;
			break;
		}
	}

	for (int i = 0; i < CHARACTER_COUNT; i++)
	{
		if (characterTable[i].charVal == charCode)
		{
			index = characterTable[i].index;
			break;
		}
	}

	return index;
}

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

	if (argc != 4 && argc != 2)
	{
		std::cout << "ERROR : Invalid Argument\n";
		system("Pause");

		return -1;
	}

	// get name of files that will be read
	std::memcpy(inputFile, argv[1], strlen(argv[1]));

	// get characters if there are
	if (argc == 4)
	{
		firstChar = getTurkishIndexOfCharFromCmdLine(*argv[2]);
		secondChar = getTurkishIndexOfCharFromCmdLine(*argv[3]);
	}

	// read data and create model
	model.readData(inputFile);

	// print model occurunce
	if (argc == 2)
	{
		model.vocabularylist->print();
	}

	// show probability of characters
	if (argc == 4)
	{
		std::cout << "P(" << getCharWithIndex(getIndexOfChar(firstChar)) << "|" << getCharWithIndex(getIndexOfChar(secondChar)) <<") = " << model.calculateProbability(getIndexOfChar(firstChar), getIndexOfChar(secondChar)) << "\n";
	}

#if _WIN32
	system("Pause");
#endif

    return 0;
}

//============================================================================//
//============================== CLASS FUNCTIONS =============================//
//============================================================================//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void vocab_list::create()
{
	this->head = (vocab_node *)malloc(sizeof(vocab_node));

	this->head->character = 0;
	this->head->next = NULL;
	this->head->list = NULL;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void vocab_list::print()
{
	vocab_node *currVocabNode = this->head;
	occur_node *currOccurNode = currVocabNode->list;
	char tempChar = 0;

	while (true)
	{
		if (currVocabNode->character != 0)
		{
			tempChar = getCharWithIndex(currVocabNode->character);

			if (tempChar == ' ')
			{
				std::cout << "<SP>" << ":\n";
			}
			else
			{
				std::cout << getCharWithIndex(currVocabNode->character) << ":\n";
			}

			while (true)
			{
				if (currVocabNode->list != NULL)
				{
					tempChar = getCharWithIndex(currOccurNode->character);

					if (tempChar == ' ')
					{
						std::cout << "\t<" << "<SP>" << "," << currOccurNode->occurence << ">";
					}
					else
					{
						std::cout << "\t<" << tempChar << "," << currOccurNode->occurence << ">";

					}

				}
				else
				{
					break;
				}

				std::cout << "\n";

				if (currOccurNode->next == NULL)
				{
					break;
				}

				currOccurNode = currOccurNode->next;
			}
		}

		if (currVocabNode->next == NULL)
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
	// if character isnt valid, dont do anything
	if (character == '\0')
	{
		return;
	}

	// to insert first value 
	if (this->head->character == 0 && this->head->next == NULL && this->head->list == NULL)
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

			if (currVocabNode->next == NULL)
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
				if (character > currVocabNode->character && currVocabNode->next == NULL)
				{
					currVocabNode->next = (vocab_node *)malloc(sizeof(vocab_node));

					currVocabNode->next->character = character;
					currVocabNode->next->next = NULL;
					currVocabNode->next->list = NULL;

					break;
				}
				else if (character < currVocabNode->character)
				{
					vocab_node *newNode = (vocab_node *)malloc(sizeof(vocab_node));

					newNode->character = currVocabNode->character;
					newNode->list = currVocabNode->list;
					newNode->next = currVocabNode->next;

					currVocabNode->character = character;
					currVocabNode->list = NULL;
					currVocabNode->next = newNode;

					break;
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
			if (currVocabNode->list == NULL)
			{
				currVocabNode->list = (occur_node *)malloc(sizeof(occur_node));

				currVocabNode->list->character = secondChar;
				currVocabNode->list->occurence = 1;
				currVocabNode->list->next = NULL;

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
						if (currOccurNode->next == NULL)
						{
							currOccurNode->next = (occur_node *)malloc(sizeof(occur_node));

							currOccurNode->next->character = secondChar;
							currOccurNode->next->occurence = 1;
							currOccurNode->next->next = NULL;

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

		if (currVocabNode->next == NULL)
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
	occur_node *currOccurNode = NULL;
	int occurence = 0;

	// find character in model
	while (true)
	{
		if (currVocabNode->character != 0)
		{
			if (currVocabNode->character == character)
			{
				currOccurNode = currVocabNode->list;
				break;
			}
		}

		if (currVocabNode->next == NULL)
		{
			break;
		}

		currVocabNode = currVocabNode->next;
	}

	// if there is character, find occurence
	if (currOccurNode != NULL)
	{
		while (true)
		{
			occurence += currOccurNode->occurence;

			if (currOccurNode->next == NULL)
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
	occur_node *currOccurNode = NULL;
	int co_occurence = 0;

	// find first character in model
	while (true)
	{
		if (currVocabNode->character != 0)
		{
			if (currVocabNode->character == firstChar)
			{
				currOccurNode = currVocabNode->list;
				break;
			}
		}

		if (currVocabNode->next == NULL)
		{
			break;
		}

		currVocabNode = currVocabNode->next;
	}

	// if there is occurence with second character, find co_occurence
	if (currOccurNode != NULL)
	{
		while (true)
		{
			if (currOccurNode->character == secondChar)
			{
				co_occurence = currOccurNode->occurence;
			}
			
			if (currOccurNode->next == NULL)
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
	FILE *fd = NULL;

	// open as UTF-8 file because of special character
	fd = fopen(inputFile, "r, ccs=UTF-8");
	if (fd != NULL)
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

			// if there is newline or null characters, continue for next byte
			if (currChar == '\n' || currChar == '\0') 
			{
				if (currChar == '\n') // if it is end of line, reset the characters for adding characters of new sentence
				{
					currChar = 0;
					prevChar = 0;
				}
				continue;
			}

			// check if character is special Turkish char, manipulate the character
			if (isTurkishCharacter(currChar))
			{
				char specialTurkishChar[2] = { 0 };

				specialTurkishChar[1] = currChar;
				fread(&specialTurkishChar[0], sizeof(char), 1, fd);
				
				currChar = getTurkishCharCode(specialTurkishChar);
			}
			else
			{
				// if character is an alphetical character(small or big), to low this character (as A -> a, a -> a)
				if (currChar > 0)
				{
					if (isalpha(currChar))
					{
						currChar = tolower(currChar);
					}
				}
			}

			// add character to model according to character sort map
			this->vocabularylist->add_char(getIndexOfChar(currChar));

			// if character is a first character of file, continue to read next one. But if it is second, add occurunce to model
			if (currChar != 0 && prevChar != 0)
			{
				this->vocabularylist->add_occurence(getIndexOfChar(prevChar), getIndexOfChar(currChar));
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
		std::cout << getCharWithIndex(currNode->character) << " ";

		if (currNode->next == NULL)
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
	double probability = 0;

	if (this->vocabularylist->get_union_occurence(secondChar, firstChar) > 0 && this->vocabularylist->get_occurence(secondChar) > 0)
	{
		probability = static_cast<double>(this->vocabularylist->get_union_occurence(secondChar, firstChar)) / (double)(this->vocabularylist->get_occurence(secondChar));
	}

	return probability;
}

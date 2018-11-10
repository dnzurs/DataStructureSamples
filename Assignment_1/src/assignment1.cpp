//============================================================================//
//
//	File Name	: assignment1.cpp
//	Create Date	: 16.10.2018
//	Designers	: Nebi Deniz Uras
//	Number 	    : 504181527
//	Description	: Data Structures Homework 1
//
//	Important Notes:
//
//============================================================================//
#define _CRT_SECURE_NO_WARNINGS
//============================================================================//
//=============================== INCLUDES ===================================//
//============================================================================//
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

//============================================================================//
//==========================  MACRO DEFINITIONS ==============================//
//============================================================================//
#define FILE_NAME_LENGTH	(256)
#define OPERATOR_COUNT		(10)	// initial operator count
#define SPACE_CHARACTER		(' ')	// space character for empty grid

#define DEBUG				(0) //TODO : showGrid silinecek
#define FILE_OPERATION		(0) //TODO : showGrid silinecek

//============================================================================//
//=========================== TYPE DEFINITIONS ===============================//
//============================================================================//
struct Operators
{
	int centerRow;
	int centerColumn;
	int size;
	char type;
	int memberCount;
	int *members;
};

struct Grid
{
	int row;
	int column;
	char *table;
};

struct Instruction
{
	char process[4];
	int row;
	int column;
	int size;
};

//============================================================================//
//========================== FUNCTION PROTOTYPES =============================//
//============================================================================//

/*	Grid Initilization Functions */
static int checkBorderError(Operators *addOperator);
static int checkConflictError(Operators *addOperator);
static int addOperatorsToGrid(int operatorIndex);
static int gridInit(void);

/*	Instruction Functions */
static bool isMemberOfOperator(int indexOfMember, int index);
static int getIndexOfOperator(void);
static int moveRight(void);
static int moveLeft(void);
static int moveUp(void);
static int moveDown(void);
static int doInstructions(void);;

//============================================================================//
//============================ GLOBAL VARIABLES ==============================//
//============================================================================//
#if FILE_OPERATION
char gridFileName[FILE_NAME_LENGTH]			= {0};
char instructionsFileName[FILE_NAME_LENGTH]	= {0};
#else
char gridFileName[FILE_NAME_LENGTH]			= "C:/Users/USER/Desktop/ITU/Homework1/assignment1_tests/grid_no_error_jump.txt";
char instructionsFileName[FILE_NAME_LENGTH]	= "C:/Users/USER/Desktop/ITU/Homework1/assignment1_tests/inst_no_error_jump.txt";
#endif

static int			operatorCount = 0;
static Operators	*operators = NULL;
static Grid			grid;
static Instruction	instruction;

#if DEBUG 
static void showGrid(void)
{
	printf(" ");
	unsigned char buffer[1000] = { 0 };

	memcpy(buffer, grid.table, grid.row * grid.column);

	for (int i = 0; i < grid.column; i++)
	{
		printf("%d", i + 1);
	}

	for (int i = 0; i < grid.row * grid.column; i++)
	{
		if (i % grid.column == 0)
			printf("\n%d", i / grid.column + 1);

		printf("%c", *(grid.table + i));
	}

	printf("\n");
}
#endif

#if DEBUG
// Sil
showGrid();
for (int i = 0; i < operatorCount; i++)
{
	printf("\n%c %d %d %d", operators[i].type, operators[i].centerRow, operators[i].centerColumn, operators[i].size);

	printf(" ---- Member Locations -> ");
	for (int j = 0; j < operators[i].memberCount; j++)
		printf("%d ", operators[i].members[j]);
}
printf("\n");
//
#endif

//============================================================================//
//============================ PUBLIC FUNCTIONS ==============================//
//============================================================================//

int main(int argc, char *argv[])
{
#if FILE_OPERATION
	if(argc < 2)
	{
		printf("Invalid Operation\n");
		return -1;
	}
	
	// get name of files that will be read
	memcpy(gridFileName, argv[1], strlen(argv[1]));
	memcpy(instructionsFileName, argv[2], strlen(argv[2]));
#endif 

	gridInit();	
	doInstructions();

	/* free all dynamic memories */
	for (int i = 0; i < operatorCount; i++)
	{
		free(operators[i].members);
	}
	free(operators);
	free(grid.table);
	
	getchar();
    return 0;
}

//============================================================================//
//============================ PRIVATE FUNCTIONS =============================//
//============================================================================//

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int checkBorderError(Operators *addOperator)
{
	int retVal = 0;

	if (addOperator->type == '+')
	{
		// check center coordination not to over flow from border
		if (addOperator->centerRow > grid.row || addOperator->centerColumn > grid.column)
		{
			retVal = -1;
		}
		else
		{
			// check other coordinates according to size not to over flow from border
			for (int i = 1; i <= addOperator->size; i++)
			{
				if ((addOperator->centerRow - i) < 1) // check up
				{
					retVal = -1;
				}

				if ((addOperator->centerRow + i) > grid.row) // check down
				{
					retVal = -1;
				}

				if ((addOperator->centerColumn + i) > grid.column) // check right
				{
					retVal = -1;
				}

				if ((addOperator->centerColumn - i) < 1) // check left
				{
					retVal = -1;
				}
			}
		}
	}

	if (addOperator->type == '-')
	{
		// check center coordination not to over flow from border
		if (addOperator->centerRow > grid.row || addOperator->centerColumn > grid.column)
		{
			retVal = -1;
		}
		else
		{
			// check other coordinates according to size not to over flow from border
			for (int i = 1; i <= addOperator->size; i++)
			{
				if ((addOperator->centerColumn + i) > grid.column) // check right
				{
					retVal = -1;
				}

				if ((addOperator->centerColumn - i) < 1) // check left
				{
					retVal = -1;
				}
			}
		}
	}

	if (addOperator->type == '/')
	{
		// check center coordination not to over flow from border
		if (addOperator->centerRow > grid.row || addOperator->centerColumn > grid.column)
		{
			retVal = -1;
		}
		else
		{
			// check other coordinates according to size not to over flow from border
			for (int i = 1; i <= addOperator->size; i++)
			{
				if ((addOperator->centerRow - i) < 1) // check up
				{
					retVal = -1;
				}

				if ((addOperator->centerRow + i) > grid.row) // check down
				{
					retVal = -1;
				}

				if ((addOperator->centerColumn + i) > grid.column) // check right
				{
					retVal = -1;
				}

				if ((addOperator->centerColumn - i) < 1) // check left
				{
					retVal = -1;
				}
			}
		}
	}

	if (addOperator->type == 'x')
	{
		// check center coordination not to over flow from border
		if (addOperator->centerRow > grid.row || addOperator->centerColumn > grid.column)
		{
			retVal = -1;
		}
		else
		{
			// check other coordinates according to size not to over flow from border
			for (int i = 1; i <= addOperator->size; i++)
			{
				if ((addOperator->centerRow - i) < 1) // check up
				{
					retVal = -1;
				}

				if ((addOperator->centerRow + i) > grid.row) // check down
				{
					retVal = -1;
				}

				if ((addOperator->centerColumn + i) > grid.column) // check right
				{
					retVal = -1;
				}

				if ((addOperator->centerColumn - i) < 1) // check left
				{
					retVal = -1;
				}
			}
		}
	}

	if (retVal == -1)
	{
		printf("BORDER ERROR: Operator %c with size %d can not be placed on (%d,%d).\n", addOperator->type, addOperator->size, addOperator->centerRow, addOperator->centerColumn);
	}

	return retVal;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int checkConflictError(Operators *addOperator)
{
	int retVal = 0;

	if (addOperator->type == '+')
	{
		// check center coordination
		if (*(grid.table - 1 + ((addOperator->centerRow - 1) * grid.column) + addOperator->centerColumn) != SPACE_CHARACTER)
		{
			retVal = -1;
		}
		else
		{
			// check other coordinates according to size
			for (int i = 1; i <= addOperator->size; i++)
			{
				if (*(grid.table - 1 + ((addOperator->centerRow - 1 - i) * grid.column) + addOperator->centerColumn) != SPACE_CHARACTER) // check up
				{
					retVal = -1;
				}

				if (*(grid.table - 1 + ((addOperator->centerRow - 1 + i) * grid.column) + addOperator->centerColumn) != SPACE_CHARACTER) // check down
				{
					retVal = -1;
				}

				if (*(grid.table - 1 + ((addOperator->centerRow - 1) * grid.column) + addOperator->centerColumn + i) != SPACE_CHARACTER) // check right
				{
					retVal = -1;
				}

				if (*(grid.table - 1 + ((addOperator->centerRow - 1) * grid.column) + addOperator->centerColumn - i) != SPACE_CHARACTER) // check left
				{
					retVal = -1;
				}
			}
		}
	}

	if (addOperator->type == '-')
	{
		// check center coordination
		if (*(grid.table - 1 + ((addOperator->centerRow - 1) * grid.column) + addOperator->centerColumn) != SPACE_CHARACTER)
		{
			retVal = -1;
		}
		else
		{
			// check other coordinates according to size
			for (int i = 1; i <= addOperator->size; i++)
			{
				if (*(grid.table - 1 + ((addOperator->centerRow - 1) * grid.column) + addOperator->centerColumn + i) != SPACE_CHARACTER) // check right
				{
					retVal = -1;
				}

				if (*(grid.table - 1 + ((addOperator->centerRow - 1) * grid.column) + addOperator->centerColumn - i) != SPACE_CHARACTER) // check left
				{
					retVal = -1;
				}
			}
		}
	}

	if (addOperator->type == '/')
	{
		// check center coordination
		if (*(grid.table - 1 + ((addOperator->centerRow - 1) * grid.column) + addOperator->centerColumn) != SPACE_CHARACTER)
		{
			retVal = -1;
		}
		else
		{
			// check other coordinates according to size
			for (int i = 1; i <= addOperator->size; i++)
			{
				if (*(grid.table - 1 + ((addOperator->centerRow - 1 - i) * grid.column) + addOperator->centerColumn + i) != SPACE_CHARACTER) // check up right
				{
					retVal = -1;
				}

				if (*(grid.table - 1 + ((addOperator->centerRow - 1 + i) * grid.column) + addOperator->centerColumn - i) != SPACE_CHARACTER) // check down left
				{
					retVal = -1;
				}
			}
		}
	}

	if (addOperator->type == 'x')
	{
		// check center coordination
		if (*(grid.table - 1 + ((addOperator->centerRow - 1) * grid.column) + addOperator->centerColumn) != SPACE_CHARACTER)
		{
			retVal = -1;
		}
		else
		{
			// check other coordinates according to size
			for (int i = 1; i <= addOperator->size; i++)
			{
				if (*(grid.table - 1 + ((addOperator->centerRow - 1 - i) * grid.column) + addOperator->centerColumn + i) != SPACE_CHARACTER) // check up right
				{
					retVal = -1;
				}

				if (*(grid.table - 1 + ((addOperator->centerRow - 1 - i) * grid.column) + addOperator->centerColumn - i) != SPACE_CHARACTER) // check up left
				{
					retVal = -1;
				}

				if (*(grid.table - 1 + ((addOperator->centerRow - 1 + i) * grid.column) + addOperator->centerColumn + i) != SPACE_CHARACTER) // check down right
				{
					retVal = -1;
				}

				if (*(grid.table - 1 + ((addOperator->centerRow - 1 + i) * grid.column) + addOperator->centerColumn - i) != SPACE_CHARACTER) // check down left
				{
					retVal = -1;
				}
			}
		}
	}

	if (retVal == -1)
	{
		printf("CONFLICT ERROR: Operator %c with size %d can not be placed on (%d,%d).\n", addOperator->type, addOperator->size, addOperator->centerRow, addOperator->centerColumn);
	}

	return retVal;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int addOperatorsToGrid(int operatorIndex)
{
	int retVal = -1;

	if (operators[operatorIndex].type == '+')
	{
		// add operators to center
		*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn) = operators[operatorIndex].type;
		*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn - 1;
		operators[operatorIndex].memberCount++;

		// add operators to other coordinates according to size
		for (int i = 1; i <= operators[operatorIndex].size; i++)
		{
			// add up
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1 - i) * grid.column) + operators[operatorIndex].centerColumn) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1 - i) * grid.column) + operators[operatorIndex].centerColumn - 1;
			operators[operatorIndex].memberCount++;

			// add down
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1 + i) * grid.column) + operators[operatorIndex].centerColumn) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1 + i) * grid.column) + operators[operatorIndex].centerColumn - 1;
			operators[operatorIndex].memberCount++;

			// add right
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn + i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn + i - 1;
			operators[operatorIndex].memberCount++;

			// add left
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn - i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn - i - 1;
			operators[operatorIndex].memberCount++;
		}

		retVal = 0;
	}

	if (operators[operatorIndex].type == '-')
	{
		// add operators to center
		*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn) = operators[operatorIndex].type;
		*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn - 1;
		operators[operatorIndex].memberCount++;

		// add operators to other coordinates according to size
		for (int i = 1; i <= operators[operatorIndex].size; i++)
		{
			// add right
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn + i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn + i - 1;
			operators[operatorIndex].memberCount++;

			// add left
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn - i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn - i - 1;
			operators[operatorIndex].memberCount++;
		}

		retVal = 0;
	}

	if (operators[operatorIndex].type == '/')
	{
		// add operators to center
		*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn) = operators[operatorIndex].type;
		*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn - 1;
		operators[operatorIndex].memberCount++;

		// add operators to other coordinates according to size
		for (int i = 1; i <= operators[operatorIndex].size; i++)
		{
			// add up right
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1 - i) * grid.column) + operators[operatorIndex].centerColumn + i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1 - i) * grid.column) + operators[operatorIndex].centerColumn + i - 1;
			operators[operatorIndex].memberCount++;

			// add down left
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1 + i) * grid.column) + operators[operatorIndex].centerColumn - i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1 + i) * grid.column) + operators[operatorIndex].centerColumn - i - 1;
			operators[operatorIndex].memberCount++;
		}

		retVal = 0;
	}

	if (operators[operatorIndex].type == 'x')
	{
		// add operators to center
		*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn) = operators[operatorIndex].type;
		*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1) * grid.column) + operators[operatorIndex].centerColumn - 1;
		operators[operatorIndex].memberCount++;

		// add operators to other coordinates according to size
		for (int i = 1; i <= operators[operatorIndex].size; i++)
		{
			// add up right
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1 - i) * grid.column) + operators[operatorIndex].centerColumn + i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1 - i) * grid.column) + operators[operatorIndex].centerColumn + i - 1;
			operators[operatorIndex].memberCount++;

			// add up left																		   
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1 - i) * grid.column) + operators[operatorIndex].centerColumn - i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1 - i) * grid.column) + operators[operatorIndex].centerColumn - i - 1;
			operators[operatorIndex].memberCount++;

			// add down right																	   
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1 + i) * grid.column) + operators[operatorIndex].centerColumn + i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1 + i) * grid.column) + operators[operatorIndex].centerColumn + i - 1;
			operators[operatorIndex].memberCount++;

			// add down left																	   
			*(grid.table - 1 + ((operators[operatorIndex].centerRow - 1 + i) * grid.column) + operators[operatorIndex].centerColumn - i) = operators[operatorIndex].type;
			*(operators[operatorIndex].members + operators[operatorIndex].memberCount) = ((operators[operatorIndex].centerRow - 1 + i) * grid.column) + operators[operatorIndex].centerColumn - i - 1;
			operators[operatorIndex].memberCount++;
		}

		retVal = 0;
	}

	return retVal;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int gridInit(void)
{
	int retVal = 0;
	FILE *fd = NULL;

	fd = fopen(gridFileName, "r");
	if (fd != NULL)
	{
		// read row and column of grid
		fscanf(fd, "%d%d", &grid.row, &grid.column);

		// create dynamic array for grid
		grid.table = (char *)malloc((grid.row * grid.column) * sizeof(char));

		// check dynamic array. If it is succesfull, initialize grid
		if (grid.table != NULL)
		{
			Operators tempOperator;

			memset(&tempOperator, 0, sizeof(Operators));

			// firstly, set grid as space character for controls
			memset(grid.table, ' ', (grid.row * grid.column) * sizeof(char));

			// get memory for operator infos
			operators = (Operators *)malloc(OPERATOR_COUNT * sizeof(Operators)); // get memory for 5 operator infos initially

			printf("A grid is created: %d %d\n", grid.row, grid.column);

			// read grid file and add operators to grid
			while (1)
			{
				fscanf(fd, "%s%d%d%d", &tempOperator.type, &tempOperator.centerRow, &tempOperator.centerColumn, &tempOperator.size);

				// if it is the last line of file, dont continue to process (protection for twice times)
				if (feof(fd))
				{
					break;
				}

				if (checkBorderError(&tempOperator) == -1 || checkConflictError(&tempOperator) == -1)
				{
					retVal = -1;
				}
				else
				{
					// if operator count is bigger than current count of dynamic object, reallocation for infos
					static int count = OPERATOR_COUNT;
					if ((operatorCount + 1) >= count)
					{
						operators = (Operators *)realloc(operators, (OPERATOR_COUNT + operatorCount) * sizeof(Operators));
						count += OPERATOR_COUNT;
					}

					// increase operator count and copy operator to struct for finding center and other things
					operators[operatorCount].centerColumn = tempOperator.centerColumn;
					operators[operatorCount].centerRow = tempOperator.centerRow;
					operators[operatorCount].size = tempOperator.size;
					operators[operatorCount].type = tempOperator.type;
					operators[operatorCount].memberCount = 0;
					operators[operatorCount].members = NULL;

					if (operators[operatorCount].type == '+' || operators[operatorCount].type == 'x')
					{
						// get memory for members
						operators[operatorCount].members = (int *)malloc((4 * operators[operatorCount].size + 1) * sizeof(int));
					}

					if (operators[operatorCount].type == '-' || operators[operatorCount].type == '/')
					{
						// get memory for members
						operators[operatorCount].members = (int *)malloc((2 * operators[operatorCount].size + 1) * sizeof(int));
					}

					retVal = addOperatorsToGrid(operatorCount);

					if (retVal == 0)
					{
						printf("SUCCESS: Operator %c with size %d is placed on (%d,%d).\n", operators[operatorCount].type, operators[operatorCount].size,
																						   operators[operatorCount].centerRow, operators[operatorCount].centerColumn);
						operatorCount++;
					}
				}
			}
		}
		else
		{
			printf("A grid is not created !!!\n");
			retVal = -1;
		}

		fclose(fd);
	}
	else
	{
		printf("File is not opened\n");
		retVal = -1;
	}

	return retVal;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static bool isMemberOfOperator(int indexOfMember, int index)
{
	bool retVal = false;

	for (int i = 0; i <= operators[index].memberCount; i++)
	{
		if (indexOfMember == operators[index].members[i])
		{
			retVal = true;
			break;
		}
	}

	return retVal;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int getIndexOfOperator(void)
{
	int operatorIndex = 0;
	char type = *(grid.table + (instruction.row - 1) * grid.column + instruction.column - 1);
	int indexOfMember = (instruction.row - 1) * grid.column + instruction.column - 1;

	for (int i = 0; i < operatorCount; i++)
	{
		if (type == operators[i].type)
		{
			if (isMemberOfOperator(indexOfMember, i) == true)
			{
				operatorIndex = i;
				break;
			}
		}
	}

	return operatorIndex;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int moveRight(void)
{
	int retVal = 0;
	int operatorIndex = getIndexOfOperator();
	int row = 0;
	int col = 0;

	// check border for all members
	for (int i = 0; i < operators[operatorIndex].memberCount; i++)
	{
		row = operators[operatorIndex].members[i] / grid.column + 1;
		col = operators[operatorIndex].members[i] - (row - 1) * grid.column + 1;

		if (col + instruction.size > grid.column)
		{
			retVal = -1;
			printf("BORDER ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
				operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn + instruction.size);
			break;
		}
	}

	// check conflict all members
	for (int i = 0; i < operators[operatorIndex].memberCount; i++)
	{
		if (grid.table[operators[operatorIndex].members[i] + instruction.size] != SPACE_CHARACTER)
		{
			if (isMemberOfOperator(operators[operatorIndex].members[i] + instruction.size, operatorIndex) == false)
			{
				retVal = -1;
				printf("CONFLICT ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
					operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn + instruction.size);
				break;
			}
		}
	}

	// move members to new location
	if (retVal == 0)
	{
		printf("SUCCESS: %c moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
			operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn + instruction.size);

		for (int i = 0; i < operators[operatorIndex].memberCount; i++)
		{
			grid.table[operators[operatorIndex].members[i]] = SPACE_CHARACTER;
			operators[operatorIndex].members[i] += instruction.size;
		}

		operators[operatorIndex].memberCount = 0;
		operators[operatorIndex].centerColumn += instruction.size;

		addOperatorsToGrid(operatorIndex);
	}

	return retVal;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int moveLeft(void)
{
	int retVal = 0;
	int operatorIndex = getIndexOfOperator();
	int row = 0;
	int col = 0;

	// check border for all members
	for (int i = 0; i < operators[operatorIndex].memberCount; i++)
	{
		row = operators[operatorIndex].members[i] / grid.column + 1;
		col = operators[operatorIndex].members[i] - (row - 1) * grid.column + 1;

		if (col - instruction.size < 1)
		{
			retVal = -1;
			printf("BORDER ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
				operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn - instruction.size);
			break;
		}
	}

	// check conflict all members
	for (int i = 0; i < operators[operatorIndex].memberCount; i++)
	{
		if (grid.table[operators[operatorIndex].members[i] - instruction.size] != SPACE_CHARACTER)
		{
			if (isMemberOfOperator(operators[operatorIndex].members[i] - instruction.size, operatorIndex) == false)
			{
				retVal = -1;
				printf("CONFLICT ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
					operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn - instruction.size);
				break;
			}
		}
	}

	// move members to new location
	if (retVal == 0)
	{
		printf("SUCCESS: %c moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
			operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn - instruction.size);

		for (int i = 0; i < operators[operatorIndex].memberCount; i++)
		{
			grid.table[operators[operatorIndex].members[i]] = SPACE_CHARACTER;
			operators[operatorIndex].members[i] -= instruction.size;
		}

		operators[operatorIndex].memberCount = 0;
		operators[operatorIndex].centerColumn -= instruction.size;

		addOperatorsToGrid(operatorIndex);
	}

	return retVal;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int moveUp(void)
{
	int retVal = 0;
	int operatorIndex = getIndexOfOperator();
	int row = 0;
	int col = 0;

	// check border for all members
	for (int i = 0; i < operators[operatorIndex].memberCount; i++)
	{
		row = operators[operatorIndex].members[i] / grid.column + 1;
		col = operators[operatorIndex].members[i] - (row - 1) * grid.column + 1;

		if (row - instruction.size < 1)
		{
			retVal = -1;
			printf("BORDER ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
				operators[operatorIndex].centerRow - instruction.size, operators[operatorIndex].centerColumn);
			break;
		}
	}

	// check conflict all members
	for (int i = 0; i < operators[operatorIndex].memberCount; i++)
	{
		if (grid.table[operators[operatorIndex].members[i] - instruction.size * grid.column] != SPACE_CHARACTER)
		{
			if (isMemberOfOperator(operators[operatorIndex].members[i] - instruction.size * grid.column, operatorIndex) == false)
			{
				retVal = -1;
				printf("CONFLICT ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
					operators[operatorIndex].centerRow - instruction.size, operators[operatorIndex].centerColumn);
				break;
			}
		}
	}

	// move members to new location
	if (retVal == 0)
	{
		printf("SUCCESS: %c moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
			operators[operatorIndex].centerRow - instruction.size, operators[operatorIndex].centerColumn);

		for (int i = 0; i < operators[operatorIndex].memberCount; i++)
		{
			grid.table[operators[operatorIndex].members[i]] = SPACE_CHARACTER;
			operators[operatorIndex].members[i] -= instruction.size * grid.column;
		}

		operators[operatorIndex].memberCount = 0;
		operators[operatorIndex].centerRow -= instruction.size;

		addOperatorsToGrid(operatorIndex);
	}

	return retVal;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int moveDown(void)
{
	int retVal = 0;
	int operatorIndex = getIndexOfOperator();
	int row = 0;
	int col = 0;

	// check border for all members
	for (int i = 0; i < operators[operatorIndex].memberCount; i++)
	{
		row = operators[operatorIndex].members[i] / grid.column + 1;
		col = operators[operatorIndex].members[i] - (row - 1) * grid.column + 1;

		if (row + instruction.size > grid.row)
		{
			retVal = -1;
			printf("BORDER ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
				operators[operatorIndex].centerRow + instruction.size, operators[operatorIndex].centerColumn);
			break;
		}
	}

	// check conflict all members
	for (int i = 0; i < operators[operatorIndex].memberCount; i++)
	{
		if (grid.table[operators[operatorIndex].members[i] + instruction.size * grid.column] != SPACE_CHARACTER)
		{
			if (isMemberOfOperator(operators[operatorIndex].members[i] + instruction.size * grid.column, operatorIndex) == false)
			{
				retVal = -1;
				printf("CONFLICT ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
					operators[operatorIndex].centerRow + instruction.size, operators[operatorIndex].centerColumn);
				break;
			}
		}
	}

	// move members to new location
	if (retVal == 0)
	{
		printf("SUCCESS: %c moved from (%d,%d) to (%d,%d).\n", operators[operatorIndex].type, operators[operatorIndex].centerRow, operators[operatorIndex].centerColumn,
			operators[operatorIndex].centerRow + instruction.size, operators[operatorIndex].centerColumn);

		for (int i = 0; i < operators[operatorIndex].memberCount; i++)
		{
			grid.table[operators[operatorIndex].members[i]] = SPACE_CHARACTER;
			operators[operatorIndex].members[i] += instruction.size * grid.column;
		}

		operators[operatorIndex].memberCount = 0;
		operators[operatorIndex].centerRow += instruction.size;

		addOperatorsToGrid(operatorIndex);
	}

	return retVal;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static int doInstructions(void)
{
	int retVal = 0;
	FILE *fd = NULL;

	fd = fopen(instructionsFileName, "r+");
	if (fd != NULL)
	{
		// read instruction file and do instructions
		while (true)
		{
			fscanf(fd, "%s%d%d%d", &instruction.process, &instruction.row, &instruction.column, &instruction.size);
			
			// if it is the last line of file, dont continue to process (protection for twice times)
			if (feof(fd))
			{
				break;
			}

			if (strcmp((const char *)instruction.process, (const char *)"MVU") == 0)
			{
				moveUp();
			}

			if (strcmp((const char *)instruction.process, (const char *)"MVD") == 0)
			{
				moveDown();
			}

			if (strcmp((const char *)instruction.process, (const char *)"MVR") == 0)
			{
				moveRight();
			}

			if (strcmp((const char *)instruction.process, (const char *)"MVL") == 0)
			{
				moveLeft();
			}
		}

		fclose(fd);
	}
	else
	{
		printf("File is not opened\n");
		retVal = -1;
	}

	return retVal;
}

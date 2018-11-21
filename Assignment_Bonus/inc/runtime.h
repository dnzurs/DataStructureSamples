/*
 * runtime.h
 *
 *      Author: Nebi Deniz Uras
 *      Number: 507181527
 */

#ifndef RUNTIME_H_
#define RUNTIME_H_

 //============================================================================//
 //=============================== INCLUDES ===================================//
 //============================================================================//
#include <string>

 //============================================================================//
 //==========================  MACRO DEFINITIONS ==============================//
 //============================================================================//

 //============================================================================//
 //=========================== TYPE DEFINITIONS ===============================//
 //============================================================================//
struct Statement
{
	std::string type;
	std::string arg1;
	std::string arg2;
};

typedef unsigned AddressValue;
typedef int ScalarValue;

template<typename T>
struct Stack
{
	//implement your stack here
};
 //============================================================================//
 //========================== FUNCTION PROTOTYPES =============================//
 //============================================================================//
AddressValue executeStatement(	const Statement & statement,
								const AddressValue currentAddress,
								ScalarValue variables[5],
								Stack<ScalarValue> & callStack,
								const AddressValue addressOfCalleeIfCall, 
								bool & isDone);

#endif /* RUNTIME_H_ */

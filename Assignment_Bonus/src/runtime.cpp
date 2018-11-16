/*
 * runtime.cpp
 *
 *      Author: Nebi Deniz Uras
 *      Number: 507181527
 */

#include "runtime.h"
#include <iostream>

AddressValue executeStatement( const Statement & statement, 
			       const AddressValue currentAddress, 
			       ScalarValue variables [5], 
			       Stack<ScalarValue> & callStack, 
			       const AddressValue addressOfCalleeIfCall, 
			       bool & isDone )
{
    (void) variables;
    (void) callStack;
    (void) isDone;
    (void) addressOfCalleeIfCall;
    
    //Your entry point is here
    std::cout << "Executing " << statement.type << " at line " << currentAddress << std::endl;
    
    //Return what address you want to go next
    return currentAddress + 1;
}

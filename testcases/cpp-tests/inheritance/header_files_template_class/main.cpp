#include "derived.h"

int main()
{
    Derived<int> d;
    return 1;
}

// Since ROSE generates a new node from a template class instance, the class is found in the AST.
// Method definitions only appear in the AST if they are called on the instantiated object
// If a method is not called, it will appear as undefined in the AST, artificially inflating the LCOM value

// Expected LCOM4 = 1
// Result from tool: LCOM4 = 1 if --lcom:filter-undefined-methods is set, 2 otherwise
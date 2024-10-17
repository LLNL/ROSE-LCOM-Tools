#include "derived.h"

int main()
{
    Derived d;
    return 1;
}

// ROSE will not accept .h files as input. As a result, "Derived" is not found in the AST. 

// Expected LCOM4 = null (no class found)
// Result from tool: LCOM4 = null
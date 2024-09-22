#include "derived.h"

int main()
{
    Derived<int> d;
    d.Method1();
    d.Method2();
    d.Method3();
    d.Method4();
    return 1;
}

// This test checks what happens when a template class is included from a header file
// Due to the behavior of the C++ AST, we also analyze the bahavior of inheritance in this test

// Since ROSE generates a new node from a template class instance, the class is found in the AST.
// Method definitions only appear in the AST if they are called on the instantiated object
// If a method is not called, it will appear as undefined in the AST, artificially inflating the LCOM value

// Since Method1 and Method2 are from an inherited class, we expect them to not count against the LCOM value

// Expected LCOM4 = 1
// Result from tool: LCOM4 = 1
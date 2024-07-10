#include "derived.h"

int main()
{
    Derived<int> d;
    return 1;
}

// Since ROSE generates a new node from a template class instance, the class is found, but the LCOM4 is calculated incorrectly.
// Expected LCOM4 = 1
// Result from tool: LCOM4 = 2 TODO: test failed (Same reason as template_instantiation.cpp)
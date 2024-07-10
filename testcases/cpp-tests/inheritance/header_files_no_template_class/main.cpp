#include "derived.h"

int main()
{
    Derived d;
    return 1;
}

// Expected LCOM4 = 1
// Result from tool: LCOM4 = null TODO: test failed (class not found) (actually, this is probably intended behavior, as the class is in a header file, not in the main file)
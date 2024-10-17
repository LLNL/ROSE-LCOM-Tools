namespace myNamespace
{
    int x = 1;

    int methodOne() {
        return x;
    }
}

namespace myNamespace
{
    int y = 2;

    int methodTwo() {
        return x + y;
    }
}

// Expected LCOM4 of myNamespace = 1
// Result: LCOM4 of myNamespace = 1
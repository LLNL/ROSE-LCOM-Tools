namespace myNamespace
{
    int y = 2;

    int methodTwo() {
        return y;
    }
}

// Expected LCOM4 of myNamespace if namespace2.cpp is included = 2
// Result: LCOM4 of myNamespace = 2

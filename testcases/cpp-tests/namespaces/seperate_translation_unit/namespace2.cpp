namespace myNamespace
{
    int x = 1;
    
    int methodOne() {
        return x;
    }
}

// Expected LCOM4 of myNamespace if namespace.cpp is included = 2
// Result: LCOM4 of myNamespace = 2
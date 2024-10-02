class Function {
    static int i, j;

    static int method() {
        return Function::i;
    }

    int (& aliasMethod)() = method;

    int method2() {
        return aliasMethod();
    }
};

// Function aliases don't count: LCOM4 = 2
// Functions aliases count:      LCOM4 = 1
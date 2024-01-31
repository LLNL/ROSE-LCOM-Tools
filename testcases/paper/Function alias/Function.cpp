class Function {
    int i, j;

    static int method() {
        return i;
    }

    static int (*aliasMethod)() = &Function::method;

    int method2() {
        j = 2;
        auto& aliasMethod = method;
        return aliasMethod();
    }
};

// Function aliases don't count: LCOM4 = 2
// Functions aliases count:      LCOM4 = 1
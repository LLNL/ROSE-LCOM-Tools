class Variable {
    int i, j;
    int& k = i;

    int method() {
        return i;
    }

    int method2() {
        j = k;
        return j;
    }
};

// Variable aliases don't count: LCOM4 = 2
// Variable aliases count:       LCOM4 = 1
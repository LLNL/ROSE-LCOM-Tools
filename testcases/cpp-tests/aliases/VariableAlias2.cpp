class Variable {
    int i = 10;
    int *j = &i;
    int& k = *j;

    int Method1() {
        return i;
    }

    int* Method2() {
        return j;
    }

    int Method3() {
        return k;
    }
};
class Variable {
    int i, l;
    int& k = i;
    int& j = k;

    int Method1() {
        return i;
    }

    int Method2() {
        return k;
    }

    int Method3() {
        return j;
    }

    int Method4() {
        return l;
    }
};
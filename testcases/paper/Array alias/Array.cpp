class Array {
    int i [50];
    int(&j)[50] = i;

    void method1() {
        i[1] = 1;
    }

    void method2() {
        i[2] = 2;
    }

    void method3() {
        i[3] = 3;
    }

    void aliasMethod() {
        j[1] = 3; 
    }
};

// Alias doesn't count: LCOM4 = 2 or 4
// Alias counts:        LCOM4 = 1 or 3
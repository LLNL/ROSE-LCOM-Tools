class Constructors {
    int i, j;
    
    Constructors() {
        i = 1;
    }

    Constructors(int _i) : i(_i) {}

    Constructors(double _j) : j(_j) {}

    void method() {
        j = 2;
    }
};

// Constructors don't count: LCOM4 = 1
// Constructors count:       LCOM4 = 2
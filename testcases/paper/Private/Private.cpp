class Private {
    int i, j;
public:
    void publicMethod() {
        i = 1;
    }
private:
    void privateMethod() {
        j = 2;
    }
};

// Private don't count: LCOM4 = 1
// Private count:       LCOM4 = 2
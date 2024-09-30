class Array {
    int i [50];
    int(&j)[50] = i;
    int k [100];

    void method1() {
        i[1] = 1;
    }

    void method2() {
        i[2] = 2;
    }

    void method3() {
        j[1] = 3;
    }

    void method4() {
        j[2] = 3; 
    }

    void method5() {
        k[1] = 3; 
    }
};
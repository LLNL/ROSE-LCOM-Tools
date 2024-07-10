namespace OuterNamespace {
    namespace InnerNamespace {
        int x = 1;
        int y = 2;

        void methodOne() {
            x++;
        }

        void methodTwo() {
            y++;
        }
    };

    void methodThree() {
        InnerNamespace::x++;
    }

    void methodFour() {
        InnerNamespace::y++;
    }
}

// Expected LCOM4 of OuterNamespace::InnerNamespace = 2
// Result: LCOM4 of OuterNamespace::InnerNamespace = 2

// Expected LCOM4 of OuterNamespace = 2
// Result: LCOM4 of OuterNamespace = 2
class myClass
{
    public:
        int item;
        float item2;
        double item3;

        int methodOne() {
            return item;
        }

        float methodTwo() {
            return item2;
        }

        double methodThree() {
            return item3;
        }
};

// Expected LCOM4 = 3
// Result from tool: LCOM4 = 3
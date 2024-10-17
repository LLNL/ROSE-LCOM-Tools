class myClass
{
    public:
        int item;

        int methodOne();
        int methodTwo();
        int methodFive();

        int methodThree() {
            return item;
        }

        int methodFour() {
            return item;
        }
};


// Expected LCOM4 = 1 if --filter-undefined-methods is set, 4 otherwise
// Result from tool: LCOM4 = 1, 4
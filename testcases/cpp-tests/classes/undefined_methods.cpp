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


// Expected LCOM4 = 1
// Result from tool: LCOM4 = 1
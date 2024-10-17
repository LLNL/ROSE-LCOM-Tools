class otherClass
{
    public:
        int item1;
        int item2;
};

class myClass
{
    public:
        otherClass otherClass;

        int methodOne() {
            return otherClass.item1;
        }

        int methodTwo() {
            return otherClass.item2;
        }


};

// Access to otherClass doesn't count as shared attributes: LCOM4 = 2
// Access to otherClass does count as shared attributes: LCOM4 = 1
// Result from tool: LCOM4 = 1
class otherClass
{
    public:
        int otherItem1;
        int otherItem2;
};

class myClass
{
    public:
        int item;
        otherClass otherClass;

        int methodOne() {
            return otherClass.otherItem1;
        }

        int methodTwo() {
            return otherClass.otherItem2;
        }


};


// Access to otherClass doesn't count as shared attributes: LCOM4 = 2
// Access to otherClass does count as shared attributes: LCOM4 = 1
// Result from tool: LCOM4 = 1
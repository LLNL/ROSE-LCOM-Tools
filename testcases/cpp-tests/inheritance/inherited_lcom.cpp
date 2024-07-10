class baseClass
{
    public:
        int item;
        int item2;

        int methodOne() {
            return item;
        }

        int methodTwo() {
            return item2;
        }
};

class newClass : public baseClass
{
    public:
        int item3;

        int methodThree() {
            return item3;
        }
};

// baseClass has an LCOM4 of 2, newClass would be expected to have an LCOM4 of 3 if the inheritance is taken into account, but this is not the case for standard LCOM
// Expected LCOM4: 1
// Result LCOM4: 1
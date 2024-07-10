class baseClass
{
    public:
        int item;

        baseClass(int x)
        {
            item = x;
        }
};

class baseClass2
{
    public:
        int item2;

        baseClass2(int y)
        {
            item2 = y;
        }
};

class derivedClass : public baseClass, public baseClass2
{
    public:
        int item3;

        derivedClass(int x, int y, int z) : baseClass(x), baseClass2(y)
        {
            item3 = z;
        }
};

// Expected LCOM4: 1
// Result from tool: LCOM4 of derivedClass = 1, 
//                   LCOM4 of baseClass = 1, 
//                   LCOM4 of baseClass2 = 1
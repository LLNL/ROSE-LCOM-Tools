class baseClass
{
    protected:
        int item;

    public:
        baseClass(int x)
        {
            item = x;
        }
};

class derivedClass : public baseClass
{
    public:
        int item2;

        derivedClass(int x, int y) : baseClass(x)
        {
            item2 = y;
        }
};

// Result from tool: LCOM4 of derivedClass = 1, 
//                   LCOM4 of baseClass = 1
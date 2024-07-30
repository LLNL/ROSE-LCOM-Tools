class myClass
{
    public:
        int item;
        
        myClass() : item(0)
        {
        }

        myClass(int i) : item(i)
        {
        }

        myClass(int i, int j) : item(i - j)
        {
        }
};


// Expected LCOM4 = 0 if constructors are not counted, 1 otherwise
// Result from tool: LCOM4 = 0
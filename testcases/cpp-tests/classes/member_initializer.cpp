class myClass
{
    public:
        int item;
        int item2;
        
        myClass() : item(0)
        {
        }

        myClass(int i) : item(i)
        {
        }

        myClass(int i, int j) : item(i - j)
        {
        }

        myClass(double d) : item2(d)
        {
        }
};


// Expected LCOM4 = null if --filter-ctors-dtors, 2 otherwise
// Result from tool: LCOM4 = null, 2
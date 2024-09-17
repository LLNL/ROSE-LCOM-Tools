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


// Expected LCOM4 = null if --filter-ctors-dtors, 1 otherwise
// Result from tool: LCOM4 = null, 1
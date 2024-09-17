class myClass
{
    public:
        int item;
        int item2;
        
        myClass() : item(0)
        {
        }

        myClass(int i) : item2(i)
        {
        }
};


// Expected LCOM4 = null if --filter-ctors-dtors, 2 otherwise
// Result from tool: LCOM4 = null, 2
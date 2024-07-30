class myClass
{
    public:
        int item;
        int item2;
        
        myClass() : item(0)
        {
        }

        myClass(int i, int j) : item(i), item2(j)
        {
        }
};


// Expected LCOM4 = 2
// Result from tool: LCOM4 = 2 // TODO: test failed (despite accurate result)
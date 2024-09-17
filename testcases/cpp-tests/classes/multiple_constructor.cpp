class myClass
{
    public:
        int item;
        
        myClass() {
            item = 0;
        }

        myClass(int i) {
            item = i;
        }

        myClass(int i, int j) {
            item = i - j;
        }
};

// Expected LCOM4 = 1
// Result from tool: LCOM4 = 1
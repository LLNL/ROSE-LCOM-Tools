class myClass
{
    public:
        int item;
        int item2;
        int item3;
        
        myClass() {
            item = 0;
        }

        myClass(int i) {
            item2 = i;
        }

        myClass(int i, int j) {
            item3 = i - j;
        }
};

// Expected LCOM4 = 3
// Result from tool: LCOM4 = 3
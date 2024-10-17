class myClass
{
    public:
        int item;
        int item2;
        
        myClass() {
            item = 0;
        }

        myClass(int i) {
            item = i;
        }

        myClass(int i, int j) {
            item = i - j;
        }

        myClass(double d) {
            item2 = d;
        }
};

// Expected LCOM4 = 2
// Result from tool: LCOM4 = 2
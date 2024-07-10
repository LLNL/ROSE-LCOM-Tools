class myClass
{
    public:
        int item;

        int methodOne() {
            return item;
        }

        int methodTwo() {
            return item + 1;
        }

};

int main()
{
    myClass obj;
    return 1;
}

// Regular class instantiations are not special in ROSE
// Expected LCOM4 = 1
// Result from tool: LCOM4 = 1
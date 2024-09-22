class XX;

template < template < class XX > class X > class myClass
{
    public:
        X<XX> item;
        int item2;

        X<XX> methodOne() {
            return item;
        }

        X<XX> methodTwo() {
            return item;
        }

        int methodThree() {
            return item2;
        }
}; 

// Expected LCOM4: 2 
// Result LCOM4: 2
template < class X, class Y, class Z > class myClass
{
    public:
        X item;
        Y item2;
        Z item3;

        X methodOne() {
            return item;
        }

        Y methodTwo() {
            return item2;
        }

        Z methodThree() {
            return item3;
        }
}; 

// Expected LCOM4: 3 for myClass, 2 for X
// Result LCOM4: 3 for myClass, 2 for X
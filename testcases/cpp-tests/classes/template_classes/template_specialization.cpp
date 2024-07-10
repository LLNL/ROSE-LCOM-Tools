class XX;

template < class XX > class X
{
    public:
        XX item;
        XX item2;

        XX methodOne() {
            return item;
        }

        XX methodTwo() {
            return item2;
        }
};

// Template specialization
template <> class X <int>
{
    public:
        int item;

        int methodThree() {
            return item;
        }

        int methodFour() {
            return item + 1;
        }
};

// Expected LCOM4: 2 for X <XX>, 1 for X <int>
// Result LCOM4: 2 for X <XX>, 1 for X <int>
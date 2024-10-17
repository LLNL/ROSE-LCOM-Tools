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

        XX methodThree() {
            return item;
        }
};

// Template specialization
template <> class X <int>
{
    public:
        int item;
        int item2;

        int methodOne() {
            return item;
        }

        int methodFTwo() {
            return item + 1;
        }

        int methodThree() {
            return item2;
        }
};

// Expected LCOM4: 2 for X <XX>, 2 for X <int>
// Result LCOM4: 2 for X <XX>, 2 for X <int>
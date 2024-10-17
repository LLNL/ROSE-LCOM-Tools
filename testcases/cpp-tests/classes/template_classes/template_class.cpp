template <typename T>
class myClass
{
    public:
        T item;
        T item2;

        T methodOne() {
            return item;
        }

        T methodTwo() {
            return item2;
        }

        T methodThree() {
            return item + 1;
        }

        T methodFour() {
            return item2 + 1;
        }
};

// Expected LCOM4 = 2
// Result from tool: LCOM4 = 2
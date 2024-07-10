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
};

// Expected LCOM4 = 2
// Result from tool: LCOM4 = 2
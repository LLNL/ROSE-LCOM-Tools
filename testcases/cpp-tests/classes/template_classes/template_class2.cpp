template <typename T>
class myClass
{
    public:
        T item;

        T methodOne() {
            return item;
        }

        T methodTwo() {
            return item + 1;
        }
};

// Expected LCOM4 = 1
// Result from tool: LCOM4 = 1
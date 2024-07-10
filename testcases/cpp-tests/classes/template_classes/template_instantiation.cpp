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

int main()
{
    myClass<int> obj;
    return 1;
}

// ROSE generates an additional node for the template class instantiation
// Expected LCOM4 = 1 for declaration, 1 for instantiation
// Result from tool: LCOM4 = 1 for declaration, 2 for instantiation //TODO: test failed

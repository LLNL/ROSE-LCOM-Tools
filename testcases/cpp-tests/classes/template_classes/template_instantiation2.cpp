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
// Method definitions only appear in the AST if they are called on the instantiated object
// If methods go uncalled, they should be filtered out prior to LCOM4 calculation

// Expected LCOM4 = 1 for declaration, 0 for instantiation (class should be empty)
// Result from tool: LCOM4 = 1 for declaration, 2 for instantiation TODO: test failed

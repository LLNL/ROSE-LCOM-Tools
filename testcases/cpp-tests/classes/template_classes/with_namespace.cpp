namespace myNamespace
{
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
}